find_package(GTest REQUIRED)

add_executable(test_mobject mobject_test.cpp)
target_link_libraries(test_mobject manimcpp GTest::GTest GTest::Main)

add_executable(test_utils utils_test.cpp)
target_link_libraries(test_utils manimcpp GTest::GTest GTest::Main)

add_test(NAME MobjectTest COMMAND test_mobject)
add_test(NAME UtilsTest COMMAND test_utils)