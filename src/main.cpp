#include "PathTracer.hpp"
#include "scenes/cornell_box.h"

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

    kctf::Timer timer;

    logger.info("Zepher") << "rendering...";
    zephyr::tracer::render_from_rtask_file(scene, config);
    logger.info("Zepher") << "done...";

    timer.stop();
    timer.print();

    // logger.page_cut();

    logger.info("Zepher") << "process [" << config.sysinf.timestamp << "] finished";

    return 0;
}
