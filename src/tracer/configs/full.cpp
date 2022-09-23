#include "full.h"


#include "settings.h"


namespace zephyr::tracer::config {

FullT from_command_line(int argc, char* argv[]) {
    argparse::ArgumentParser program(PROJECT_NAME, PROJECT_VERSION);

    FullT config;
    
    config.argparse_args(program);
    
    program.parse_args(argc, argv);
    
    config.argparse_scan(program);

    return config;
}

} // namespace zephyr::tracer::config
