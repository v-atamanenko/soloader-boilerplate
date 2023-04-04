/*
 * reimpl/pthr.h
 *
 * Wrapper for vitasdk/newlib pthread functions to work with
 * Android's pthread struct which is different
 *
 * Copyright (C) 2021      Andy Nguyen
 * Copyright (C) 2022      Rinnegatamante
 * Copyright (C) 2022      GrapheneCt
 * Copyright (C) 2022-2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#ifndef SOLOADER_PTHR_H
#define SOLOADER_PTHR_H

#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    pthread_attr_t * real_ptr; // replaces `uint32_t flags;`
    int32_t magic; // replaces `void * stack_base;`
    size_t stack_size;
    size_t guard_size;
    int32_t sched_policy;
    int32_t sched_priority;
} pthread_attr_t_bionic;

typedef struct
{
    pthread_mutex_t * real_ptr; // replaces `int volatile value;`
} pthread_mutex_t_bionic;

typedef struct
{
    pthread_cond_t * real_ptr; // replaces `int volatile value;`
} pthread_cond_t_bionic;

// pthread_t is same size on bionic and newlib
int pthread_create_soloader(pthread_t *thread, const pthread_attr_t_bionic *attr, void *(*start)(void *), void *param);
int pthread_kill_soloader(pthread_t thread, int sig);
int pthread_join_soloader(pthread_t thread, void **value_ptr);
int pthread_detach_soloader(pthread_t thread);
int pthread_equal_soloader(pthread_t t1, pthread_t t2);
pthread_t pthread_self_soloader();
int pthread_once_soloader(volatile int *once_control, void (*init_routine)(void));

// pthread_t and sched_param are same size on bionic and newlib
int pthread_getschedparam_soloader(pthread_t thread, int *policy, struct sched_param *param);
int pthread_setschedparam_soloader(pthread_t thread, int policy, const struct sched_param *param);

// condattr_t is same size on bionic and newlib
int pthread_condattr_init_soloader(pthread_condattr_t *attr);
int pthread_condattr_destroy_soloader(pthread_condattr_t *attr);

// mutexattr_t is same size on bionic and newlib
int pthread_mutexattr_init_soloader(pthread_mutexattr_t *attr);
int pthread_mutexattr_settype_soloader(pthread_mutexattr_t *attr, int type);
int pthread_mutexattr_destroy_soloader(pthread_mutexattr_t *attr);

int pthread_cond_init_soloader(pthread_cond_t_bionic *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy_soloader(pthread_cond_t_bionic *cond);
int pthread_cond_wait_soloader(pthread_cond_t_bionic *cond, pthread_mutex_t_bionic *mutex);
int pthread_cond_timedwait_soloader(pthread_cond_t_bionic *cond, pthread_mutex_t_bionic *mutex, struct timespec *abstime);
int pthread_cond_broadcast_soloader(pthread_cond_t_bionic *cond);
int pthread_cond_signal_soloader(pthread_cond_t_bionic *cond);

int pthread_mutex_init_soloader(pthread_mutex_t_bionic *uid, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy_soloader(pthread_mutex_t_bionic *mutex);
int pthread_mutex_lock_soloader(pthread_mutex_t_bionic *mutex);
int pthread_mutex_trylock_soloader(pthread_mutex_t_bionic *mutex);
int pthread_mutex_unlock_soloader(pthread_mutex_t_bionic *mutex);

int pthread_attr_init_soloader(pthread_attr_t_bionic *attr);
int pthread_attr_destroy_soloader(pthread_attr_t_bionic *attr);
int pthread_attr_setdetachstate_soloader(pthread_attr_t_bionic *attr, int state);
int pthread_attr_setstacksize_soloader(pthread_attr_t_bionic *attr, size_t stacksize);

int pthread_setname_np_soloader(pthread_t thread, const char* thread_name);

int sem_init_soloader (int * sem, int pshared, unsigned int value);
int sem_destroy_soloader(int * sem);
int sem_getvalue_soloader (int * sem, int * sval);
int sem_post_soloader (int * sem);
int sem_timedwait_soloader (int * sem, const struct timespec * abstime);
int sem_trywait_soloader (int * sem);
int sem_wait_soloader (int * sem);

#endif // SOLOADER_PTHR_H
