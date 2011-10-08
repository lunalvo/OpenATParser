//
//  scheduler.c
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "config.h"                         
#include "scheduler.h"                      

static Bool scheduler_flag = ENABLED;

//Init
void scheduler_init (void)
{
#ifdef Scheduler_task_1_init
    Scheduler_task_1_init();  
#endif
#ifdef Scheduler_task_2_init
    Scheduler_task_2_init();  
#endif
#ifdef Scheduler_task_3_init
    Scheduler_task_3_init();  
#endif
}

//Scheduler loop
void scheduler_tasks (void)
{

    while (scheduler_flag)
    {
#ifdef Scheduler_task_1
        Scheduler_task_1();
#endif
#ifdef Scheduler_task_2
        Scheduler_task_2();
#endif
#ifdef Scheduler_task_3
        Scheduler_task_3();
#endif
    }
}

//Will run the scheduler
void scheduler (void)
{
    scheduler_init();
    // Loop exec
    scheduler_tasks();
}

void enableScheduler( void )
{
	scheduler_flag = ENABLED;
}

void disableScheduler( void )
{
	scheduler_flag = DISABLED;
}
