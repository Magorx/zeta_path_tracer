cmake_minimum_required(VERSION 3.15)
project(zeta_path_tracer LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "${WARNING_FLAGS} -fdeclspec -march=native")

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)

add_executable(zephyr
        src/hits/aabb.cpp
        src/acceleration_structures/bvh.cpp

        src/camera_light_scene/camera.cpp
        src/camera_light_scene/light.cpp

        src/color/color.cpp
        src/color/rgba.cpp

        src/hits/hit_record.cpp

        src/hits/hittable.cpp
        src/hits/hittable_list.cpp

        src/material/material.cpp

        src/utils/vec3d.cpp
        src/utils/ray.cpp
        src/utils/timer.cpp
        src/utils/logger.cpp

        src/tracer/path_tracer.cpp
        src/tracer/render_task.cpp
        
        src/tracer/configs/full.cpp
        src/tracer/configs/render.cpp
        src/tracer/configs/system.cpp
        src/tracer/configs/verbosity.cpp

        src/applications/sfml/interface/interface.cpp

        src/image/denoiser.cpp
        src/multithreading/mt_path_tracer.cpp
        
        src/utils/fast_random.hpp
        
        src/collections/hittables/sphere.cpp
        src/collections/hittables/triangle.cpp
        src/collections/hittables/rect_xy.cpp
        src/collections/hittables/rect_xz.cpp
        src/collections/hittables/rect_yz.cpp
        src/collections/hittables/box.cpp
        src/collections/hittables/model.cpp

        src/collections/instances/translate.cpp
        src/collections/instances/instance.cpp
        src/collections/instances/rot_x.cpp
        src/collections/instances/rot_y.cpp
        src/collections/instances/rot_z.cpp

        src/collections/materials/lambertian.cpp
        src/collections/materials/metal.cpp
        src/collections/materials/dielectric.cpp

        src/collections/lights/diffuse.cpp

        src/collections/textures/solid_color.cpp
        src/collections/textures/checkered.cpp

        src/main.cpp
        )

target_link_libraries(zephyr sfml-graphics sfml-window sfml-system)        

add_custom_target(convert
        COMMAND convert image.ppm image.png
        )

add_custom_target(view
        COMMAND eog -f image.png
        )
