project(version CXX)

#add_definitions(--no-rtti)

add_library(version SHARED
#        ${CMAKE_CURRENT_LIST_DIR}/exceptions.hpp
#        ${CMAKE_CURRENT_LIST_DIR}/value.hpp
#        ${CMAKE_CURRENT_LIST_DIR}/token.hpp
#        ${CMAKE_CURRENT_LIST_DIR}/version.hpp
#        ${CMAKE_CURRENT_LIST_DIR}/_version.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Types.hpp
        ${CMAKE_CURRENT_LIST_DIR}/ComparableValue.hpp
        ${CMAKE_CURRENT_LIST_DIR}/NewToken.hpp
        ${CMAKE_CURRENT_LIST_DIR}/NewVersion.hpp
        ${CMAKE_CURRENT_LIST_DIR}/NewVersion.cpp
        )

set_target_properties(version PROPERTIES
        PREFIX "_"
        )

target_link_libraries(version
        fmt::fmt-header-only
        string-view-lite
        pybind11
        )