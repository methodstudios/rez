add_executable(test
        test.cpp
        test_Comparable.cpp
        test_Token.cpp
        test_Version.cpp
        )

target_link_libraries(test
        fmt::fmt-header-only
        string-view-lite
        gtest
        )