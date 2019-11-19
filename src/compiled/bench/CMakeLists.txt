add_executable(bench
        bench.cpp
        vector_compare.cpp)
target_link_libraries(bench benchmark string-view-lite)