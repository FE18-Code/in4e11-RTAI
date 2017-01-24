/*
Squelette module RTAI
*/
#include <linux/module.h>
MODULE_LICENSE("GPL");
#include <asm/io.h>

#include <asm/rtai.h>
#include <rtai_sched.h>

#define NUMERO      1
#define PRIORITE    1
#define STACK_SIZE  2000
#define PERIODE     1000000000    //  1 s
#define TICK_PERIOD 1000000    //  1 ms
#define N_BOUCLE    100
#define K_MAX 68000000
#define CPU_ID 0

#define T1_CAPACITY 1
#define T1_PERIOD 4
#define T2_CAPACITY 2
#define T2_PERIOD 6
#define T3_CAPACITY 3
#define T3_PERIOD 8 

static RT_TASK task1;
static RT_TASK task2;
static RT_TASK task3;

RTIME capacity_time_unit; /* time to run capacite() = unit */
RTIME module_time_ref; /* time @module init */

/* Prototypes */
void capacity_measure();
void capacity(int c);

void capacity_measure(){
  RTIME t=rt_get_time();
  capacity(1);
  capacity_time_unit = rt_get_time()-t;
  printk("Capacity first run : took %llu\n", count2nano(capacity_time_unit));
}

void capacity(int c){
  unsigned long k = K_MAX;
  while(c-->1){
    while(k-->1){
      nop();
    }
    k=K_MAX;
  }
}

void t1(int arg){
  static int boucle = N_BOUCLE;
  while(boucle--){
    printk("t1-Alive time=%llu\n", count2nano(rt_get_time()-module_time_ref));
    capacity(T1_CAPACITY); /* simulate any job */
    printk("t1-Dead time=%llu\n", count2nano(rt_get_time()-module_time_ref));
    rt_task_wait_period();
  }
}

static int mon_init(void) {
  int ierr;
  RTIME now;

  capacity_measure(); /* benchmark : time to run capacity() fct */

  rt_set_oneshot_mode();
  
  /* declare tasks */
  ierr = rt_task_init_cpuid(&task1, t1, NUMERO, STACK_SIZE, PRIORITE, 0, 0, CPU_ID);
  printk("[tache %d] cree code retour %d par programme %s\n", NUMERO, ierr, __FILE__);

  if(!ierr){ /* if OK : run them */
    start_rt_timer(nano2count(TICK_PERIOD));
    module_time_ref=rt_get_time();
    
    rt_task_make_periodic(&task1, rt_get_time(), nano2count(PERIODE)); 
  }
  return ierr;
}

void mon_exit(void) {
  stop_rt_timer();
  rt_task_delete(&task1);
}


module_init(mon_init);
module_exit(mon_exit);
