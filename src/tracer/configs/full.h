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
        sysinf.argparse_args(program);
        verbos.argparse_args(program);
    }

    inline void argparse_scan(argparse::ArgumentParser &program) {
        render.argparse_scan(program);
        sysinf.argparse_scan(program);
        verbos.argparse_scan(program);
    }
};

FullT from_command_line(int argc, char* argv[]);

} // namespace zephyr::tracer::config
