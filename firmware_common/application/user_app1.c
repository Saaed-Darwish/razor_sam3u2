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
- void shiftLeft(char* line)

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
volatile u32 G_u32UserApp1Flags; /*!< @brief Global state flags */

u8 G_aau8Songs[10][50] = {"Sunrise - chatGPT", "none", "none", "none", "none", "none", "none", "none", "none", "none"};
u32 G_u32CurrentIndexUserApp1 = 0;
bool G_bPlaySong = FALSE;

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;    /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;     /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;      /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags; /*!< @brief From main.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine; /*!< @brief The state machine function pointer */
// static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */

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
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, G_aau8Songs[G_u32CurrentIndexUserApp1]);
  LcdMessage(LINE2_START_ADDR, "      <-    -> start");
  /* If good initialization, set state to Idle */
  if (1)
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
  u8* songDisplay = G_aau8Songs[G_u32CurrentIndexUserApp1];
  u8* controlsDisplay = "      <-    -> start";
  
  static u32 u32MillisecondsPassed = 0;
  static u8 u8IndexLeft = 0;
  static u8 u8IndexRight = 0;
  
  static u16 au16NotesRightEL[] = {C5, E5, G5, A5, G5, E5, C5, D5, E5, F5, E5, D5, C5, C5, D5, E5, F5, G5, A5, G5, F5, E5, D5, C5};
  static u16 au16DurationRightEL[] = {EN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, QN};
  static u16 au16NoteTypeRightEL[] = {RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT};

  static u16 au16NotesLeftEL[] = {C4, G3, C4, F3, A3, F3, C4, G3};
  static u16 au16DurationLeftEL[] = {HN, HN, HN, HN, HN, HN, HN, HN};
  static u16 au16NoteTypeLeftEL[] = {RT, RT, RT, RT, RT, RT, RT, RT};

  static u32 u32RightTimer = 0;
  static u16 u16CurrentDurationRight = 0;
  static u16 u16NoteSilentDurationRight = 0;
  static bool bNoteActiveRight = TRUE;

  static u32 u32LeftTimer = 0;
  static u16 u16CurrentDurationLeft = 0;
  static u16 u16NoteSilentDurationLeft = 0;
  static bool bNoteActiveLeft = TRUE;

  u8 u8CurrentIndex;
  
  static u16 au16EmptyNotes[] = {NO};
  static u16 au16EmptyDuration[] = {FN};
  static u16 au16EmptyNoteType[] = {RT};

  u16 *au16NotesLeft;
  u16 *au16DurationLeft;
  u16 *au16NoteTypeLeft;

  u16 *au16NotesRight;
  u16 *au16DurationRight;
  u16 *au16NoteTypeRight;
  
  u16 u16SizeNotesRight = 0;
  u16 u16SizeNotesLeft = 0;
  
  static u16 u16NoteMoveInterval;
  static u32 u32CurrentInterval;
  
  u8 *au8line1;
  u8 *au8line2;

  if(G_u32CurrentIndexUserApp1 == 0) {
    au16NotesLeft = au16NotesLeftEL;
    au16DurationLeft = au16DurationLeftEL;
    au16NoteTypeLeft = au16NoteTypeLeftEL;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftEL);
    
    au16NotesRight = au16NotesRightEL;
    au16DurationRight = au16DurationRightEL;
    au16NoteTypeRight = au16NoteTypeRightEL;
    
    u16SizeNotesRight = sizeof(au16NotesRightEL);
  } else {
    au16NotesLeft = au16EmptyNotes;
    au16DurationLeft = au16EmptyDuration;
    au16NoteTypeLeft = au16EmptyNoteType;
    
    u16SizeNotesLeft = sizeof(au16EmptyNotes);
    
    au16NotesRight = au16EmptyNotes;
    au16DurationRight = au16EmptyDuration;
    au16NoteTypeRight = au16EmptyNoteType;
    
    u16SizeNotesRight = sizeof(au16EmptyNotes);
  }

  static bool bResetDone = FALSE;

  if (G_bPlaySong)
  {
    static int len;
    if (!bResetDone)
    {
      u8IndexLeft = 0;
      u8IndexRight = 0;
      u8CurrentIndex = 0;
      u32RightTimer = G_u32SystemTime1ms;
      u32LeftTimer = G_u32SystemTime1ms;
      u16CurrentDurationRight = 0;
      u16CurrentDurationLeft = 0;
      u16NoteSilentDurationRight = 0;
      u16NoteSilentDurationLeft = 0;
      bNoteActiveRight = FALSE;
      bNoteActiveLeft = FALSE;
      PWMAudioOff(BUZZER1);
      PWMAudioOff(BUZZER2);
      u32MillisecondsPassed = 0;
      
      // initialize gameplay string
      if(G_u32CurrentIndexUserApp1 == 0) {
        u8 line1[] = "               *  *   *     *        *  *   *       *      ";
        u8 line2[] = "                             *      * *               *    ";
        au8line1 = line1;
        au8line2 = line2;
        len = 59;
        u16NoteMoveInterval= 350;
        u32CurrentInterval= 0;
      } else {
        au8line1 = "                    ";
        au8line2 = "                    ";
        len = 20;
      }
      
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, au8line1);
      LcdMessage(LINE2_START_ADDR, au8line2);
    
      bResetDone = TRUE;
    }
    
    // gameplay goes here
    LedOn(BLUE);
    if(u32CurrentInterval+ u16NoteMoveInterval< u32MillisecondsPassed) {
      for (int i = 1; i < len; i++) {
          au8line1[i-1] = au8line1[i];
      }
      au8line1[len-1] = ' ';
      
      for (int i = 1; i < len; i++) {
          au8line2[i-1] = au8line2[i];
      }
      au8line2[len-1] = ' ';
      
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, au8line1);
      LcdMessage(LINE2_START_ADDR, au8line2);
      
      u32CurrentInterval= u32MillisecondsPassed;
    }
    
    if(u32MillisecondsPassed >= 10000) {
      G_bPlaySong = FALSE;
      u32MillisecondsPassed = 0;
      LedOff(BLUE);
      u8IndexLeft = 0;
      u8IndexRight = 0;
      u8CurrentIndex = 0;
      u32RightTimer = G_u32SystemTime1ms;
      u32LeftTimer = G_u32SystemTime1ms;
      u16CurrentDurationRight = 0;
      u16CurrentDurationLeft = 0;
      u16NoteSilentDurationRight = 0;
      u16NoteSilentDurationLeft = 0;
      bNoteActiveRight = FALSE;
      bNoteActiveLeft = FALSE;
      PWMAudioOff(BUZZER1);
      PWMAudioOff(BUZZER2);
      songDisplay = G_aau8Songs[G_u32CurrentIndexUserApp1];
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, songDisplay);
      LcdMessage(LINE2_START_ADDR, controlsDisplay);
      bResetDone = FALSE;
    }
  }


  if (WasButtonPressed(BUTTON0) && G_bPlaySong == FALSE)
  {
    ButtonAcknowledge(BUTTON0);
    // idk yet
  }
  if (WasButtonPressed(BUTTON1) && G_bPlaySong == FALSE)
  {
    ButtonAcknowledge(BUTTON1);
    u8IndexLeft = 0;
    u8IndexRight = 0;
    u8CurrentIndex = 0;
    u32RightTimer = G_u32SystemTime1ms;
    u32LeftTimer = G_u32SystemTime1ms;
    u16CurrentDurationRight = 0;
    u16CurrentDurationLeft = 0;
    u16NoteSilentDurationRight = 0;
    u16NoteSilentDurationLeft = 0;
    bNoteActiveRight = FALSE;
    bNoteActiveLeft = FALSE;
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    // move current song selection left
    G_u32CurrentIndexUserApp1--;
    if(G_u32CurrentIndexUserApp1 == -1) {
      G_u32CurrentIndexUserApp1 = 9;
    }
    songDisplay = G_aau8Songs[G_u32CurrentIndexUserApp1];
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, songDisplay);
    LcdMessage(LINE2_START_ADDR, controlsDisplay);
  }
  if (WasButtonPressed(BUTTON2) && G_bPlaySong == FALSE)
  {
    ButtonAcknowledge(BUTTON2);
    u8IndexLeft = 0;
    u8IndexRight = 0;
    u8CurrentIndex = 0;
    u32RightTimer = G_u32SystemTime1ms;
    u32LeftTimer = G_u32SystemTime1ms;
    u16CurrentDurationRight = 0;
    u16CurrentDurationLeft = 0;
    u16NoteSilentDurationRight = 0;
    u16NoteSilentDurationLeft = 0;
    bNoteActiveRight = FALSE;
    bNoteActiveLeft = FALSE;
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    // move current song selection right
    G_u32CurrentIndexUserApp1++;
    if(G_u32CurrentIndexUserApp1 == 10) {
      G_u32CurrentIndexUserApp1 = 0;
    }
    songDisplay = G_aau8Songs[G_u32CurrentIndexUserApp1];
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, songDisplay);
    LcdMessage(LINE2_START_ADDR, controlsDisplay);
  }

  if (WasButtonPressed(BUTTON3) && G_bPlaySong == FALSE)
  {
    ButtonAcknowledge(BUTTON3);

    // start game for the current selected song

    G_bPlaySong = TRUE;
  }
  
  if(u32MillisecondsPassed == 4294967295) {
    u32MillisecondsPassed = 0;
  }
  
  u32MillisecondsPassed++;
  
  // RIGHT HAND
  if (IsTimeUp(&u32RightTimer, (u32)u16CurrentDurationRight))
  {
    u8 u8CurrentIndex = u8IndexRight;
    u32 u32CurrentTime = u32RightTimer;
    if (bNoteActiveRight)
    {
      if (au16NoteTypeRight[u8CurrentIndex] == RT)
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
        if (u8IndexRight == u16SizeNotesRight / sizeof(u16))
        {
          u8IndexRight = 0;
        }
      }

      if (au16NotesRight[u8CurrentIndex] != NO)
      {
        PWMAudioSetFrequency(BUZZER1, au16NotesRight[u8CurrentIndex]);
        PWMAudioOn(BUZZER1);
      }
      else
      {
        PWMAudioOff(BUZZER1);
      }
    }
    else
    {
      PWMAudioOff(BUZZER1);
      
      u32RightTimer = G_u32SystemTime1ms;
      u16CurrentDurationRight = u16NoteSilentDurationRight;

      bNoteActiveRight = TRUE;

      u8IndexRight++;
      if (u8IndexRight == u16SizeNotesRight / sizeof(u16))
      {
        u8IndexRight = 0;
      }
    }
  }

  // LEFT HAND
  if (IsTimeUp(&u32LeftTimer, (u32)u16CurrentDurationLeft))
  {
    u8 u8CurrentIndex = u8IndexLeft;
    u32 u32CurrentTime = u32LeftTimer;
    if (bNoteActiveLeft)
    {
      if (au16NoteTypeLeft[u8CurrentIndex] == RT)
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
        if (u8IndexLeft == u16SizeNotesLeft / sizeof(u16))
        {
          u8IndexLeft = 0;
        }
      }

      if (au16NotesLeft[u8CurrentIndex] != NO)
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
      if (u8IndexLeft == u16SizeNotesLeft / sizeof(u16))
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
