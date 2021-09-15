#ifndef SEMAPHORE_H
#define SEMAPHORE_H

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
    inline Semaphore(uint32_t value = 0) {
        sem = dispatch_semaphore_create(value);
    }

    inline void wait() {
        dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
    }
    
    inline void post() {
        dispatch_semaphore_signal(sem);
    }
};

#else // todo: ELIF LINUX

class Semaphore
{
    sem_t sem;

    Semaphore(const Semaphore& other) = delete;
    Semaphore& operator=(const Semaphore& other) = delete;

public:
    inline Semaphore(uint32_t value = 0)
    {
        sem_init(&sem, 0, value);
    }

    ~Semaphore()
    {
        sem_destroy(&sem);
    }

    inline void wait()
    {
        int rc = -1;
        do
        {
            rc = sem_wait(&sem);
        }
        while (rc == -1 && errno == EINTR);
    }

    inline void post() {
        sem_post(&sem);
    }
};

#endif // NOT APPLE

// ERROR HERE, NOT DEFINED

#endif // SEMAPHORE_H
