cmake_minimum_required(VERSION 3.10)
project(Moka)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include_directories("inc/")

file(
  GLOB SOURCE_FILES 
  "src/moka/assets/text_loader.cpp"
  "src/moka/assets/resource_dir.cpp"
  "src/moka/project/project.cpp"
  "src/moka/helpers/cli_parser.cpp"
)

file (
  GLOB TESTS_SOURCE_FILES
  "tests/test_main.cpp"
  "tests/test_observable.cpp"
)

add_library(MokaUtils STATIC ${SOURCE_FILES})
target_link_libraries(MokaUtils ${MOKA_UTILS_DEP_LIBS})
# add_link_options(-no-pie)

set_property(TARGET MokaUtils PROPERTY CXX_STANDARD 20)
set_property(TARGET MokaUtils PROPERTY CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

if ("${UNIT_TESTS}" STREQUAL "")
  set(UNIT_TESTS 0)
endif()

if (NOT ("${UNIT_TESTS}" STREQUAL "0"))
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  if (WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
  endif()

  enable_testing()

  include(GoogleTest)
  add_executable(MokaUtils_UnitTests ${TESTS_SOURCE_FILES})
  target_link_libraries(MokaUtils_UnitTests gtest MokaUtils)
  set_property(TARGET MokaUtils_UnitTests PROPERTY CXX_STANDARD 20)
  set_property(TARGET MokaUtils_UnitTests PROPERTY CXX_STANDARD_REQUIRED 20)

  gtest_discover_tests(MokaUtils_UnitTests)
endif()

set_property(GLOBAL PROPERTY MOKA_UTILS 1)
set_property(GLOBAL PROPERTY MOKA_UTILS_INCS "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set_property(GLOBAL PROPERTY MOKA_UTILS_LIBS MokaUtils)
