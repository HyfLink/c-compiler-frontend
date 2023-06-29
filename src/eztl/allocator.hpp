/*
 *  (1) malloc_alloc
 *  (2) default_alloc
 *  (3) allocator
 */
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include "move.hpp"

namespace eztl
{
    //* throw bad alloc

    static inline void throw_bad_alloc() {
        fprintf(stderr, "out of memory\n");
        exit(EXIT_FAILURE);
    }

    //* malloc_alloc

    class malloc_alloc {
    public:
        static inline void *allocate(size_t size) {
            void *result = malloc(size);
            return result ? result : oom_malloc(size);
        }

        static inline void *reallocate(void *ptr, size_t /* oldsz */, size_t newsz) {
            void *result = realloc(ptr, newsz);
            return result ? result : oom_realloc(ptr, newsz);
        }

        static inline void deallocate(void *ptr, size_t /* size */) {
            free(ptr);
        }

        static inline void (*set_oom_handler(void (*new_handler)()))() {
            void (*old_handler)() = s_oom_handler;
            s_oom_handler = new_handler;
            return old_handler;
        }

    private:
        static void *oom_malloc(size_t size);
        static void *oom_realloc(void *ptr, size_t newsz);
        static void (*s_oom_handler)();
    };

    void (*malloc_alloc::s_oom_handler)() = nullptr;

    void *malloc_alloc::oom_malloc(size_t size) {
        for (;;) {
            if (s_oom_handler == nullptr)
                throw_bad_alloc();
            (*s_oom_handler)();

            void *result = malloc(size);
            if (result) return result;
        }
    }

    void *malloc_alloc::oom_realloc(void *ptr, size_t size) {
        for (;;) {
            if (s_oom_handler == nullptr)
                throw_bad_alloc();
            (*s_oom_handler)();

            void *result = realloc(ptr, size);
            if (result) return result;
        }
    }

    //* default_alloc

    class default_alloc {
    public:
        static void *allocate(size_t size);
        static void deallocate(void *ptr, size_t size);
        static void *reallocate(void *ptr, size_t oldsz, size_t newsz);

    private:
        union obj_t {
            obj_t *next;
            char data[1];
        };

        static constexpr size_t ALIGN = 8;
        static constexpr size_t MAX_BYTES = 128;
        static constexpr size_t NFREELISTS = MAX_BYTES / ALIGN;
        static constexpr inline size_t roundup(size_t bytes) { return (bytes + ALIGN - 1) & ~(ALIGN - 1); }
        static constexpr inline size_t listindex(size_t bytes) { return (bytes + ALIGN - 1) / (ALIGN) - 1; }
        
        /// return an object of @p size, and adds to free list of @p size
        static void *refill(size_t size);
        /// allocates a chuck for objects of @p size, @p nobjs may be reduced
        static void *chunk_alloc(size_t size, int &nobjs);

    private:
        static obj_t *s_freelist[NFREELISTS];
        static char *s_start_free;
        static char *s_end_free;
        static size_t s_heap_size;
    };

    default_alloc::obj_t *default_alloc::s_freelist[NFREELISTS] = {nullptr};
    char *default_alloc::s_start_free = nullptr;
    char *default_alloc::s_end_free = nullptr;
    size_t default_alloc::s_heap_size = 0;

    void *default_alloc::allocate(size_t size)
    {
        if (size > (size_t)MAX_BYTES)
            return malloc_alloc::allocate(size);

        obj_t **pfreelist = s_freelist + listindex(size);

        if (*pfreelist == nullptr)
            return refill(roundup(size));

        obj_t *temp = *pfreelist;
        *pfreelist = (*pfreelist)->next;
        return temp;
    }

    void default_alloc::deallocate(void *ptr, size_t size)
    {
        if (size > MAX_BYTES)
            return malloc_alloc::deallocate(ptr, size);

        obj_t **pfreelist = s_freelist + listindex(size);
        static_cast<obj_t *>(ptr)->next = *pfreelist;
        *pfreelist = static_cast<obj_t *>(ptr);
    }

    void *default_alloc::reallocate(void *ptr, size_t oldsz, size_t newsz)
    {
        if (oldsz > MAX_BYTES && newsz > MAX_BYTES)
            return malloc_alloc::reallocate(ptr, oldsz, newsz);
        if (roundup(oldsz) == roundup(newsz))
            return ptr;

        void *result = allocate(newsz);
        memcpy(result, ptr, newsz > oldsz ? oldsz : newsz);
        deallocate(ptr, oldsz);
        return result;
    }

