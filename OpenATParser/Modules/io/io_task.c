//
//  io_task.c
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "config.h"
#include "io_task.h"

static Bool doEcho = ENABLED;

void io_task_init(void)
{ 
    printf("Welcome to Open AT Parser!\n");
}

void io_task(void)
{
            
    
}

char getAsynchronousByte( void )  
{
    char c = FAIL;
    
    c = getchar();
    if(doEcho && c)
    {
      putchar(c);
    }
    
    return (c);
}

void enableEcho( void )
{
	doEcho = ENABLED;
}

void disableEcho( void )
{
	doEcho = DISABLED;
}