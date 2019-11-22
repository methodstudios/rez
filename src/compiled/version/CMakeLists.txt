project(version CXX)

#add_definitions(--no-rtti)

add_library(version SHARED
        ${CMAKE_CURRENT_LIST_DIR}/Types.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Exceptions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Comparable.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Token.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Version.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Version.cpp
        )

set_target_properties(version PROPERTIES
        PREFIX "_"
        )

target_link_libraries(version
        fmt::fmt-header-only
        string-view-lite
        pybind11
        )