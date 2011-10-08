//
//  io_task.h
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef OpenATParser_io_task_h
#define OpenATParser_io_task_h

void io_task_init     (void);
void io_task          (void);

char getAsynchronousByte (void);

void enableEcho(void);
void disableEcho(void);


#endif
