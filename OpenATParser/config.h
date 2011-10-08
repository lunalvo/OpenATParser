//
//  config.h
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef OpenATParser_config_h
#define OpenATParser_config_h

//_____ I N C L U D E S ____________________________________________________
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//_____ D E F I N E S ______________________________________________________


#define Scheduler_time_init     init_soft_timers

#define SCHEDULER_TYPE          SCHEDULER_FREE  // SCHEDULER_(TIMED|TASK|FREE|CUSTOM)


//_____ D E C L A R A T I O N S ____________________________________________

typedef float               Float16;

typedef unsigned char       U8 ;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef char                S8 ;
typedef short               S16;
typedef int                 S32;
typedef unsigned char       Bool;

typedef U8                  Status;
typedef Bool                Status_bool;

#define PASS 0
#define FAIL 1

#define TRUE 1
#define FALSE 0

#define ENABLED 1
#define DISABLED 0

#endif
