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

u8 G_aau8Songs[10][50] = {"Gentle (Tutorial)", "Sunrise", "Forgotten Shadows", "Mystic Dream", "Castle Abyss", "none", "none", "none", "none", "none"};
static u32 G_u32CurrentIndexUserApp1 = 0;
bool G_bPlaySong = FALSE;
bool G_bShowScore = FALSE;
u8 G_au8Line1CA[] = "    * * * *        * * * * ****  *    * *   *";
u8 G_au8Line2CA[] = "    * * * * * * * ** * * *   *  ****   * *  *";
u8 G_au8Line1SR[] = "      *  *   *     *        *  *   *       *      *";
u8 G_au8Line2SR[] = "                    *      * *               *    *";
u8 G_au8Line1FS[] = "  *  *  *  *  * *  *  *  ***   **  *  *   *  *  * *";
u8 G_au8Line2FS[] = "    *  *  *     *    *  *    **   **  *  *  *  *  *";
u8 G_au8Line1MD[] = "  *  *  *  *  * *  *  *  ***   **  *  *   *  *  * *";
u8 G_au8Line2MD[] = "    *  *  *     *    *  *    **   **  *  *  *  *  *";
u8 G_au8Line1TUT[] = "       *      *       *                           *             *";
u8 G_au8Line2TUT[] = "                             *      *      *            *       *";
u8 G_au8Line1[] = "                    ";
u8 G_au8Line2[] = "                    ";
u8 u8perfect = 'o';
u8 u8good = '.';
u8 u8miss = 'x';
u8 G_u8Perfect = 0;
u8 G_u8Good = 0;
u8 G_u8Miss = 0;

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
  
  static u16 au16NotesRightSR[] = {C5, E5, G5, A5, G5, E5, C5, D5, E5, F5, E5, D5, C5, C5, D5, E5, F5, G5, A5, G5, F5, E5, D5, C5};
  static u16 au16DurationRightSR[] = {EN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, EN, QN, EN, EN, EN, EN, QN};
  static u16 au16NoteTypeRightSR[] = {RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT};

  static u16 au16NotesLeftSR[] = {C4, G3, C4, F3, A3, F3, C4, G3};
  static u16 au16DurationLeftSR[] = {HN, HN, HN, HN, HN, HN, HN, HN};
  static u16 au16NoteTypeLeftSR[] = {RT, RT, RT, RT, RT, RT, RT, RT};
  
  static u16 au16NotesRightFS[] = {C4, D4S, G4, G4S, A4S, G4, F4, D4S, C4, C4, G3, G3S, A3S, G3, F3, D3S, C3};
  static u16 au16DurationRightFS[] = {HN, QN, QN, QN, QN, HN, QN, QN, HN, EN, EN, EN, EN, EN, EN, EN, EN};
  static u16 au16NoteTypeRightFS[] = {RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT};

  static u16 au16NotesLeftFS[] = {C4, G3, C3, F3, A3, F3, C3, G3};
  static u16 au16DurationLeftFS[] = {FN, FN, HN, HN, HN, HN, HN, HN};
  static u16 au16NoteTypeLeftFS[] = {RT, RT, RT, RT, RT, RT, RT, RT};

  static u16 au16NotesRightMD[] = {
    E5, B4, C5, D5, E5, D5, C5, B4, A4, A4, C5, E5, D5, C5, B4,
    C5, D5, E5, C5, A4, A4, D5, F5, E5, D5, C5, E5, E5, D5, C5
  };
  static u16 au16DurationRightMD[] = {
    QN, EN, EN, EN, EN, EN, EN, EN, EN, EN, EN, EN, EN, EN, QN,
    EN, EN, EN, EN, EN, EN, EN, EN, EN, EN, QN, QN, QN, QN, FN
  };
  static u16 au16NoteTypeRightMD[] = {
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT
  };
  
  static u16 au16NotesLeftMD[] = {
    A3, E3, A3, C4, E4, A3, C4, E4, D3, A3, D4, F4, A3, E3, A3,
    E4, A4, C4, E4, A3, D4, F4, E3, A3, E4, A3, C4, E4, A3, E3
  };
  static u16 au16DurationLeftMD[] = {
    QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN,
    QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, FN
  };
  static u16 au16NoteTypeLeftMD[] = {
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT
  };
  
  static u16 au16NotesRightTUT[] = {
    C4, NO, NO, NO, E4, NO, NO, NO, G4, NO, NO, NO, E4, NO, NO, NO,
    C4, NO, NO, NO, G3, NO, NO, NO, C4, NO, NO, NO, NO, NO, NO, NO
  };
  static u16 au16DurationRightTUT[] = {
    EN, QN, QN, QN, EN, QN, QN, QN, EN, QN, QN, QN, EN, QN, QN, QN,
    EN, QN, QN, QN, EN, QN, QN, QN, EN, QN, QN, QN, QN, QN, QN, FN
  };
  static u16 au16NoteTypeRightTUT[] = {
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT
  };
  
  static u16 au16NotesLeftTUT[] = {
    C3, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO,
    G3, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO
  };
  static u16 au16DurationLeftTUT[] = {
    EN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN,
    EN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, QN, FN
  };
  static u16 au16NoteTypeLeftTUT[] = {
      RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT,
      RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT, RT
  };
  
  static u16 au16NotesRightCA[] = {
    C4, D4, F4, G4S, F4, D4, A3,
    C4, F4, G5, G4S, G5, F4, C4,
    C5, F5, G4, G4S, G4, F5, C5,
  };
  static u16 au16DurationRightCA[] = {
    QN, QN, QN, HN, QN, QN, QN,
    QN, QN, HN, FN, HN, QN, QN,
    QN, QN, QN, HN, QN, QN, QN
  };
  static u16 au16NoteTypeRightCA[] = {
    RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT
  };
  
  static u16 au16NotesLeftCA[] = {
    D3, F3, G4, A3S, G4, F3, C3,
    D3, F3, G3, F3S, A3, F3, G3, F3, C3,
    D4, F4, G3, A3S, G3, F4, C4,
  };
  static u16 au16DurationLeftCA[] = {
    QN, QN, QN, HN, QN, QN, QN,
    QN, QN, QN, QN, FN, QN, QN, QN, QN,
    QN, QN, QN, HN, QN, QN, QN
  };
  static u16 au16NoteTypeLeftCA[] = {
    RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT, RT, RT,
    RT, RT, RT, RT, RT, RT, RT
  };

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
  
  static u8 line1[] = "                    ";
  static u8 line2[] = "                    ";
  static u8 line1SR[] = "      *  *   *     *        *  *   *       *      *";
  static u8 line2SR[] = "                    *      * *               *    *";
  static u8 line1FS[] = "  *  *  *  *  * *  *  *  ***   **  *  *   *  *  * *";
  static u8 line2FS[] = "    *  *  *     *    *  *    **   **  *  *  *  *  *";
  static u8 line1MD[] = "  *  *  *  *  * *  *  *  ***   **  *  *   *  *  * *";
  static u8 line2MD[] = "    *  *  *     *    *  *    **   **  *  *  *  *  *";
  static u8 line1TUT[] = "       *      *       *                           *             *";
  static u8 line2TUT[] = "                             *      *      *            *       *";
  static u8 line1CA[] = "    * * * *        * * * * ****  *    * *   *";
  static u8 line2CA[] = "    * * * * * * * ** * * *   *  ****   * *  *";
  static u32 u32GameEndMS;
  
  static bool bTutorial = FALSE;
  
  static bool bMissRedOn = FALSE;
  static bool bPerfectGreenOn = FALSE;
  static bool bGoodYellowOn = FALSE;
  
  static u8 au8star[] = "*";
  static u8 au8space[] = " ";
  
  static int length;
  
  static bool bCalculated = FALSE;
  
  if(G_bShowScore) {
    if(!bCalculated) {
      u8 i = 0;
      u8 tempPerfect[4];
      if (G_u8Perfect == 0) {
          tempPerfect[i++] = '0';
      } else {
          while (G_u8Perfect > 0) {
              tempPerfect[i++] = (G_u8Perfect % 10) + '0';
              G_u8Perfect /= 10;
          }
      }
      tempPerfect[i] = '\0';
      for (u8 start = 0, end = i - 1; start < end; start++, end--) {
          u8 temp = tempPerfect[start];
          tempPerfect[start] = tempPerfect[end];
          tempPerfect[end] = temp;
      }
      
      i = 0;
      u8 tempGood[4];
      if (G_u8Good == 0) {
          tempGood[i++] = '0';
      } else {
          while (G_u8Good > 0) {
              tempGood[i++] = (G_u8Good % 10) + '0';
              G_u8Good /= 10;
          }
      }
      tempGood[i] = '\0';
      for (u8 start = 0, end = i - 1; start < end; start++, end--) {
          u8 temp = tempGood[start];
          tempGood[start] = tempGood[end];
          tempGood[end] = temp;
      }
      
      i = 0;
      u8 tempMiss[4];
      if (G_u8Miss == 0) {
          tempMiss[i++] = '0';
      } else {
          while (G_u8Miss > 0) {
              tempMiss[i++] = (G_u8Miss % 10) + '0';
              G_u8Miss /= 10;
          }
      }
      tempMiss[i] = '\0';
      for (u8 start = 0, end = i - 1; start < end; start++, end--) {
          u8 temp = tempMiss[start];
          tempMiss[start] = tempMiss[end];
          tempMiss[end] = temp;
      }
      
      u8 displayScore[20] = {0};
      u8 currentLength = 0;

      for(u8 i = 0; tempPerfect[i] != '\0' && currentLength < sizeof(displayScore) - 1; i++) {
          displayScore[currentLength++] = tempPerfect[i];
      }

      if (currentLength < sizeof(displayScore) - 1) {
          displayScore[currentLength++] = ' ';
      }

      for(u8 i = 0; tempGood[i] != '\0' && currentLength < sizeof(displayScore) - 1; i++) {
          displayScore[currentLength++] = tempGood[i];
      }

      if (currentLength < sizeof(displayScore) - 1) {
          displayScore[currentLength++] = ' ';
      }

      for(u8 i = 0; tempMiss[i] != '\0' && currentLength < sizeof(displayScore) - 1; i++) {
          displayScore[currentLength++] = tempMiss[i];
      }

      displayScore[currentLength] = '\0';
    
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "Perfect Good Miss");
      LcdMessage(LINE2_START_ADDR, displayScore);
      
      bCalculated = TRUE;
    }
    
    if(WasButtonPressed(BUTTON0) || WasButtonPressed(BUTTON1) || WasButtonPressed(BUTTON2) || WasButtonPressed(BUTTON3)) {
      ButtonAcknowledge(BUTTON0);
      ButtonAcknowledge(BUTTON1);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON3);
      
      G_u8Perfect = 0;
      G_u8Good = 0;
      G_u8Miss = 0;
      G_bShowScore = FALSE;
      
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, songDisplay);
      LcdMessage(LINE2_START_ADDR, controlsDisplay);
    }
  }
  
  if(G_u32CurrentIndexUserApp1 == 0) {
    au16NotesLeft = au16NotesLeftTUT;
    au16DurationLeft = au16DurationLeftTUT;
    au16NoteTypeLeft = au16NoteTypeLeftTUT;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftTUT);
    
    au16NotesRight = au16NotesRightTUT;
    au16DurationRight = au16DurationRightTUT;
    au16NoteTypeRight = au16NoteTypeRightTUT;
    
    u16SizeNotesRight = sizeof(au16NotesRightTUT);
    
    u32GameEndMS = 34000;
  } else if(G_u32CurrentIndexUserApp1 == 1) {
    au16NotesLeft = au16NotesLeftSR;
    au16DurationLeft = au16DurationLeftSR;
    au16NoteTypeLeft = au16NoteTypeLeftSR;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftSR);
    
    au16NotesRight = au16NotesRightSR;
    au16DurationRight = au16DurationRightSR;
    au16NoteTypeRight = au16NoteTypeRightSR;
    
    u16SizeNotesRight = sizeof(au16NotesRightSR);
    
    u32GameEndMS = 20000;
  } else if(G_u32CurrentIndexUserApp1 == 2) {
    au16NotesLeft = au16NotesLeftFS;
    au16DurationLeft = au16DurationLeftFS;
    au16NoteTypeLeft = au16NoteTypeLeftFS;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftFS);
    
    au16NotesRight = au16NotesRightFS;
    au16DurationRight = au16DurationRightFS;
    au16NoteTypeRight = au16NoteTypeRightFS;
    
    u16SizeNotesRight = sizeof(au16NotesRightFS);
    
    u32GameEndMS = 24000;
  } else if(G_u32CurrentIndexUserApp1 == 3) {
    au16NotesLeft = au16NotesLeftMD;
    au16DurationLeft = au16DurationLeftMD;
    au16NoteTypeLeft = au16NoteTypeLeftMD;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftMD);
    
    au16NotesRight = au16NotesRightMD;
    au16DurationRight = au16DurationRightMD;
    au16NoteTypeRight = au16NoteTypeRightMD;
    
    u16SizeNotesRight = sizeof(au16NotesRightMD);
    
    u32GameEndMS = 16000;
  } else if(G_u32CurrentIndexUserApp1 == 4) {
    au16NotesLeft = au16NotesLeftCA;
    au16DurationLeft = au16DurationLeftCA;
    au16NoteTypeLeft = au16NoteTypeLeftCA;
    
    u16SizeNotesLeft = sizeof(au16NotesLeftCA);
    
    au16NotesRight = au16NotesRightCA;
    au16DurationRight = au16DurationRightCA;
    au16NoteTypeRight = au16NoteTypeRightCA;
    
    u16SizeNotesRight = sizeof(au16NotesRightCA);
    
    u32GameEndMS = 20000;
  } else {
    au16NotesLeft = au16EmptyNotes;
    au16DurationLeft = au16EmptyDuration;
    au16NoteTypeLeft = au16EmptyNoteType;
    
    u16SizeNotesLeft = sizeof(au16EmptyNotes);
    
    au16NotesRight = au16EmptyNotes;
    au16DurationRight = au16EmptyDuration;
    au16NoteTypeRight = au16EmptyNoteType;
    
    u16SizeNotesRight = sizeof(au16EmptyNotes);
    
    u32GameEndMS = 1000;
  }

  static bool bResetDone = FALSE;
  static u16 intTUTStep = 0;
  
  if (G_bPlaySong && bTutorial) {
    if(intTUTStep == 0) {
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "o                   ");
      LcdMessage(LINE2_START_ADDR, "top ---      --- ---");
      intTUTStep++;
    } else if(intTUTStep == 1) {
      if (WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
        intTUTStep++;
      }
    } else if(intTUTStep == 2) {
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "o                   ");
      LcdMessage(LINE2_START_ADDR, "--- top      --- ---");
      intTUTStep++;
    } else if(intTUTStep == 3) {
      if (WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
        intTUTStep++;
      }
    } else if(intTUTStep == 4) {
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "                    ");
      LcdMessage(LINE2_START_ADDR, "o            bot ---");
      intTUTStep++;
    } else if(intTUTStep == 5) {
      if (WasButtonPressed(BUTTON2))
      {
        ButtonAcknowledge(BUTTON2);
        intTUTStep++;
      }
    } else if(intTUTStep == 6) {
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "                    ");
      LcdMessage(LINE2_START_ADDR, "o            --- bot");
      intTUTStep++;
    } else if(intTUTStep == 7) {
      if (WasButtonPressed(BUTTON3))
      {
        ButtonAcknowledge(BUTTON3);
        intTUTStep++;
      }
    } else if(intTUTStep == 8) {
      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, "Starting Game...    ");
      LcdMessage(LINE2_START_ADDR, "Tutorial Complete   ");
      intTUTStep++;
    } else if(intTUTStep < 1000) {
      intTUTStep++;
    } else {
      bTutorial = FALSE;
      intTUTStep = 0;
    }
  }
  
  if (G_bPlaySong && !bTutorial)
  {
    if(G_u32CurrentIndexUserApp1 == 0) {
      au8line1 = line1TUT;
      au8line2 = line2TUT;
      length = sizeof(line1TUT) / sizeof(line1TUT[0]);
      u16NoteMoveInterval = 500;
    } else if(G_u32CurrentIndexUserApp1 == 1) {
      au8line1 = line1SR;
      au8line2 = line2SR;
      length = sizeof(line1SR) / sizeof(line1SR[0]);
      u16NoteMoveInterval = 350;
    } else if(G_u32CurrentIndexUserApp1 == 2) {
      au8line1 = line1FS;
      au8line2 = line2FS;
      length = sizeof(line1FS) / sizeof(line1FS[0]);
      u16NoteMoveInterval = 400;
    } else if(G_u32CurrentIndexUserApp1 == 3) {
      au8line1 = line1MD;
      au8line2 = line2MD;
      length = sizeof(line1MD) / sizeof(line1MD[0]);
      u16NoteMoveInterval = 250;
    } else if(G_u32CurrentIndexUserApp1 == 4) {
      au8line1 = line1CA;
      au8line2 = line2CA;
      length = sizeof(line1CA) / sizeof(line1CA[0]);
      u16NoteMoveInterval = 350;
    } else {
      au8line1 = line1;
      au8line2 = line2;
      length = sizeof(line1) / sizeof(line1[0]);
      u16NoteMoveInterval = 350;
    }
    
    if (!bResetDone)
    {
      u32CurrentInterval = 0;
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
      
      u8 temp1 = au8line1[20];
      u8 temp2 = au8line2[20];
      au8line1[20] = '\0';
      au8line2[20] = '\0';

      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, au8line1);
      LcdMessage(LINE2_START_ADDR, au8line2);

      au8line1[20] = temp1;
      au8line2[20] = temp2;
      
      bResetDone = TRUE;
    }
    
    if (WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      if(au8line1[0] == au8star[0]) {
        bPerfectGreenOn = TRUE;
        G_u8Perfect++;
        LedOn(GREEN);
        au8line1[0] = u8perfect;
      } else if(au8line1[1] == au8star[0]) {
        bGoodYellowOn = TRUE;
        G_u8Good++;
        LedOn(YELLOW);
        au8line1[1] = u8good;
      } else if(au8line1[2] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line1[2] = u8miss;
      } else if(au8line1[3] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line1[3] = u8miss;
      }
    }
    
    if (WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      if(au8line1[0] == au8star[0]) {
        bPerfectGreenOn = TRUE;
        G_u8Perfect++;
        LedOn(GREEN);
        au8line1[0] = u8perfect;
      } else if(au8line1[1] == au8star[0]) {
        bGoodYellowOn = TRUE;
        G_u8Good++;
        LedOn(YELLOW);
        au8line1[1] = u8good;
      } else if(au8line1[2] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line1[2] = u8miss;
      } else if(au8line1[3] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line1[3] = u8miss;
      }
    }
    
    if (WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      if(au8line2[0] == au8star[0]) {
        bPerfectGreenOn = TRUE;
        G_u8Perfect++;
        LedOn(GREEN);
        au8line2[0] = u8perfect;
      } else if(au8line2[1] == au8star[0]) {
        bGoodYellowOn = TRUE;
        G_u8Good++;
        LedOn(YELLOW);
        au8line2[1] = u8good;
      } else if(au8line2[2] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line2[2] = u8miss;
      } else if(au8line2[3] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line2[3] = u8miss;
      }
    }
    
    if (WasButtonPressed(BUTTON3))
    {
      ButtonAcknowledge(BUTTON3);
      if(au8line2[0] == au8star[0]) {
        bPerfectGreenOn = TRUE;
        G_u8Perfect++;
        LedOn(GREEN);
        au8line2[0] = u8perfect;
      } else if(au8line2[1] == au8star[0]) {
        bGoodYellowOn = TRUE;
        G_u8Good++;
        LedOn(YELLOW);
        au8line2[1] = u8good;
      } else if(au8line2[2] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line2[2] = u8miss;
      } else if(au8line2[3] == au8star[0]) {
        bMissRedOn = TRUE;
        G_u8Miss++;
        LedOn(RED);
        au8line2[3] = u8miss;
      }
    }
      
    // gameplay goes here
    LedOn(BLUE);
    if(u32CurrentInterval + u16NoteMoveInterval < u32MillisecondsPassed) {
      if(bMissRedOn) {
        LedOff(RED);
        bMissRedOn = FALSE;
      }
      if(bPerfectGreenOn) {
        LedOff(GREEN);
        bPerfectGreenOn = FALSE;
      }
      if(bGoodYellowOn) {
        LedOff(YELLOW);
        bGoodYellowOn = FALSE;
      }
      if(au8line1[0] == au8star[0] || au8line2[0] == au8star[0]) {
        LedOn(RED);
        G_u8Miss++;
        bMissRedOn = TRUE;
      }
      for (int i = 1; i < length; i++) {
          au8line1[i-1] = au8line1[i];
          au8line2[i-1] = au8line2[i];
      }
      au8line1[length - 2] = au8space[0];
      au8line2[length - 2] = au8space[0];
      
      u8 temp1 = au8line1[20];
      u8 temp2 = au8line2[20];
      au8line1[20] = '\0';
      au8line2[20] = '\0';

      LcdCommand(LCD_CLEAR_CMD);
      LcdMessage(LINE1_START_ADDR, au8line1);
      LcdMessage(LINE2_START_ADDR, au8line2);

      au8line1[20] = temp1;
      au8line2[20] = temp2;
      
      u32CurrentInterval = u32MillisecondsPassed;
    }
    
    if(u32MillisecondsPassed >= u32GameEndMS) {
      G_bPlaySong = FALSE;
      u32MillisecondsPassed = 0;
      LedOff(BLUE);
      LedOff(GREEN);
      LedOff(YELLOW);
      LedOff(RED);
      bMissRedOn = FALSE;
      bPerfectGreenOn = FALSE;
      bGoodYellowOn = FALSE;
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
      if(G_u32CurrentIndexUserApp1 == 0) {
        memcpy(line1TUT, G_au8Line1TUT, sizeof(line1TUT));
        memcpy(line2TUT, G_au8Line2TUT, sizeof(line2TUT));
      } else if(G_u32CurrentIndexUserApp1 == 1) {
        memcpy(line1SR, G_au8Line1SR, sizeof(line1SR));
        memcpy(line2SR, G_au8Line2SR, sizeof(line2SR));
      } else if(G_u32CurrentIndexUserApp1 == 2) {
        memcpy(line1FS, G_au8Line1FS, sizeof(line1FS));
        memcpy(line2FS, G_au8Line2FS, sizeof(line2FS));
      } else if(G_u32CurrentIndexUserApp1 == 3) {
        memcpy(line1MD, G_au8Line1MD, sizeof(line1MD));
        memcpy(line2MD, G_au8Line2MD, sizeof(line2MD));
      } else if(G_u32CurrentIndexUserApp1 == 4) {
        memcpy(line1CA, G_au8Line1CA, sizeof(line1CA));
        memcpy(line2CA, G_au8Line2CA, sizeof(line2CA));
      } else {
        memcpy(line1, G_au8Line1, sizeof(line1));
        memcpy(line2, G_au8Line2, sizeof(line2));
      }
      bCalculated = FALSE;
      G_bShowScore = TRUE;
      bResetDone = FALSE;
    }
  }


  if (WasButtonPressed(BUTTON0) && G_bPlaySong == FALSE && G_bShowScore == FALSE)
  {
    ButtonAcknowledge(BUTTON0);
  }
  if (WasButtonPressed(BUTTON1) && G_bPlaySong == FALSE && G_bShowScore == FALSE)
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
  if (WasButtonPressed(BUTTON2) && G_bPlaySong == FALSE && G_bShowScore == FALSE)
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

  if (WasButtonPressed(BUTTON3) && G_bPlaySong == FALSE && G_bShowScore == FALSE)
  {
    ButtonAcknowledge(BUTTON3);

    // start game for the current selected song
    // allow tutorial text for buttons
    if(G_u32CurrentIndexUserApp1 == 0) {
      bTutorial = TRUE;
    } else {
      bTutorial = FALSE;
    }

    G_bPlaySong = TRUE;
  }
  
  if(u32MillisecondsPassed == 4294900000) {
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
