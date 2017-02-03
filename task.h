/* task.h - Main header */

#ifndef TASK_H
#define TASK_H

#include <linux/module.h>
MODULE_LICENSE("GPL");
#include <asm/io.h>

#include <asm/rtai.h>
#include <rtai_sched.h>
#include <rtai_sem.h>

#include "taskset_sem.h"

/* tasks params data, prio, signal */
#define TASK_NODATA (long int)0
#define INT_NOHANDLER 0

/* task_init default stack size */
#define STACK_SIZE 2000

/* task_init default cpu */
#define CPU_ID 0

/* task_init fpu option */
#define FPU_NOFPU 0
#define FPU_USEFPU 1

/* sems init values */
#define SEM_INIT_WAIT 0
#define SEM_INIT_PASS 1

#define PERIODE     1000000000    //  1 s
#define TICK_PERIOD 1000000    //  1 ms
#define N_BOUCLE 10
#define K_MAX 140000000

#endif

