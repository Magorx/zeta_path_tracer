#pragma once


#include "extern/argparse.h"


namespace zephyr::tracer::config {

struct SystemT {
    long long timestamp;
    int  kernel_cnt;

    void argparse_args(argparse::ArgumentParser &program);
    void argparse_scan(argparse::ArgumentParser &program);
};

} // namespace zephyr::tracer::config
