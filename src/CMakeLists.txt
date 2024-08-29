add_library(${PROJECT_NAME} SHARED
        map_file.cpp
        map.cpp
        brush.cpp
        qmath.cpp
        face.cpp
        entity_solid.cpp
        wrapper.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "../include/")
