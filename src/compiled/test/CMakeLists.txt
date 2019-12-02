add_executable(test
        test.cpp
        test_Token.cpp
        test_Version.cpp
        test_Bound.cpp
        )

target_link_libraries(test
        fmt::fmt-header-only
        gtest
        version
        )