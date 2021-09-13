#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <cstdint>
#include <errno.h>
#include <semaphore.h>
#endif

#ifdef __APPLE__

class Semaphore {
    dispatch_semaphore_t sem;

public:
    inline Semaphore(uint32_t value) {
        *sem = dispatch_semaphore_create(value);
    }

    inline void wait() {
        dispatch_semaphore_wait(s->sem, DISPATCH_TIME_FOREVER);
    }
    
    inline void post() {
        dispatch_semaphore_signal(s->sem);
    }
};

#else

class Semaphore {
    sem_t sem;

public:
    inline Semaphore(uint32_t value) {
        sem_init(&sem, 0, value);
    }

    inline void wait() {
        int r;
        do {
            r = sem_wait(&sem);
        } while (r == -1 && errno == EINTR);
    }

    inline void post() {
        sem_post(&sem);
    }
};

#endif // APPLE or not

#endif //SEMAPHORES_H