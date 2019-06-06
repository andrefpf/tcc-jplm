# JPL-RC

Build instructions:
  ```bash
  ~$ cd jplm
  ~/jplm/$ mkdir build; cd build
  ~/jplm/build/$ cmake ..
  ~/jplm/build/$ make -j
  ```

Test instructions:
  ```bash
  ~/jplm/build/$ ctest
  ```


ctest --verbose //shows all tests
ctest --rerun-failed //runs again the failed tests

//to see a color output of the tests using ctest:
export GTEST_COLOR=1
//it can be useful to rerun the failed test to pinpoint the error:
ctest --verbose --rerun-failed