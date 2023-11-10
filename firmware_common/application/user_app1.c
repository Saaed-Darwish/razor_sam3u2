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
  /* initialize LEDs to off */
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(WHITE);
  LedOff(RED);
  
  LedPWM(CYAN, LED_PWM_5);
  LedPWM(GREEN, LED_PWM_5);
  LedPWM(YELLOW, LED_PWM_5);
  LedPWM(ORANGE, LED_PWM_5);
  LedPWM(BLUE, LED_PWM_5);
  LedPWM(PURPLE, LED_PWM_5);
  LedPWM(WHITE, LED_PWM_5);
  LedPWM(RED, LED_PWM_5);
  /* backlight to white */
  LedOn(LCD_RED);
  LedOn(LCD_BLUE);
  LedOn(LCD_GREEN);
  
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
  
    static u16 u16PurpleBlinkCounter = 0;
    static u8 u8Counter = 0;
    static u8 u8ColorIndex = 0;
    
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(BLUE);
    LedOff(PURPLE);
    LedOff(WHITE);
    LedOff(RED);
    
    u16PurpleBlinkCounter++;
    if(u16PurpleBlinkCounter == 250) {
      u8Counter++;
      if(u8Counter == 16) {
        u8Counter = 0;
      }
      /*

      if(u8Counter & 0x01) {
        LedOn(RED);
      } else {
        LedOff(RED);
      }
      
      if(u8Counter & 0x02) {
        LedOn(ORANGE);
      } else {
        LedOff(ORANGE);
      }
      
      if(u8Counter & 0x04) {
        LedOn(YELLOW);
      } else {
        LedOff(YELLOW);
      }
      
      if(u8Counter & 0x08) {
        LedOn(GREEN);
      } else {
        LedOff(GREEN);
      }
      */
      
      u8ColorIndex++;
      if(u8ColorIndex == 16) {
        u8ColorIndex = 0;
      }
      
      switch (u8ColorIndex)
      {
          case 0:         // pure red
              LedOff(LCD_BLUE);
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedPWM(LCD_RED, LED_PWM_100);
              break;

          case 1:         // red with a hint of blue
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_95);
              LedPWM(LCD_BLUE, LED_PWM_5);
              break;

          case 2:         // deeper red-magenta
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_90);
              LedPWM(LCD_BLUE, LED_PWM_10);
              break;

          case 3:         // red-magenta
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_85);
              LedPWM(LCD_BLUE, LED_PWM_15);
              break;

          case 4:         // magenta with dominant red
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_80);
              LedPWM(LCD_BLUE, LED_PWM_20);
              break;

          case 5:         // magenta
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_75);
              LedPWM(LCD_BLUE, LED_PWM_25);
              break;

          case 6:         // magenta with dominant blue
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_70);
              LedPWM(LCD_BLUE, LED_PWM_30);
              break;

          case 7:         // purple
              LedOff(LCD_GREEN);
              LedOn(LCD_BLUE);
              LedOn(LCD_RED);
              LedPWM(LCD_RED, LED_PWM_60);
              LedPWM(LCD_BLUE, LED_PWM_40);
              break;

          case 8:         // deeper purple
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_50);
              LedPWM(LCD_BLUE, LED_PWM_50);
              break;

          case 9:         // purple-pink
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_60);
              LedPWM(LCD_BLUE, LED_PWM_40);
              break;

          case 10:        // pinkish-purple
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_70);
              LedPWM(LCD_BLUE, LED_PWM_30);
              break;

          case 11:        // pink
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_80);
              LedPWM(LCD_BLUE, LED_PWM_20);
              break;

          case 12:        // deeper pink
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_85);
              LedPWM(LCD_BLUE, LED_PWM_15);
              break;

          case 13:        // pink-red
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedOn(LCD_BLUE);
              LedPWM(LCD_RED, LED_PWM_90);
              LedPWM(LCD_BLUE, LED_PWM_10);
              break;

          case 14:        // near pure red
              LedOff(LCD_BLUE);
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedPWM(LCD_RED, LED_PWM_95);
              break;

          case 15:        // back to red
              LedOff(LCD_BLUE);
              LedOff(LCD_GREEN);
              LedOn(LCD_RED);
              LedPWM(LCD_RED, LED_PWM_100);
              break;
      }
      
      LedToggle(PURPLE);
      u16PurpleBlinkCounter = 0;
    }
    
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
