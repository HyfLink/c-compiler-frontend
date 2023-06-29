# eztl

* [x] template meta
  1. type_traits
     1. type traits
     2. is integer
  2. constants
     1. integral_constant
     2. bool_constant
     3. true_type & false_type

* [ ] memory
  1. construct
     1. construct(ptr, args...)
     2. destroy(first[, last])
  2. allocator
     1. malloc_alloc
     2. default_alloc
     3. allocator
  3. [ ] uninitialized
     1. uninitialized_copy(first, last, result)
     2. uninitialized_fill(first, last, val)
     3. uninitialized_fill_n(first, cnt, result)

* [ ] iterator
  1. iterator tag
  2. iterator base
  3. iterator function
     1. distance(iter1, iter2)
     2. advance(iter, dis)
  4. [ ] special iterator
     1. {back, front, } insert iterator
     2. reverse iterator
     3. {istream, ostream, istreambuf, ostreambuf} iterator

* [ ] sequence container
  1. [ ] vector
  2. [ ] list
  3. [ ] deque

* [x] function
  1. functor
    1. plus, minus, multiplies, divides, modulus, negate
    2. equal_to, not_equal_to, less, less_equal, greater, greater_equal
    3. logical_{and, or, not}
    4. bit_{and, or, xor, not}
    5. identity, project{1st, 2nd}, select{1st, 2nd}
  2. functions
    6. not1 & not2
    7. fun_ptr
    8. mem_fun & mem_fun_ref

* [ ] algorithm
  1. algobase
    1. swap  & iter_swap
    2. min   & max
    3. copy  & copy_backward & copy_n
    4. fill  & fill_n 
    5. equal & mismatch
    6. lexicographical_compare
  3. heap
    1. push_heap(first, last{, comp})
    2. pop_heap(first, last{, comp})
    3. make_heap(first, last{, comp})
    4. sort_heap(first, last{, comp})
  2. [ ] algorithm
    1. median
    2. for_each
    3. find{_if}
    4. adjacent_find
    5. count{_if}
    6. search{_n}
    7. swap_ranges
    8. transform
    9. replace{_copy}{_if}
    10. generate{_n}
    11. remove{_copy}{_if}
    12. unique{_copy}
    13. reverse{_copy}
    14. rotate{_copy}
    15. random_shuffle
    16. random_sample{_n}
    17. {stable_}partition
    18. {stable_}sort
    19. partial_sort{_copy}
    20. n_th_element
    21. lower_bound
    22. upper_bound
    23. equal_range
    24. binary_search
    25. {inplace_}merge
    26. includes
    27. set_union
    28. set_intersection
    29. set_difference
    30. set_symmetric_difference
    31. max_element
    32. min_element
    33. {next, prev}_permutation
    34. find_first_of
    35. find_end
    36. is_heap
    37. is_sorted

* [x] pair
  1. pair comparison
  2. pair function
     1. make_pair(first, second)
     2. get<0/1>(pair)

