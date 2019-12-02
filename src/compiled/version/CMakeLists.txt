project(version CXX)

# version static library
add_library(version STATIC
        ${CMAKE_CURRENT_LIST_DIR}/Types.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Exceptions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Token.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Version.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Bound.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Global.cpp
        )

target_link_libraries(version
        string-view-lite
        )

# Python module
add_library(_version SHARED
        ${CMAKE_CURRENT_LIST_DIR}/Module.cpp
        )

set_target_properties(_version PROPERTIES
        PREFIX ""
        )

target_link_libraries(_version
        fmt::fmt-header-only
        pybind11
        version
        )