/***********************************************************************************************************************
File: main.h

Description:
Header file for main.c.
*******************************************************************************/

#ifndef __MAIN_H
#define __MAIN_H


/***********************************************************************************************************************
* Constant Definitions
***********************************************************************************************************************/
#define U32_DRINK_SERVE_TIME    (u32)3000   /* Loop iterations before drink is removed */
#define U8_MAX_DRINKS           (u8)3   /* Maxiumum number of drinks a server can hold */
#define U8_MAX_SERVERS          (u8)3       /* Maximum number of servers */

/***********************************************************************************************************************
* Type Definitions
***********************************************************************************************************************/
typedef enum {EMPTY, BEER, SHOOTER, WINE, HIBALL} DrinkType; // DrinkType Enum

typedef struct
{
  u8 u8ServerNumber;                            /* Server ID */
  DrinkType asServingTray[U8_MAX_DRINKS];       /* Data Payload Array */
  void* psNextServer;                           /* Pointer to Next ServerType */
} ServerType;

/***********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
bool InitializeServer(ServerType** psServer_);
bool CreateServer(ServerType** psServerList_);

      
    #endif /* __MAIN_H */