    void *default_alloc::refill(size_t size)
    {
        int nobjs = 20;
        char *chuck = static_cast<char *>(chunk_alloc(size, nobjs));
        if (nobjs == 1) return static_cast<void *>(chuck);

        obj_t **pfreelist = s_freelist + listindex(size);
        obj_t *result = reinterpret_cast<obj_t *>(chuck);
        obj_t *next_obj = reinterpret_cast<obj_t *>(chuck + size);
        obj_t *current_obj;
        *pfreelist = next_obj;

        for (int i = 1;; ++i) {
            current_obj = next_obj;
            next_obj = reinterpret_cast<obj_t *>(reinterpret_cast<char *>(next_obj) + size);
            current_obj->next = (nobjs == i + 1) ? nullptr : next_obj;
            if (current_obj->next == nullptr) break;
        }

        return result;
    }
    
    void *default_alloc::chunk_alloc(size_t size, int &nobjs)
    {
        // re-caculate nobjs if left_bytes is not enough
        size_t left_bytes = s_end_free - s_start_free;
        size_t total_bytes = size * nobjs;
        if (size <= left_bytes && left_bytes <= total_bytes) {
            nobjs = (int)(left_bytes / size);
            total_bytes = size * nobjs;
        }

        // allocates at least one obj 
        if (size <= left_bytes) {
            char *result = s_start_free;
            s_start_free += total_bytes;
            return result;
        }

        // try to make use of left-over piece, put them into free list
        if (left_bytes > 0) {
            obj_t **pfreelist = s_freelist + listindex(left_bytes);
            reinterpret_cast<obj_t *>(s_start_free)->next = *pfreelist;
            *pfreelist = reinterpret_cast<obj_t *>(s_start_free);
        }

        // try to get memory from malloc()
        size_t bytes_to_get = 2 * total_bytes + roundup(s_heap_size >> 4);
        size_t bytes_got = bytes_to_get;
        s_start_free = static_cast<char *>(malloc(bytes_to_get));

        // out of memory, find unused obj in the free list
        if (s_start_free == nullptr) {
            for (size_t isize = size; isize <= MAX_BYTES; isize += ALIGN) {
                obj_t **pfreelist = s_freelist + listindex(isize);
                if (*pfreelist != nullptr) {
                    s_start_free = reinterpret_cast<char *>(*pfreelist);
                    *pfreelist = (*pfreelist)->next;
                    bytes_got = isize;
                    break;
                }
            }
        }

        // there is no way to get memory
        if (s_start_free == nullptr)
            throw_bad_alloc();

        // enough memory is put into the heap, get memory for objs recursively
        s_end_free = s_start_free + bytes_got;
        s_heap_size += bytes_got;
        return chunk_alloc(size, nobjs);
    }

    //* allocator

    template <typename T, typename Alloc = default_alloc>
    struct allocator {
        typedef T           value_type;
        typedef T *         pointer;
        typedef T &         reference;
        typedef const T *   const_pointer;
        typedef const T &   const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        constexpr allocator() {}
        constexpr allocator(const allocator<T, Alloc> &) {}
        template <typename U>
        constexpr allocator(const allocator<U, Alloc> &) {}
        ~allocator() {}

        template <typename U>
        struct rebind { typedef allocator<U, Alloc> other; };

        static inline pointer address(reference ref) { return &ref; }
        static inline const_pointer address(const_reference ref) { return &ref; }
        static inline size_type max_size() { return size_type(-1) / sizeof(T); }

        static inline pointer allocate() {
            return pointer(Alloc::allocate(sizeof(T)));
        }
        static inline pointer allocate(size_type nitems) {
            return pointer(nitems ? Alloc::allocate(nitems * sizeof(T)) : nullptr);
        }

        static inline void deallocate(pointer ptr) {
            Alloc::deallocate(static_cast<void *>(ptr), sizeof(T));
        }
        
        static inline void deallocate(pointer ptr, size_type nitems) {
            Alloc::deallocate(static_cast<void *>(ptr), nitems * sizeof(T));
        }

        static inline pointer reallocate(pointer ptr, size_type old_nitems, size_type new_nitems)
        {
            return pointer(Alloc::reallocate(ptr, old_nitems * sizeof(T), new_nitems * sizeof(T)));
        }
        template <typename... Args>
        static inline void construct(pointer ptr, Args... args) {
            new (ptr) T(forward<Args>(args)...);
        }
        static inline void construct(pointer ptr) { new (ptr) T(); }
        static inline void destroy(pointer ptr) { ptr->~T(); }
    };

} // namespace eztl
