cmake_minimum_required(VERSION 3.22)

project(material VERSION 1.0)

add_library(${PROJECT_NAME} SHARED
        ./source/bulk_material_asset.c
        ./source/indexed_material_asset.c
        ./source/material_asset.c
        ./include/material/internal/module.h
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC library)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        )