//
//  atc_task.c
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "io_task.h"
#include "atc_task.h"
#include "scheduler.h"

//_____ D E C L A R A T I O N S ____________________________________________
void clear_atc_buffer(void);
void atc_parser(CommandLine_t  *commandBuffer_p);
void printResultCode(ResultCode_t res);
ResultCode_t parseCommandBuffer (CommandLine_t *commandBuffer_p);
ResultCode_t checkCommandBuffer (unsigned char *at_cmd_string);
ExtendedOperation_t getExtendedOperation (CommandLine_t *commandBuffer_p);
U16 getDecimalValue (CommandLine_t *commandBuffer_p);

//At handlres functions
ResultCode_t commandSWVersion(CommandLine_t *commandBuffer_p);
ResultCode_t commandTest(CommandLine_t *commandBuffer_p);
ResultCode_t commandQuit(CommandLine_t *commandBuffer_p);
ResultCode_t commandAtHelp(CommandLine_t *commandBuffer_p);

//_____ G L O B A L V A R S ____________________________________________
static unsigned char atc_buffer[ATC_BUFFER_SIZE];

static const AtCommand_t Command[] =
{
    /* Command-                     */
    /* String     CommandFunc       */
    /* Command List Max Size = 255   (U8)   */
    /* ======     ===========       */
    {"AT%SWV", commandSWVersion},
    {"AT%TEST", commandTest},
    {"AT%QUIT", commandQuit},
    {"AT%HELP", commandAtHelp},
    {NULL,       NULL}
};

//_____ F U N C T I O N S  ____________________________________________


void atc_task_init(void)
{
    clear_atc_buffer();  
}

static CommandLine_t commandBuffer_p_static = {0};


void atc_task(void)
{
	//Local variables
	char lastByte = 0;
	static U8 pos = 0;
	
	//Fetch byte from serial and handle at functions
	if((lastByte = getAsynchronousByte()) != FAIL)
	{
		if(pos > (ATC_BUFFER_SIZE-1))
			pos = 0;
		
		if((lastByte == CHAR_LF)||(lastByte == CHAR_CR))
		{
			atc_buffer[pos] = (U8) 0;
			
			printf("\r\n");
            
			commandBuffer_p_static.character = atc_buffer;
			commandBuffer_p_static.length	  = pos;
			commandBuffer_p_static.position  = 0;
			
			atc_parser(&commandBuffer_p_static);
            
			clear_atc_buffer();
			pos = 0;
		}
		else if(lastByte == CHAR_BS)
		{
			if(pos > 0)
				pos--;
		}
		else
		{
			atc_buffer[pos++] = lastByte;
		}
	}
    
}

//Clear atc_buffer using zeros
void clear_atc_buffer(void)
{
	memset(atc_buffer,'\0',ATC_BUFFER_SIZE);
}

void atc_parser(CommandLine_t *commandBuffer_p)
{
	ResultCode_t   res = INVALID_RESULT_CODE;
	
	if( !strncasecmp( (char *) commandBuffer_p->character, "AT%", 3 ) ) 
	{
		res = checkCommandBuffer(commandBuffer_p->character);
		if (res == RESULT_CODE_OK)
		{
			res = parseCommandBuffer(commandBuffer_p);
		}
	}
	else if( !strcasecmp( (char *) commandBuffer_p->character, "AT") )
	{
		res = RESULT_CODE_OK;
	}
	else if( !strcasecmp( (char *) commandBuffer_p->character, "ATE1") )
	{
		enableEcho();
		res = RESULT_CODE_OK;
	}
	else if( !strcasecmp( (char *) commandBuffer_p->character, "ATE0") )
	{
		disableEcho();
		res = RESULT_CODE_OK;
	}
	
	printResultCode(res);
}

void printResultCode(ResultCode_t res)
{
	switch (res)
    {
        case INVALID_RESULT_CODE:
        	printf("\r\n%cInvalid command%c\r\n",CHAR_STX,CHAR_ETX);
            break;
        case RESULT_CODE_OK:
        	printf("\r\n%cOK%c\r\n",CHAR_STX,CHAR_ETX);
            break;
        case RESULT_CODE_ERROR:
        default:
			printf("\r\n%cERROR%c\r\n",CHAR_STX,CHAR_ETX);
            break;
            
    }
}

ResultCode_t parseCommandBuffer (CommandLine_t *commandBuffer_p)
{
	U8 index,pos;
	ResultCode_t result = INVALID_RESULT_CODE;
	//char* commandString;
    
	index = 0;
	while(Command[index].commandString != NULL)
	{
		//commandString = Command[index].commandString;
		pos = strlen(Command[index].commandString);
		
		//if ( !strnicmp( (char *)commandBuffer_p->character, commandString, strlen(commandString) ) )
		if ( !strncasecmp( (char *)commandBuffer_p->character, Command[index].commandString, pos ) )
		{
			commandBuffer_p->position = pos;
            
			result = Command[index].commandFunc(commandBuffer_p);
			
			break;
		}
        
		index++;
	}
    
	return result;
}

