#add_definitions(--no-rtti --no-omit-frame-pointer)

add_executable(test
        test.cpp
        test_value.cpp
        #test_token.cpp
        #test_version.cpp
        )

target_link_libraries(test
        fmt::fmt-header-only
        string-view-lite
        gtest
        )