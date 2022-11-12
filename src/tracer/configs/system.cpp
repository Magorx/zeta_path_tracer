#include "system.h"

#include "settings.h"


namespace zephyr::tracer::config {

void SystemT::argparse_args(argparse::ArgumentParser &program) {
    program.add_optional_args_cut("\nConfig [system]:");

    program.add_argument("-t", "--threads")
        .help("Treads count for parallel execution")
        .default_value(DEFAULT_THREADS_CNT)
        .scan<'i', int>();
    
    program.add_argument("-c", "--complexity")
        .help("max task complexity to be used in jobber")
        .default_value(32*32)
        .scan<'i', int>();
    
    program.add_argument("--timestemp")
        .help("Unique number to mark some tmp files")
        .default_value(rand())
        .scan<'i', int>();
}

void SystemT::argparse_scan(argparse::ArgumentParser &program) {
    kernel_cnt = program.get<int>("--threads");
    timestamp  = program.get<int>("--timestemp");
    max_task_complexity = program.get<int>("--complexity");
}

} // namespace zephyr::tracer::config
