/*
Squelette module RTAI
*/

#include "task.h"

static RT_TASK task1;
static RT_TASK task2;
static RT_TASK task3;

RTIME capacity_time_unit; /* time needed to run capacite() = unit */
RTIME module_time_ref; /* time @module init */

/* Prototypes */
void capacity_measure();
void capacity(int c);

void capacity_measure(){
  RTIME t=rt_get_time();
  capacity(1);
  capacity_time_unit = rt_get_time()-t;
  printk("Capacity first run : took %lluns (count=%llu)\n", count2nano(capacity_time_unit), capacity_time_unit);
}

void capacity(int c){
  unsigned long k = K_MAX;
  while(c-->=1){
    while(k-->1){
      nop();
    }
    k=K_MAX;
  }
}

void t1(int arg){
  static int boucle = N_BOUCLE;
  while(boucle--){
    printk("t1-Alive time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    capacity(T1_CAPACITY); /* simulate any job */
    printk("t1-Dead time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    rt_task_set_resume_end_times(-nano2count(T1_PERIOD*count2nano(capacity_time_unit)), -nano2count(T1_PERIOD*count2nano(capacity_time_unit)));
  }
}

void t2(int arg){
  static int boucle = N_BOUCLE;
  while(boucle--){
    printk("t2-Alive time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    capacity(T2_CAPACITY); /* simulate any job */
    printk("t2-Dead time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    rt_task_set_resume_end_times(-nano2count(T2_PERIOD*count2nano(capacity_time_unit)), -nano2count(T2_PERIOD*count2nano(capacity_time_unit)));
  }
}

void t3(int arg){
  static int boucle = N_BOUCLE;
  while(boucle--){
    printk("t3-Alive time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    capacity(T3_CAPACITY); /* simulate any job */
    printk("t3-Dead time=%lluns\n", count2nano(rt_get_time()-module_time_ref));
    rt_task_set_resume_end_times(-nano2count(T3_PERIOD*count2nano(capacity_time_unit)), -nano2count(T3_PERIOD*count2nano(capacity_time_unit)));
  }
}

static int mon_init(void) {
  int ierr;
  RTIME now;

  rt_set_oneshot_mode();
#ifdef HAS_PREEMPT_ALWAYS
  rt_preempt_always(PREEMPT);
#endif

  start_rt_timer(nano2count(TICK_PERIOD));
  capacity_measure(); /* benchmark : time to run capacity() fct */
  module_time_ref=rt_get_time();
  
  /* declare tasks */
  ierr = rt_task_init_cpuid(&task1, t1, TASK_NODATA, STACK_SIZE, T1_PRIORITY, FPU_NOFPU, INT_NOHANDLER, CPU_ID);
  printk("[tache %d periode=%lluns] cree code retour %d par programme %s\n", 1, T1_PERIOD*count2nano(capacity_time_unit), ierr, __FILE__);
  ierr = rt_task_init_cpuid(&task2, t2, TASK_NODATA, STACK_SIZE, T2_PRIORITY, FPU_NOFPU, INT_NOHANDLER, CPU_ID);
  printk("[tache %d periode=%lluns] cree code retour %d par programme %s\n", 2, T2_PERIOD*count2nano(capacity_time_unit), ierr, __FILE__);
  ierr = rt_task_init_cpuid(&task3, t3, TASK_NODATA, STACK_SIZE, T3_PRIORITY, FPU_NOFPU, INT_NOHANDLER, CPU_ID);
  printk("[tache %d periode=%lluns] cree code retour %d par programme %s\n", 3, T3_PERIOD*count2nano(capacity_time_unit), ierr, __FILE__);

  if(!ierr){ /* if OK : run them */
    rt_task_make_periodic(&task1, module_time_ref+nano2count(TICK_PERIOD), nano2count(T1_PERIOD*count2nano(capacity_time_unit)));
    rt_task_make_periodic(&task2, module_time_ref+nano2count(TICK_PERIOD), nano2count(T2_PERIOD*count2nano(capacity_time_unit)));
    rt_task_make_periodic(&task3, module_time_ref+nano2count(TICK_PERIOD), nano2count(T3_PERIOD*count2nano(capacity_time_unit)));  
  }
  return ierr;
}

void mon_exit(void) {
  stop_rt_timer();
  rt_task_delete(&task1);
  rt_task_delete(&task2);
  rt_task_delete(&task3);         
}


module_init(mon_init);
module_exit(mon_exit);
