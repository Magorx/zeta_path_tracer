#pragma once


namespace zephyr::tracer::config {

struct SystemT {
    long long timestamp;
    int  kernel_cnt;
    const char *rtask_filename;
};

} // namespace zephyr::tracer::config
