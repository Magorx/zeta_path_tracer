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

    FullT(const RenderT     &render_config,
          const VerbosityT  &verbos_config,
          const SystemT &sysinf_config);
    
    inline void update_from_command_line_args(int argc, char* argv[]) {
        using ::optarg;
        while (true) {
            switch(getopt(argc, argv, "k:t:v:R")) {
                case '?':
                case 'h':
                default:
                    printf("run ./pather -k #threads-count -t #render-task-file\n");
                    printf("use -t only if you understand what it does\n");
                    break;

                case -1:
                    break;

                case 'k':
                    sscanf(optarg, "%d", &sysinf.kernel_cnt);
                    continue;

                case 't':
                    sysinf.rtask_filename = strdup(optarg);
                    continue;

                case 'R':
                    sysinf.to_rewrire_rtask_file = true;
                    continue;
            }
            break;
        }
    }
};

} // namespace zephyr::tracer::config
