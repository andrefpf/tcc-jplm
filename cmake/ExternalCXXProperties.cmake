message("Configuring CxxProperties library.")

project(cxxproperties-download NONE)


# Extracted from https://github.com/tschuchortdev/cmake_git_clone.git

# returns true if only a single one of its arguments is true
function(xor result)
  set(true_args_count 0)

  foreach(foo ${ARGN})
    if(foo)
      math(EXPR true_args_count "${true_args_count}+1")
    endif()
  endforeach()

  if(NOT (${true_args_count} EQUAL 1))
    set(${result} FALSE PARENT_SCOPE)
  else()
    set(${result} TRUE PARENT_SCOPE)
  endif()
endfunction()

function(at_most_one result)
  set(true_args_count 0)

  foreach(foo ${ARGN})
    if(foo)
      math(EXPR true_args_count "${true_args_count}+1")
    endif()
  endforeach()

  if(${true_args_count} GREATER 1)
    set(${result} FALSE PARENT_SCOPE)
  else()
    set(${result} TRUE PARENT_SCOPE)
  endif()
endfunction()

include(CMakeParseArguments)
find_package(Git)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "git not found!")
endif()

function(git_clone)

  cmake_parse_arguments(
      PARGS                                                               # prefix of output variables
      "QUIET"                                                             # list of names of the boolean arguments (only defined ones will be true)
      "PROJECT_NAME;GIT_URL;GIT_TAG;GIT_BRANCH;GIT_COMMIT;DIRECTORY"      # list of names of mono-valued arguments
      ""                                                                  # list of names of multi-valued arguments (output variables are lists)
      ${ARGN}                                                             # arguments of the function to parse, here we take the all original ones
  ) # remaining unparsed arguments can be found in PARGS_UNPARSED_ARGUMENTS

  if(NOT PARGS_PROJECT_NAME)
    message(FATAL_ERROR "You must provide a project name")
  endif()

  if(NOT PARGS_GIT_URL)
    message(FATAL_ERROR "You must provide a git url")
  endif()

  if(NOT PARGS_DIRECTORY)
    set(PARGS_DIRECTORY ${CMAKE_BINARY_DIR})
  endif()

  set(${PARGS_PROJECT_NAME}_SOURCE_DIR
      ${PARGS_DIRECTORY}/${PARGS_PROJECT_NAME}
      CACHE INTERNAL "" FORCE) # makes var visible everywhere because PARENT_SCOPE wouldn't include this scope

  set(SOURCE_DIR ${PARGS_PROJECT_NAME}_SOURCE_DIR)

  # check that only one of GIT_TAG xor GIT_BRANCH xor GIT_COMMIT was passed
  at_most_one(at_most_one_tag ${PARGS_GIT_TAG} ${PARGS_GIT_BRANCH} ${PARGS_GIT_COMMIT})

  if(NOT at_most_one_tag)
    message(FATAL_ERROR "you can only provide one of GIT_TAG, GIT_BRANCH or GIT_COMMIT")
  endif()

  if(NOT PARGS_QUIET)
    message(STATUS "downloading/updating ${PARGS_PROJECT_NAME}")
  endif()

  # first clone the repo
  if(EXISTS ${${SOURCE_DIR}})
    if(NOT PARGS_QUIET)
      message(STATUS "${PARGS_PROJECT_NAME} directory found, pulling...")
    endif()

    execute_process(
        COMMAND             ${GIT_EXECUTABLE} pull origin master
        COMMAND             ${GIT_EXECUTABLE} submodule update --remote
        WORKING_DIRECTORY   ${${SOURCE_DIR}}
        OUTPUT_VARIABLE     git_output)
  else()
    if(NOT PARGS_QUIET)
      message(STATUS "${PARGS_PROJECT_NAME} directory not found, cloning...")
    endif()

    execute_process(
        COMMAND             ${GIT_EXECUTABLE} clone ${PARGS_GIT_URL} --recursive
        WORKING_DIRECTORY   ${PARGS_DIRECTORY}
        OUTPUT_VARIABLE     git_output)
  endif()

  if(NOT PARGS_QUIET)
    message("${git_output}")
  endif()

  # now checkout the right commit
  if(PARGS_GIT_TAG)
    execute_process(
        COMMAND             ${GIT_EXECUTABLE} fetch --all --tags --prune
        COMMAND             ${GIT_EXECUTABLE} checkout tags/${PARGS_GIT_TAG} -b tag_${PARGS_GIT_TAG}
        WORKING_DIRECTORY   ${${SOURCE_DIR}}
        OUTPUT_VARIABLE     git_output)
  elseif(PARGS_GIT_BRANCH OR PARGS_GIT_COMMIT)
    execute_process(
        COMMAND             ${GIT_EXECUTABLE} checkout ${PARGS_GIT_BRANCH}
        WORKING_DIRECTORY   ${${SOURCE_DIR}}
        OUTPUT_VARIABLE     git_output)
  else()
    message(STATUS "no tag specified, defaulting to master")
    execute_process(
        COMMAND             ${GIT_EXECUTABLE} checkout master
        WORKING_DIRECTORY   ${${SOURCE_DIR}}
        OUTPUT_VARIABLE     git_output)
  endif()

  if(NOT PARGS_QUIET)
    message("${git_output}")
  endif()
endfunction()

git_clone(
    PROJECT_NAME    cxxproperties
    GIT_URL         https://github.com/pedrogarciafreitas/cxxproperties.git
    GIT_BRANCH      master
    DIRECTORY       "${CMAKE_CURRENT_BINARY_DIR}"
)
set(cxxproperties_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/cxxproperties/)
include_directories(${cxxproperties_INCLUDE})
