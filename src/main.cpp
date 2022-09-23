#include "PathTracer.hpp"
#include "scenes/cornell_box.h"

#include <utils/logger.h>

// ============================================================================

int main(int argc, char* argv[]) {
    Brans::srand_sse(time(nullptr));
    srand(time(nullptr));

    auto config = zephyr::tracer::config::from_command_line(argc, argv);

    logger.set_verb_level((Logger::Level) config.verbos.verbosity);

    logger.log(0, "hola", "KCTF", "Welcome to Zephyr ray-tracer, have a nice day");

    logger.info("Zepher", "process (%d) started", config.sysinf.timestamp);
    logger.info("Zepher", "using (%d) threads"  , config.sysinf.kernel_cnt);

    Scene *scene = cornell_box_scene();
    logger.info("Zepher", "scene prepared");

    logger.info("Zepher", "rendering...");
    zephyr::tracer::render_from_rtask_file(scene, config);
    logger.info("Zepher", "done...");

    logger.page_cut();

    logger.info("Zepher", "process [%d] finished", config.sysinf.timestamp);

    return 0;
}
