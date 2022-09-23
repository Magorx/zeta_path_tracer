#pragma once


namespace zephyr::tracer::config {

struct SystemT {
    int  timestamp;
    int  kernel_cnt;
    const char *rtask_filename;
    bool to_rewrire_rtask_file;

    SystemT(const int timestamp_, const int kernel_cnt_, const char *rtask_filename_, const bool to_rewrire_rtask_file_=false);
};

} // namespace zephyr::tracer::config
