//
//  conf_scheduler.h
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef OpenATParser_conf_scheduler_h
#define OpenATParser_conf_scheduler_h

//! --- Scheduler Configuration --- 
// ***** Task init *****
#define Scheduler_task_1_init    io_task_init            // Init IO task
#define Scheduler_task_2_init    atc_task_init           // Init ATC task


// ***** Task definition *****
#define Scheduler_task_1        io_task                  // IO task
#define Scheduler_task_2		atc_task                 // ATC task


#endif
