cmake -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles" -B"build" -S"."
make -C"build" -j8
cp build/mcc mcc