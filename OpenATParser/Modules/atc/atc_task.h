//
//  Header.h
//  OpenATParser
//
//  Created by Francisco Ferreira on 08/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef OpenATParser_Header_h
#define OpenATParser_Header_h

#define SEMICOLON_CHAR		';'
#define COMMA_CHAR			','
#define QUOTES_CHAR			'\"'
#define QUERY_CHAR			'?'
#define EQUALS_CHAR			'='
#define CHAR_STX			0x02
#define CHAR_ETX			0x03
#define CHAR_BS  			0x08
#define CHAR_CR				0x0D
#define CHAR_LF				0x0A

#define ATC_BUFFER_SIZE 64

typedef enum ResultCodeTag
{
    INVALID_RESULT_CODE        = -1,
    RESULT_CODE_OK             =  0,
    RESULT_CODE_ERROR          =  1,
    NUM_OF_RESULT_CODES
} ResultCode_t;


typedef enum ExtendedOperationTag
{
    INVALID_EXTENDED_OPERATION,
    EXTENDED_ACTION,
    EXTENDED_QUERY,
    EXTENDED_ASSIGN,
    EXTENDED_RANGE,
    NUM_OF_EXTENDED_OPERATIONS
} ExtendedOperation_t;


typedef struct CommandLineTag
{
    U8 tid;
    U8 position;
    U8 length;
    unsigned char  *character;
} CommandLine_t;


typedef struct AtCommandTag
{
    char *commandString;
    ResultCode_t (*commandFunc) (CommandLine_t *commandBuffer_p);
} AtCommand_t;


void atc_task_init(void);
void atc_task(void);

#endif
