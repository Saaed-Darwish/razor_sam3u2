/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */
u32 G_aeSongs;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  static u32 u32MillisecondsPassed = 0;
  
  if ( WasButtonPressed(BUTTON0) ) 
  {
    ButtonAcknowledge(BUTTON0);
    
  }
  if ( WasButtonPressed(BUTTON1) ) 
  {
    ButtonAcknowledge(BUTTON1);
    
    if(G_bSetPassword == TRUE) 
    {
      G_aePasswordUserApp1[G_u8CurrentIndexUserApp1] = B1;
      G_u8CurrentIndexUserApp1++;
    }
    else
    {
      G_aeEnteredPassword[G_u8CurrentIndexUserApp1] = B1;
      G_u8CurrentIndexUserApp1++;
    }
  }
  if ( WasButtonPressed(BUTTON2) ) 
  {
    ButtonAcknowledge(BUTTON2);
    
    if(G_bSetPassword == TRUE) 
    {
      G_aePasswordUserApp1[G_u8CurrentIndexUserApp1] = B2;
      G_u8CurrentIndexUserApp1++;
    }
    else
    {
      G_aeEnteredPassword[G_u8CurrentIndexUserApp1] = B2;
      G_u8CurrentIndexUserApp1++;
    }
  }
  
  if( WasButtonPressed(BUTTON3) ) 
  {
    ButtonAcknowledge(BUTTON3);
    
    if(G_bSetPassword == TRUE)
    {
      G_bSetPassword = FALSE;
      G_u8CurrentIndexUserApp1 = 0;
      LedOff(YELLOW);
      LedOff(GREEN);
      return;
    }
    
    if(u32MillisecondsPassed < 3000) 
    {
      LedBlink(YELLOW, LED_2HZ);
      for(int i = 0; i < 10; i++)
      {
        G_aePasswordUserApp1[i] = none;
      }
      G_u8CurrentIndexUserApp1 = 0;
      G_bSetPassword = TRUE;
    }
      
    bool bPasswordCheck = TRUE;
       
    for(int i = 0; i < 10; i++)
    {
      if(G_aeEnteredPassword[i] != G_aePasswordUserApp1[i])
      {
        bPasswordCheck = FALSE;
      }
    }
    
    if(bPasswordCheck) 
    {
      LedBlink(GREEN, LED_2HZ);
      LedOff(RED);
      G_u8CurrentIndexUserApp1 = 0;
      for(int i = 0; i < 10; i++)
      {
        G_aeEnteredPassword[i] = none;
      }
    }
    else
    {
      LedBlink(RED, LED_2HZ);
      LedOff(GREEN);
      G_u8CurrentIndexUserApp1 = 0;
      for(int i = 0; i < 10; i++)
      {
        G_aeEnteredPassword[i] = none;
      }
    }
  }
  
  if(G_u8CurrentIndexUserApp1 == 10) 
  {
    LedBlink(RED, LED_2HZ);
    LedOff(GREEN);
    G_u8CurrentIndexUserApp1 = 0;
    for(int i = 0; i < 10; i++)
    {
      G_aeEnteredPassword[i] = none;
    }
  }
  
  u32MillisecondsPassed++;
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
