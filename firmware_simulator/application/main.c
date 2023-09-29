/***********************************************************************************************************************
File: main.c                                                                

Description:
Container for the Cprimer "Server" application firmware.  
***********************************************************************************************************************/

#include "configuration.h"

#ifndef SOLUTION
/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "Main_" and be declared as static.
***********************************************************************************************************************/
static u8 Main_u8Servers = 0;   /* Number of people slinging drinks */

/***********************************************************************************************************************
Main Program
***********************************************************************************************************************/

void main(void)
{
  u32 u32UselessVariableForExample;
  u32 au32BigArray[] = {5, 4, 3, 2, 1};
  DrinkType aeDrinkArray[3] = {BEER, SHOOTER};
  
  u8 aString[] = "This is really an array of u8 with a NULL at the end.";
  
  /* Pointer Example */
  u8 u8Test = 0xA5;
  u8* pu8Example;
  u32 u32Test = 0x0000ffff;
  u32* pu32Example;
  
  /* Load Adresses into Pointers */
  pu8Example = &u8Test;
  pu32Example = &u32Test;
  
  /* Access Data via Pointers */
  *pu8Example++;
  (*pu32Example)++;
  
  pu8Example++;
  *pu32Example++;
  
  /* Struct Example */
  u8 u8CurrentServer;
  ServerType sServer1;
  ServerType* psServerParser;
  
  psServerParser = &sServer1;
  sServer1.u8ServerNumber = 18;
  u8CurrentServer = psServerParser->u8ServerNumber;
  
  while(1)
  {
    
  } /* end while(1) main loop */
  
} /* end main() */


/***********************************************************************************************************************
* Function definitions
***********************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------------------
Function InitializeServer

Description:
Initializes a new server. A new server has an empty tray of drinks and is
assigned the next number availiable.

Requires:
  - psServer_ points to the server list where a new server is to be initialized;
    since the server list is a pointer to a linked list, this is a pointer-to-pointer
  - Main_u8Servers holds the current number of active servers

Promises:
  - Returns TRUE if the server is initialized
  - Returns False is the server cannot be initialized
*/

bool InitializeServer(ServerType** psServer_)
{
  if(*psServer_ == NULL)
  {
    return FALSE;
  }
  
  (*psServer_)->u8ServerNumber = Main_u8Servers;
  
  /* empty tray */
  for(u8 i = 0; i < U8_MAX_DRINKS; i++) 
  {
    (*psServer_)->asServingTray[i] = EMPTY;
  }
  
  /* try to create server object */
  psNewServer = malloc(sizeof(ServerType));
  
  /* memory allocated? */
  if(psNewServer == NULL)
  {
    return FALSE;
  }
  
  return TRUE;
  
} /* ~InitializeServer */

/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* SOLUTION */