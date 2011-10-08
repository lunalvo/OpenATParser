//
//  scheduler.h
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef OpenATParser_scheduler_h
#define OpenATParser_scheduler_h

//!_____ I N C L U D E S ____________________________________________________
#include "conf_scheduler.h"            // Configuration for the scheduler

//!_____ M A C R O S ________________________________________________________
//! Definition of Task ID. 
#define TASK_DUMMY   0x00
#define TASK_0       0x01
#define TASK_1       0x02
#define TASK_2       0x04

#define ALL_TASK     (TASK_0|TASK_1|TASK_2)
//! End Task ID 


#ifdef Scheduler_task_1_init
extern  void Scheduler_task_1_init (void);
#else
// if you do not want init at all, do:
// #define Scheduler_task_1_init scheduler_empty_fct
#error Scheduler_task_1_init must be defined in config.h file
#endif
#ifdef Scheduler_task_2_init
extern  void Scheduler_task_2_init (void);
#endif
#ifdef Scheduler_task_3_init
extern  void Scheduler_task_3_init (void);
#endif

#ifdef Scheduler_task_1
extern  void Scheduler_task_1 (void);
#else
// if you do not want task at all, do:
// #define Scheduler_task_1 scheduler_empty_fct
#error Scheduler_task_1 must be defined in config.h file
#endif
#ifdef Scheduler_task_2
extern  void Scheduler_task_2 (void);
#endif
#ifdef Scheduler_task_3
extern  void Scheduler_task_3 (void);
#endif


//!_____ D E C L A R A T I O N ______________________________________________
void scheduler_init         (void);
void scheduler_tasks        (void);
void scheduler              (void);

void enableScheduler(void);
void disableScheduler(void);

#endif
