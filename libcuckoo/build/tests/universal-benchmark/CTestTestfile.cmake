# CMake generated Testfile for 
# Source directory: /Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/tests/universal-benchmark
# Build directory: /Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pure_read "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--reads" "100" "--prefill" "75" "--total-ops" "500" "--initial-capacity" "23")
add_test(pure_insert "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--inserts" "100" "--total-ops" "75" "--initial-capacity" "23")
add_test(pure_erase "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--erases" "100" "--prefill" "75" "--total-ops" "75" "--initial-capacity" "23")
add_test(pure_update "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--updates" "100" "--prefill" "75" "--total-ops" "500" "--initial-capacity" "23")
add_test(pure_upsert "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--upserts" "100" "--prefill" "25" "--total-ops" "200" "--initial-capacity" "23")
add_test(insert_expansion "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--inserts" "100" "--initial-capacity" "4" "--total-ops" "13107200")
add_test(read_insert_expansion "/Users/rosscoker/School/15418/LockFreeCuckooHash/libcuckoo/build/tests/universal-benchmark/universal_benchmark" "--reads" "80" "--inserts" "20" "--initial-capacity" "10" "--total-ops" "4096000")
