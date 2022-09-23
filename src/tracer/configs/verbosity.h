#pragma once


#include "extern/argparse.h"


namespace zephyr::tracer::config {

struct VerbosityT {
    int verbosity;

    void argparse_args(argparse::ArgumentParser &program);
    void argparse_scan(argparse::ArgumentParser &program);
};

} // namespace zephyr::tracer::config
