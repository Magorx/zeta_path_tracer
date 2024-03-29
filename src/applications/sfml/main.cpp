#include "PathTracer.hpp"
#include "applications/sfml/interface/interface.h"

#include <utils/logger.h>

#include "scenes/cornell_box.h"

#include "extern/argparse.h"

#include <iostream>

// ============================================================================

namespace conf = zephyr::tracer::config;

int main(int argc, char* argv[]) {
    Brans::srand_sse(time(nullptr));
    srand(time(nullptr));
    
    // logger.set_verb_level(Logger::Level::info);

    conf::FullT config = conf::from_command_line(argc, argv);

    logger.log(0, "hola", "KCTF", "Welcome to Zephyr ray-tracer, have a nice day");

    logger.info("Zephyr", "process (%d) started", config.sysinf.timestamp);
    logger.info("Zephyr", "using (%d) threads"  , config.sysinf.kernel_cnt);

    Scene *scene = cornell_box_scene();
    logger.info("Zephyr", "scene prepared");

    SFML_Interface *interface = new SFML_Interface("Zephyr", scene, config, WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_SAMPLING);

    logger.info("sfml_interface", "launching, press F to make a screenshot");

    logger.page_cut("interface.run()");

    {
    // LogLevel loglvl(logger, 0, 10); // turn off the logging

    interface->set_postprocs(ACCUMULATOR_FRAME_POSTPROC,
                             RENDERED_FRAME_POSTPROC,
                             ACCUMULATOR_FRAME_POSTPROC_RADIUS,
                             RENDERED_FRAME_POSTPROC_RADIUS);

    auto interface_interactor = new std::thread(&SFML_Interface::run, interface);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    interface->interaction_loop();

    interface_interactor->join();
    }

    // logger.info("Zephyr", "rendering...");
    // zephyr::tracer::render_from_rtask_file(scene, config);

    logger.page_cut();

    logger.info("Zephyr", "process [%d] finished", config.sysinf.timestamp);

    return 0;
}

//=============================================================================

