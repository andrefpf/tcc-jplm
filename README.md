# JPL-RC

Build instructions:
  ```cd jplm
  mkdir build
  cmake ..
  make -j```

Test instructions:
  ```~/jplm/build/$ ctest```


ctest --verbose //shows all tests
ctest --rerun-failed //runs again the failed tests

//to see a color output of the tests using ctest:
export GTEST_COLOR=1
//it can be useful to rerun the failed test to pinpoint the error:
ctest --verbose --rerun-failed