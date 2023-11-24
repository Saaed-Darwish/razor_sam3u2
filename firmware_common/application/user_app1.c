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
  u8 au8SongTitle[] = "Heart and Soul";
  
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, au8SongTitle);
  
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
    static u16 au16NotesRight[] =    {F5, F5, F5, F5, F5, E5, D5, E5, F5, G5, A5, A5, A5, A5, A5, G5, F5, G5, A5, A5S, C6, F5, F5, D6, C6, A5S, A5, G5, F5, NO, NO};
    static u16 au16DurationRight[] = {QN, QN, HN, EN, EN, EN, EN, EN, EN, QN, QN, QN, HN, EN, EN, EN, EN, EN, EN, QN,  HN, HN, EN, EN, EN, EN,  QN, QN, HN, HN, FN};
    static u16 au16NoteTypeRight[] = {RT, RT, HT, RT, RT, RT, RT, RT, RT, RT, RT, RT, HT, RT, RT, RT, RT, RT, RT, RT,  RT, HT, RT, RT, RT, RT,  RT, RT, RT, HT, HT};
    
    static u8 u8IndexRight = 0;
    static u32 u32RightTimer = 0;
    static u16 u16CurrentDurationRight = 0;
    static u16 u16NoteSilentDurationRight = 0;
    static bool bNoteActiveRight = TRUE;
    
    static u16 au16NotesLeft[] =    {F4, F4, A4, A4, D4, D4, F4, F4, A3S, A3S, D4, D4, C4, C4, E4, E4};
    static u16 au16DurationLeft[] = {EN, EN, EN, EN, EN, EN, EN, EN, EN,  EN,  EN, EN, EN, EN, EN, EN};
    static u16 au16NoteTypeLeft[] = {RT, RT, RT, RT, RT, RT, RT, RT, RT,  RT,  RT, RT, RT, RT, RT, RT};
    
    static u8 u8IndexLeft = 0;
    static u32 u32LeftTimer = 0;
    static u16 u16CurrentDurationLeft = 0;
    static u16 u16NoteSilentDurationLeft = 0;
    static bool bNoteActiveLeft = TRUE;
    
    u8 u8CurrentIndex;
    
    // RIGHT HAND
    if(IsTimeUp(&u32RightTimer, (u32)u16CurrentDurationRight))
    {
      u8 u8CurrentIndex = u8IndexRight;
      u32 u32CurrentTime = u32RightTimer;
      if(bNoteActiveRight)
      {
        if(au16NoteTypeRight[u8CurrentIndex] == RT)
        {
          u16CurrentDurationRight = au16DurationRight[u8CurrentIndex] - REGULAR_NOTE_ADJUSTMENT;
          u16NoteSilentDurationRight = REGULAR_NOTE_ADJUSTMENT;
          bNoteActiveRight = FALSE;
        } 
        else if (au16NoteTypeRight[u8CurrentIndex] == ST)
        {
          u16CurrentDurationRight = au16DurationRight[u8CurrentIndex] - STACCATO_NOTE_TIME;
          u16NoteSilentDurationRight = STACCATO_NOTE_TIME;
          bNoteActiveRight = FALSE;
        }
        else if (au16NoteTypeRight[u8CurrentIndex] == HT)
        {
          u16CurrentDurationRight = au16DurationRight[u8CurrentIndex] - REGULAR_NOTE_ADJUSTMENT;
          u16NoteSilentDurationRight = REGULAR_NOTE_ADJUSTMENT;
          bNoteActiveRight = FALSE;
          
          u8IndexRight++;
          if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16))
          {
            u8IndexRight = 0;
          }
        }
        
        if(au16NotesRight[u8CurrentIndex] != NO)
        {
          PWMAudioSetFrequency(BUZZER1, au16NotesRight[u8CurrentIndex]);
          PWMAudioOn(BUZZER1);
          
          switch(au16NotesRight[u8CurrentIndex]) 
          {
            case D5:
              LedOn(WHITE);
              break;
              
            case E5:
              LedOn(PURPLE);
              break;
              
            case F5:
              LedOn(BLUE);
              break;
              
            case G5:
              LedOn(CYAN);
              break;
            
            case A5:
              LedOn(GREEN);
              break;
            
            case A5S:
              LedOn(YELLOW);
              break;
            
            case C6:
              LedOn(ORANGE);
              break;
              
            case D6:
              LedOn(RED);
              break;
              
          }
        }
        else
        {
          PWMAudioOff(BUZZER1);
          
          LedOff(WHITE);
          LedOff(PURPLE);
          LedOff(BLUE);
          LedOff(CYAN);
          LedOff(GREEN);
          LedOff(YELLOW);
          LedOff(ORANGE);
          LedOff(RED);
        }
      }
      else
      {
        PWMAudioOff(BUZZER1);
        
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
          
        u32RightTimer = G_u32SystemTime1ms;
        u16CurrentDurationRight = u16NoteSilentDurationRight;
        
        bNoteActiveRight = TRUE;
        
        u8IndexRight++;
        if(u8IndexRight == sizeof(au16NotesRight) / sizeof(u16))
        {
          u8IndexRight = 0;
        }
      }
    }
    
    // LEFT HAND
    if(IsTimeUp(&u32LeftTimer, (u32)u16CurrentDurationLeft))
    {
      u8 u8CurrentIndex = u8IndexLeft;
      u32 u32CurrentTime = u32LeftTimer;
      if(bNoteActiveLeft)
      {
        if(au16NoteTypeLeft[u8CurrentIndex] == RT)
        {
          u16CurrentDurationLeft = au16DurationLeft[u8CurrentIndex] - REGULAR_NOTE_ADJUSTMENT;
          u16NoteSilentDurationLeft = REGULAR_NOTE_ADJUSTMENT;
          bNoteActiveLeft = FALSE;
        } 
        else if (au16NoteTypeLeft[u8CurrentIndex] == ST)
        {
          u16CurrentDurationLeft = au16DurationLeft[u8CurrentIndex] - STACCATO_NOTE_TIME;
          u16NoteSilentDurationLeft = STACCATO_NOTE_TIME;
          bNoteActiveLeft = FALSE;
        }
        else if (au16NoteTypeRight[u8CurrentIndex] == HT)
        {
          u16CurrentDurationLeft = au16DurationLeft[u8CurrentIndex] - REGULAR_NOTE_ADJUSTMENT;
          u16NoteSilentDurationLeft = REGULAR_NOTE_ADJUSTMENT;
          bNoteActiveLeft = FALSE;
          
          u8IndexLeft++;
          if(u8IndexLeft == sizeof(au16NotesLeft) / sizeof(u16))
          {
            u8IndexLeft = 0;
          }
        }
        
        if(au16NotesLeft[u8CurrentIndex] != NO)
        {
          PWMAudioSetFrequency(BUZZER2, au16NotesLeft[u8CurrentIndex]);
          PWMAudioOn(BUZZER2);
        }
        else
        {
          PWMAudioOff(BUZZER2);
        }
      }
      else
      {
        PWMAudioOff(BUZZER2);
        
        u32LeftTimer = G_u32SystemTime1ms;
        u16CurrentDurationLeft = u16NoteSilentDurationLeft;
        
        bNoteActiveLeft = TRUE;
        
        u8IndexLeft++;
        if(u8IndexLeft == sizeof(au16NotesLeft) / sizeof(u16))
        {
          u8IndexLeft = 0;
        }
      }
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
