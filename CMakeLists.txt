cmake_minimum_required(VERSION 3.17)
project(zeta_path_tracer LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3")
set(CMAKE_CXX_FLAGS_DEBUG   "-g3 -O0")

add_executable(zeta_path_tracer
        src/hits/aabb.cpp
        src/hits/aabb.h
        src/utils/bvh.cpp
        src/utils/bvh.h
        src/cameraLightColor/camera.cpp
        src/cameraLightColor/camera.h
        src/collections/collection_hittables.cpp
        src/collections/collection_hittables.h
        src/collections/collection_instances.cpp
        src/collections/collection_instances.h
        src/collections/collection_lights.cpp
        src/collections/collection_lights.h
        src/collections/collection_materials.cpp
        src/collections/collection_materials.h
        src/collections/collection_textures.cpp
        src/collections/collection_textures.h
        src/color/color.cpp
        src/color/color.h
        src/hits/hit_record.cpp
        src/hits/hit_record.h
        src/hits/hittable.cpp
        src/hits/hittable.h
        src/hits/hittable_list.cpp
        src/hits/hittable_list.h
        src/cameraLightColor/light.cpp
        src/cameraLightColor/light.h
        src/main.cpp
        src/material/material.cpp
        src/material/material.h
        src/utils/path_tracer.cpp
        src/utils/path_tracer.h
        src/utils/path_tracer_constants.h
        src/utils/PathTracer.hpp
        src/utils/progress_bar.h
        src/utils/ray.cpp
        src/utils/ray.h
        src/utils/render_task.cpp
        src/utils/render_task.h
        src/cameraLightColor/scene.h
        src/color/texture.h
        src/utils/vec3d.cpp
        src/utils/vec3d.h)
