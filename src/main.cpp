#include "PathTracer.hpp"
#include "scenes/cornell_box.h"

#include <jobber/task.h>
#include <jobber/jobber.h>

#include <utils/logger.h>
#include <utils/timer.h>

// ============================================================================

int main(int argc, char* argv[]) {
    Brans::srand_sse(time(nullptr));
    srand(time(nullptr));

    auto config = zephyr::tracer::config::from_command_line(argc, argv);

    logger.set_log_level(kctf::LoggerT::Level::info);

    logger.nc_info("hola")("KCTF") << "Welcome to Zephyr ray-tracer, have a nice day";

    logger.info("Zepher") << "process (" << config.sysinf.timestamp << ") started";
    logger.info("Zepher") << "using (" << config.sysinf.kernel_cnt << ") workers";

    logger.info("Zepher") << "building scene and BVH";
    Scene *scene = cornell_box_scene(config);
    logger.info("Zepher") << "scene prepared";
    logger.info("Zepher") << "image " << scene->camera->res_w << "x" << scene->camera->res_h;

    kctf::Timer timer;

    // logger.info("Zepher") << "rendering...";
    // zephyr::tracer::render_from_rtask_file(scene, config);
    // logger.info("Zepher") << "done...";

    jobber::storage::ComplexityCappedT<
        zephyr::TaskT, 
        jobber::splitter::LinearBisecting<zephyr::TaskT>
    > capped_storage(
       jobber::splitter::LinearBisecting<zephyr::TaskT>{64*64}
    );

    jobber::pipeline::WorkerThreadedT<
        zephyr::TaskT,
        jobber::storage::ComplexityCappedT<
            zephyr::TaskT, 
            jobber::splitter::LinearBisecting<zephyr::TaskT>
        >
    > pipeline(
        capped_storage,
        config.sysinf.kernel_cnt
    );

    zephyr::TaskT::update_compute_context({*scene, config});

    zephyr::TaskT task(0, 0, config.render.screen.width, config.render.screen.height);
    pipeline.put(std::move(task));

    auto result = pipeline.await();

    save_rgb_to_ppm_image(
        "out.ppm",
        &result.image[0],
        config.render.screen.width,
        config.render.screen.height,
        config.render.gamma_correction
    );

    timer.stop();
    timer.print();

    // logger.page_cut();

    logger.info("Zepher") << "process [" << config.sysinf.timestamp << "] finished";

    return 0;
}
