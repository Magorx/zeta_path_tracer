#pragma once


#include "render.h"
#include "system.h"
#include "verbosity.h"

#include <cstdio>
#include <cstring>
#include <unistd.h>

namespace zephyr::tracer::config {

struct FullT {
    RenderT     render;
    VerbosityT  verbos;
    SystemT     sysinf;
    
    inline void argparse_args(argparse::ArgumentParser &program) {
        render.argparse_args(program);
    }

    inline void argparse_scan(argparse::ArgumentParser &program) {
        render.argparse_scan(program);
    }
};

} // namespace zephyr::tracer::config
