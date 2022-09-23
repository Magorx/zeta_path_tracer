#include "PathTracer.hpp"
// #include "sfml_interface/interface.h"

#include "utils/logger.h"

#include "scenes/cornell_box.h"

// ============================================================================

namespace config = zephyr::tracer::config;

int main(int argc, char* argv[]) {
    Brans::srand_sse(time(NULL));
    srand(time(NULL));

    logger.page_cut();

    config::RenderT conf_render(REAL_SCREEN_WIDTH * RESOLUTION_COEF, REAL_SCREEN_HEIGHT * RESOLUTION_COEF,
                                MAX_TRACE_DEPTH,
                                PIXEL_SAMPLING,
                                GAMMA_CORRECTION,
                                BACKGROUND_COLOR);

    config::VerbosityT  conf_verbos(VERBOSITY);
    config::SystemT     conf_sysinf(vec3d_randlong() % 10000, DEFAULT_THREADS_CNT, nullptr);
    config::FullT       conf_pt(conf_render, conf_verbos, conf_sysinf);

    conf_pt.update_from_command_line_args(argc, argv);

    logger.info("Zepher", "process (%d) started", conf_pt.sysinf.timestamp);
    logger.info("Zepher", "using (%d) threads", conf_pt.sysinf.kernel_cnt);

    Scene *scene = cornell_box_scene();
    logger.info("Zepher", "scene prepared");

    // SFML_Interface *interface = new SFML_Interface("Zepher", scene, conf_pt, WINDOW_WIDTH, WINDOW_HEIGHT, PIXEL_SAMPLING);

    // logger.info("sfml_interface", "launching, press F to make a screenshot");

    // logger.page_cut("interface.run()");

    // {
    // // LogLevel loglvl(logger, 0, 10); // turn off the logging

    // interface->set_postprocs(ACCUMULATOR_FRAME_POSTPROC,
    //                          RENDERED_FRAME_POSTPROC,
    //                          ACCUMULATOR_FRAME_POSTPROC_RADIUS,
    //                          RENDERED_FRAME_POSTPROC_RADIUS);

    // auto interface_interactor = new std::thread(&SFML_Interface::run, interface);
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // interface->interaction_loop();

    // interface_interactor->join();
    // }

    zephyr::tracer::render_from_rtask_file(scene, conf_pt);

    logger.page_cut();

    logger.info("Zepher", "process [%d] finished", conf_pt.sysinf.timestamp);

    return 0;
}

//=============================================================================

