cmake_minimum_required(VERSION 3.15)
project(zeta_path_tracer LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O3 -g ${WARNING_FLAGS} -fdeclspec -march=native")

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)

add_executable(pather
        src/hits/aabb.cpp
        src/acceleration_structures/bvh.cpp
        src/camera_light_scene/camera.cpp
        src/camera_light_scene/light.cpp
        src/collections/collection_hittables.cpp
        src/collections/collection_instances.cpp
        src/collections/collection_lights.cpp
        src/collections/collection_materials.cpp
        src/collections/collection_textures.cpp
        src/color/color.cpp
        src/color/rgba.cpp
        src/hits/hit_record.cpp
        src/hits/hittable.cpp
        src/hits/hittable_list.cpp
        src/main.cpp
        src/material/material.cpp
        src/tracer/path_tracer.cpp
        src/utils/ray.cpp
        src/tracer/render_task.cpp
        src/utils/vec3d.cpp
        src/sfml_interface/interface.cpp
        src/image/denoiser.cpp
        src/multithreading/mt_path_tracer.cpp src/utils/fast_random.hpp)

target_link_libraries(pather sfml-graphics sfml-window sfml-system)        

add_custom_target(convert
        COMMAND convert image.ppm image.png
        )

add_custom_target(view
        COMMAND eog -f image.png
        )

