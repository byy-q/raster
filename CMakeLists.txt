cmake_minimum_required(VERSION 3.15)

project(Raster LANGUAGES CXX)

add_library(raster STATIC
    src/tool.cpp
)

target_include_directories(raster
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_compile_features(raster PUBLIC cxx_std_17)

include(CTest)

if(BUILD_TESTING)
    add_executable(instance_camera_transform_test
        tests/instance_camera_transform_test.cpp
    )
    target_link_libraries(instance_camera_transform_test PRIVATE raster)
    add_test(NAME instance_camera_transform COMMAND instance_camera_transform_test)
endif()
