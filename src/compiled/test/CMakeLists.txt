#add_definitions(--no-rtti --no-omit-frame-pointer)

add_executable(test
        test.cpp
        test_version_token.cpp
        )

target_link_libraries(test gtest string-view-lite)