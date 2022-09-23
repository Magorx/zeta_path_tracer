#include "verbosity.h"

#include "settings.h"


namespace zephyr::tracer::config {

void VerbosityT::argparse_args(argparse::ArgumentParser &program) {
    program.add_optional_args_cut("\nConfig [verbosity]:");

    program.add_argument("-v", "--verbosity")
        .help("The higher verbosity - the more loggs")
        .default_value(DEFAULT_THREADS_CNT)
        .scan<'i', int>();
}

void VerbosityT::argparse_scan(argparse::ArgumentParser &program) {
    verbosity = program.get<int>("--verbosity");
}

} // namespace zephyr::tracer::config
