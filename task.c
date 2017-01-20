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
#define N_BOUCLE    10

static RT_TASK ma_tache;

void mon_code(int arg) {

  RTIME t=rt_get_time();
  static int boucle = N_BOUCLE ;

  while (boucle--){
    rt_task_wait_period();
    printk("Hello world, time=%llu\n", count2nano(rt_get_time()-t));
  }
}

static int mon_init(void) {

  int ierr;
  RTIME now;

  rt_set_oneshot_mode();

  ierr = rt_task_init(&ma_tache, mon_code, NUMERO, 
		      STACK_SIZE, PRIORITE, 0, 0);
  printk("[tache %d] cree code retour %d par programme %s\n",
	 NUMERO,ierr,__FILE__);

  if (!ierr) {
    
    start_rt_timer(nano2count(TICK_PERIOD));

	rt_task_make_periodic(&ma_tache, rt_get_time(), nano2count(PERIODE)); 

  }
  return ierr;
}

void mon_exit(void) {

  stop_rt_timer();
  
  rt_task_delete(&ma_tache);

}


module_init(mon_init);
module_exit(mon_exit);