ResultCode_t checkCommandBuffer (unsigned char *at_cmd_string)
{
	U8 index;
	//char* commandString;
    
	index = 0;
	while(Command[index].commandString != NULL)
	{
		//commandString = Command[index].commandString;
		//if ( !strnicmp( (char *)at_cmd_string, commandString, strlen(commandString) ) )
		if ( !strncasecmp( (char *)at_cmd_string, Command[index].commandString, strlen(Command[index].commandString) ) )
		{
			return RESULT_CODE_OK;
		}
        
		index++;
	}
    
	return INVALID_RESULT_CODE;
}

ExtendedOperation_t getExtendedOperation (CommandLine_t *commandBuffer_p)
{
    ExtendedOperation_t result;
    
    if (commandBuffer_p->position < commandBuffer_p->length)
    {
        switch (commandBuffer_p->character[commandBuffer_p->position])
        {
            case QUERY_CHAR:  /* AT%...? */
                commandBuffer_p->position++;
                result = EXTENDED_QUERY;
                break;
                
            case EQUALS_CHAR:  /* AT%...= */
                commandBuffer_p->position++;
                if ((commandBuffer_p->position < commandBuffer_p->length) &&
                    (commandBuffer_p->character[commandBuffer_p->position] == QUERY_CHAR))
                {
                    commandBuffer_p->position++;
                    result = EXTENDED_RANGE;
                }
                else
                {
                    result = EXTENDED_ASSIGN;
                }
                break;
                
            default:  /* AT%... */
                result = EXTENDED_ACTION;
                break;
        }
    }
    else
    {
        result = EXTENDED_ACTION;
    }
    
    return result;
}

U16 getDecimalValue(CommandLine_t *commandBuffer_p)
{
    U16   value = 0;
    
    while ((commandBuffer_p->position < commandBuffer_p->length) &&
           (isdigit(commandBuffer_p->character[commandBuffer_p->position])))
    {
        value *= 10;
        value += (commandBuffer_p->character[commandBuffer_p->position] - '0');
        commandBuffer_p->position++;
    }
    return (value);
}


ResultCode_t commandSWVersion(CommandLine_t *commandBuffer_p)
{
	ResultCode_t result = RESULT_CODE_OK;
	ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);  
	
	switch (operation)
	{
		case EXTENDED_QUERY:       /* AT%SWV? */
		case EXTENDED_RANGE:       /* AT%SWV=? */
		case EXTENDED_ASSIGN:      /* AT%SWV= */
		case EXTENDED_ACTION:      /* AT%SWV */
			printf("\r\n%cOpen AT Parser 1.0, %s, %s %c\r\n",CHAR_STX,__DATE__,__TIME__,CHAR_ETX);
			result = RESULT_CODE_OK;
			break;
		default:
			result = RESULT_CODE_ERROR;
			break;
	}
	
	return result;
}

ResultCode_t commandTest(CommandLine_t *commandBuffer_p)
{
	ResultCode_t result = RESULT_CODE_OK;
	ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);  
	U16 input = 0;
    
	switch (operation)
	{
		case EXTENDED_QUERY:       /* AT%TEST? */
		case EXTENDED_RANGE:       /* AT%TEST=? */
		case EXTENDED_ACTION:      /* AT%TEST */
            printf("\r\n%cEverything running good. Try to use assign command (at%%test=2)%c\r\n",CHAR_STX,CHAR_ETX);
			result = RESULT_CODE_OK;
			break;
        case EXTENDED_ASSIGN:      /* AT%TEST= */
            input = getDecimalValue(commandBuffer_p);
            
            if(input > 32767)
            {
                result = RESULT_CODE_ERROR;
            }
            else
            {
                printf("\r\n%c2 times your number is: %d%c\r\n",CHAR_STX,2*input,CHAR_ETX);
                result = RESULT_CODE_OK;
			}
            break;
		default:
			result = RESULT_CODE_ERROR;
			break;
	}
	
	return result;
}

ResultCode_t commandQuit(CommandLine_t *commandBuffer_p)
{
	ResultCode_t result = RESULT_CODE_OK;
	ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);  
	
	switch (operation)
	{
		case EXTENDED_QUERY:       /* AT%QUIT? */
		case EXTENDED_RANGE:       /* AT%QUIT=? */
		case EXTENDED_ASSIGN:      /* AT%QUIT= */
		case EXTENDED_ACTION:      /* AT%QUIT */
			disableScheduler();
            printf("\r\n%cByeˆˆ%c\r\n",CHAR_STX,CHAR_ETX);
			result = RESULT_CODE_OK;
			break;
		default:
			result = RESULT_CODE_ERROR;
			break;
	}
	
	return result;
}

ResultCode_t commandAtHelp(CommandLine_t *commandBuffer_p) {
	ResultCode_t result = RESULT_CODE_OK;
	ExtendedOperation_t  operation = getExtendedOperation (commandBuffer_p);  
    
	U8 index;
	char* commandString;
	
	switch (operation)
	{
		case EXTENDED_QUERY:       /* AT%HELP? */
		case EXTENDED_RANGE:       /* AT%HELP=? */
		case EXTENDED_ASSIGN:      /* AT%HELP= */
		case EXTENDED_ACTION:      /* AT%HELP */
			for (index = 0; Command[index].commandString != NULL; index++)
			{
				commandString = Command[index].commandString;
				printf("%s\r\n",commandString);
			}
			result = RESULT_CODE_OK;
			break;
		default:
			result = RESULT_CODE_ERROR;
			break;
	}
	
	return result;
}