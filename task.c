/*
Squelette module RTAI
*/

#include "task.h"

static RT_TASK task1;
static RT_TASK task2;
static SEM sem1;
static SEM sem2;

RTIME module_time_ref; /* time @module init */

void t1(int arg){
  printk("t1-Alive time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
  rt_sem_signal(&sem1);
  rt_sem_wait(&sem2);
  printk("t1-Dead time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
  rt_sem_signal(&sem1);
}

void t2(int arg){
  rt_sem_wait(&sem1);
  printk("t2-Alive time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
  rt_sem_signal(&sem2);
  rt_sem_wait(&sem1);
  printk("t2-Dead time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
  rt_sem_signal(&sem2);
}

static int mon_init(void) {
  int ierr;
  RTIME now;

  rt_set_oneshot_mode();
  
  start_rt_timer(nano2count(TICK_PERIOD));
  module_time_ref=rt_get_time();
  
  /* declare tasks */
  ierr = rt_task_init_cpuid(&task1, t1, TASK_NODATA, STACK_SIZE, T1_PRIORITY, FPU_NOFPU, INT_NOHANDLER, CPU_ID);
  printk("[tache %d periode=%lluns] cree code retour %d par programme %s\n", 1, T1_PERIOD, ierr, __FILE__);
  ierr = rt_task_init_cpuid(&task2, t2, TASK_NODATA, STACK_SIZE, T2_PRIORITY, FPU_NOFPU, INT_NOHANDLER, CPU_ID);
  printk("[tache %d periode=%lluns] cree code retour %d par programme %s\n", 2, T2_PERIOD, ierr, __FILE__);

  /* sems init */
  rt_typed_sem_init(&sem1, SEM_INIT_WAIT, BIN_SEM);
  rt_typed_sem_init(&sem2, SEM_INIT_WAIT, BIN_SEM);

  if(!ierr){ /* if OK : run them */
    rt_task_make_periodic(&task1, module_time_ref+nano2count(TICK_PERIOD), nano2count(T1_PERIOD));
    rt_task_make_periodic(&task2, module_time_ref+nano2count(TICK_PERIOD), nano2count(T2_PERIOD));
  }
  return ierr;
}

void mon_exit(void) {
  stop_rt_timer();
  rt_sem_delete(&sem1);
  rt_sem_delete(&sem2);
  rt_task_delete(&task1);
  rt_task_delete(&task2);
}


module_init(mon_init);
module_exit(mon_exit);
