#pragma once


namespace zephyr::tracer::config {

struct VerbosityT {
    int VERBOSITY;

    VerbosityT(const int verbosity);
};

} // namespace zephyr::tracer::config
