#include "timerISR.h"
#include "helper.h"
#include "periph.h"
#include "LCD.h"
#include "spiAVR.h"
#include "shuan284_misc.h"
#include <stdlib.h>
#include <stdio.h>


#define NUM_TASKS 5

//Task struct for concurrent synchSMs implmentations
typedef struct _task{
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;

//TODO: Define Periods for each task
// e.g. const unsined long TASK1_PERIOD = <PERIOD>
const unsigned long PB_PERIOD = 250; // Passive Buzzer Task
const unsigned long SPI_PERIOD = 250; // SPI Display Task
const unsigned long G_PERIOD = 100; // Game Task
const unsigned long LCD_PERIOD = 200; // 16x2 LCD Task
const unsigned long UI_PERIOD = 50; // User Input Task
const unsigned long GCD_PERIOD = 50; //TODO:Set the GCD Period

// global variables
unsigned char gamemode = 0;
unsigned long score = 0;
unsigned char currLetterIdx = 0;
unsigned char livesRemaining = 3;
unsigned char userAnswer[4] = {0,0,0,0};
unsigned char waitInput = 0;
unsigned char inputDone = 0; 
unsigned char feedbackSound = 0; // 0 = no feedback to give, 1 = correct sound, 2 = incorrect sound
unsigned char feedbackDone = 0;
unsigned char timeRemaining;

task tasks[NUM_TASKS]; // declared task array with 5 tasks

void TimerISR() {
	for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
		if ( tasks[i].elapsedTime >= tasks[i].period ) {           // Check if the task is ready to tick
			tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
			tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
		}
		tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
	}
}

//TODO: Create your tick functions for each task

// Passive Buzzer Task
enum PB_States { PB_Start, PB_Music, PB_Correct, PB_Wrong };

void playNote(float noteHz)
{
  if (noteHz == 0.0) OCR1A = 0;
  else
  {
    long TOP = (16000000 / (8.0 * noteHz)) - 1;
    ICR1 = TOP;
    OCR1A = TOP/2;
  }
}


int PB_Tick(int state)
{
  //                         F#4    Ab4     C#4                      B3      B4      A4       Ab4     A4    A4                       B3      E4      Eb4     C#4     B3      Ab3
  //static float music[29] = {369.99, 415.30, 277.18, 277.18, 277.18, 246.94, 493.88, 440.00, 415.30, 440.00, 440.00, 440.00, 440.00, 277.18, 329.63, 311.13, 277.18, 246.94, 207.65, 207.65, 207.65, 207.65, 207.65, 0, 0, 0, 0, 0, 0};
  //                         Bb3      G4      F4      D4      Eb4             Bb3      G4      F4      D4      D4
  static float music[16] = {233.08, 392.00, 349.23, 293.66, 311.13, 0, 0, 0, 233.08, 392.00, 349.23, 293.66, 293.66, 0, 0, 0};
  static unsigned char PB_pos;
  static unsigned char PB_i;

  switch (state) // Transitions
  {
    case PB_Start: PB_pos = 0; state = PB_Music; break;
    case PB_Music: 
      if (feedbackSound == 0)
      {
        state = PB_Music;
      }
      else if (feedbackSound == 1)
      {
        feedbackDone = 0;
        PB_i = 0;
        state = PB_Correct;
      }
      else if (feedbackSound == 2)
      {
        feedbackDone = 0;
        PB_i = 0;
        state = PB_Wrong;
      }
      break;
    case PB_Correct:
      if (PB_i < 2)
      {
        state = PB_Correct;
      }
      else
      {
        feedbackDone = 1;
        state = PB_Music;
      }
      break;
    case PB_Wrong:
      if (PB_i < 2)
      {
        state = PB_Wrong;
      }
      else
      {
        feedbackDone = 1;
        state = PB_Music;
      }
      break;
    default: break;
  }

  switch (state) // Actions
  {
    case PB_Music:
      playNote(music[PB_pos]);
      PB_pos++;
      if (PB_pos >= 16) PB_pos = 0;
      break;
    case PB_Correct:
      if (PB_i == 0) playNote(698.46); // F5
      else playNote(880.00); // A5
      PB_i++;
      break;
    case PB_Wrong:
      if (PB_i == 0) playNote(880.00); // A5
      else playNote(698.46); // F5
      PB_i++;
      break;
    default: break;
  }

  return state;
}

// SPI Display Task
enum SPI_States { SPI_Start, SPI_MainMenu, SPI_TrainingMode, SPI_EndlessMode };
int SPI_Tick(int state)
{
  static unsigned char SPI_currDispLet;
  static unsigned char SPI_currDispLives;
  static unsigned char SPI_currDispTime;

  switch (state) // Transitions
  {
    case SPI_Start:
      displayMCM();
      state = SPI_MainMenu;
      break;
    case SPI_MainMenu:
      if (gamemode == 0)
      {
        state = SPI_MainMenu;
      }
      else
      {
        if (gamemode == 1)
        {
          setBackground(0x00, 0x00, 0x00);
          SPI_currDispLet = currLetterIdx;
          displayLetter(currLetterIdx);
          displayCode(currLetterIdx);
          state = SPI_TrainingMode;
        }
        else
        {
          setBackground(0x00, 0x00, 0x00);
          SPI_currDispLet = currLetterIdx;
          SPI_currDispLives = livesRemaining;
          SPI_currDispTime = timeRemaining;
          displayLetter(currLetterIdx);
          //displayCode(currLetterIdx); // FOR TESTING REMOVE LATER 
          displayHearts(livesRemaining);
          displayTime(timeRemaining);
          state = SPI_EndlessMode;
        }
      }
      break;
    case SPI_TrainingMode:
      if (gamemode == 1)
      {
        state = SPI_TrainingMode;
      }
      else
      {
        displayMCM();
        state = SPI_MainMenu;
      }
      break;
    case SPI_EndlessMode: 
      if (gamemode == 2)
      {
        state = SPI_EndlessMode;
      }
      else
      {
        displayMCM();
        state = SPI_MainMenu;
      }
      break;
    default: break;
  }

  switch (state) // Actions
  {
    case SPI_MainMenu: break;
    case SPI_TrainingMode: 
      if (SPI_currDispLet != currLetterIdx)
      {
        displayLetter(currLetterIdx);
        displayCode(currLetterIdx);
        SPI_currDispLet = currLetterIdx;
      }
      break;
    case SPI_EndlessMode: 
      if (SPI_currDispLet != currLetterIdx)
      {
        displayLetter(currLetterIdx);
        //displayCode(currLetterIdx); // FOR TESTING REMOVE LATER 
        SPI_currDispLet = currLetterIdx;
      }
      if (SPI_currDispLives != livesRemaining)
      {
        displayHearts(livesRemaining);
        SPI_currDispLives = livesRemaining;
      }
      if (SPI_currDispTime != timeRemaining)
      {
        displayTime(timeRemaining);
        SPI_currDispTime = timeRemaining;
      }
      break;
    default: break;
  }

  return state;
}

// Game Task
enum G_States { G_Start, G_WaitRelease, G_Idle, G_WaitInputT, G_CheckAnswerT, G_WaitInputE, G_CheckAnswerE, G_TimeOut };
int G_Tick(int state)
{
  static unsigned char G_i;
  static unsigned char G_correct; // 0 = wrong, 1 = correct, 2 = means nothing
  static unsigned char G_ansLen;
  static unsigned char G_time;
  static unsigned char G_timeLimit = 90;

  switch (state) // Transitions
  {
    case G_Start:
      currLetterIdx = 0;
      livesRemaining = 3;
      gamemode = 0;
      score = 0;
      waitInput = 0;
      feedbackSound = 0;
      state = G_Idle;
      break;
    case G_Idle:
      if ((GetBit(PINC, 1) == 0x01) && (GetBit(PINC, 2) == 0x00)) // A1 && !A2
      {
        G_i = 0;
        gamemode = 1;
        state = G_WaitInputT;
      }
      else if ((GetBit(PINC, 1) == 0x00) && (GetBit(PINC, 2) == 0x01)) // !A1 && A2
      {
        G_i = rand() % 26;
        currLetterIdx = G_i;
        score = 0;
        gamemode = 2;
        livesRemaining = 3;
        G_timeLimit = 90;
        G_time = G_timeLimit; // 9 seconds to start, minimum possible 3
        state = G_WaitInputE;
      }
      else
      {
        state = G_Idle;
      }
      break;
    case G_WaitInputT:
      if (GetBit(PINC, 0) == 0x01)
      {
        gamemode = 0;
        state = G_Start;
      }
      else
      {
        if (!inputDone)
        {
          state = G_WaitInputT;
        }
        else
        {
          waitInput = 0;
          state = G_CheckAnswerT;
        }
      }
      break;
    case G_CheckAnswerT:
      if (GetBit(PINC, 0) == 0x01)
      {
        gamemode = 0;
        state = G_Start;
      }
      else
      {
        if (!feedbackDone)
        {
          state = G_CheckAnswerT;
        }
        else
        {
          feedbackSound = 0;
          if (G_correct == 0)
          {
            state = G_WaitInputT;
          }
          else if (G_correct == 1)
          {
            if (G_i < 25)
            {
              G_i++;
              currLetterIdx = G_i;
              state = G_WaitInputT;
            }
            else
            {
              gamemode = 0;
              state = G_Idle;
            }
          }
        }
      }
      break;
    case G_WaitInputE:
      if (GetBit(PINC, 0) == 0x01)
      {
        gamemode = 0;
        state = G_Start;
      }
      else
      {
        if (G_time == 0)
        {
          waitInput = 0;
          state = G_TimeOut;
        }
        else
        {
          if (inputDone)
          {
            waitInput = 0;
            state = G_CheckAnswerE;
          }
          else
          {
            state = G_WaitInputE;
          }
        }
      }
      break;
    case G_CheckAnswerE:
      if (GetBit(PINC, 0) == 0x01)
      {
        gamemode = 0;
        state = G_Start;
      }
      else
      {
        if (!feedbackDone)
        {
          state = G_CheckAnswerE;
        }
        else
        {
          feedbackSound = 0;
          if (livesRemaining > 0)
          {
            
            G_i = rand() % 26;
            currLetterIdx = G_i;
            
            G_time = G_timeLimit;
            state = G_WaitInputE;
          }
          else
          {
            gamemode = 0;
            state = G_Idle;
          }
        }
      }
      break;
    case G_TimeOut:
      if (GetBit(PINC, 0) == 0x01)
      {
        gamemode = 0;
        state = G_Start;
      }
      else
      {
        if (!feedbackDone)
        {
          state = G_TimeOut;
        }
        else 
        {
          if (livesRemaining > 0)
          {
            G_i = rand() % 26;
            currLetterIdx = G_i;
            G_time = G_timeLimit;
            feedbackSound = 0;
            state = G_WaitInputE;
          }
          else
          {
            gamemode = 0;
            feedbackSound = 0;
            state = G_Idle;
          }
        }
      }
      break;
    default: break;
  }

  switch (state) // Actions
  {
    case G_Idle:
      currLetterIdx = 0;
      livesRemaining = 3;
      waitInput = 0;
      feedbackSound = 0;
      break;
    case G_WaitInputT:
      waitInput = 1;
      break;
    case G_CheckAnswerT:
      G_correct = 1;
      G_ansLen = 0;
      for (unsigned char i = 0; i < 4; i++)
      {
        if (code[currLetterIdx][i] > 0) G_ansLen++;
      }

      for (unsigned char i = 0; i < G_ansLen; i++)
      {
        if (userAnswer[i] != code[currLetterIdx][i])
        {
          G_correct = 0;
          break;
        }
      }

      if (G_correct == 1)
      {
        feedbackSound = 1;
      }
      else
      {
        feedbackSound = 2;
      }
      feedbackDone = 0;
      break;
    case G_WaitInputE:
      waitInput = 1;
      G_time--;
      timeRemaining = (G_time + 9) / 10;
      break;
    case G_CheckAnswerE:
      if (feedbackSound == 0)
      {
        G_correct = 1;
        G_ansLen = 0;
        for (unsigned char i = 0; i < 4; i++)
        {
          if (code[currLetterIdx][i] > 0) G_ansLen++;
        }

        for (unsigned char i = 0; i < G_ansLen; i++)
        {
          if (userAnswer[i] != code[currLetterIdx][i])
          {
            G_correct = 0;
            break;
          }
        }

        if (G_correct == 1)
        {
          score++;

          if ((score % 3 == 0) && (G_timeLimit > 30))
          {
            G_timeLimit -= 20;
          }

          feedbackSound = 1;
        }
        else
        {
          livesRemaining--;
          feedbackSound = 2;
        }
        feedbackDone = 0;
      }
      break;
    case G_TimeOut:
      if (feedbackSound == 0)
      {
        livesRemaining--;
        waitInput = 0;
        feedbackSound = 2;
        feedbackDone = 0;
      }
      break;
    default: break;
  }

  return state;
}

// 16x2 LCD Screen Task
enum LCD_States { LCD_Start, LCD_MainMenu, LCD_Training, LCD_Endless, LCD_Feedback };
int LCD_Tick(int state)
{
  static unsigned char LCD_i;
  static unsigned char LCD_x;
  static char LCD_scoreStr[4];
  static char LCD_livesStr[2];
  static unsigned long LCD_currDispScore;
  static char LCD_currDispLives;

  switch (state) // Transitions
  {
    case LCD_Start: LCD_i = 0; LCD_x = 0; state = LCD_MainMenu; break;
    case LCD_MainMenu:
      if (gamemode == 0)
      {
        state = LCD_MainMenu;
      }
      else
      {
        if (gamemode == 1)
        {
          lcd_clear();
          lcd_goto_xy(0,0);
          lcd_write_str("Use Blue Button");
          lcd_goto_xy(1,0);
          lcd_write_str("to enter code");
          state = LCD_Training;
        }
        else
        {
          if (gamemode == 2)
          {
            lcd_clear();
            lcd_goto_xy(0,0);
            lcd_write_str("Score: ");
            lcd_goto_xy(0, 8);
            snprintf(LCD_scoreStr, sizeof(LCD_scoreStr), "%lu", score);
            lcd_write_str(LCD_scoreStr);
            lcd_goto_xy(1,0);
            lcd_write_str("Lives: ");
            lcd_goto_xy(1, 8);
            snprintf(LCD_livesStr, sizeof(LCD_livesStr), "%u", livesRemaining);
            lcd_write_str(LCD_livesStr);

            LCD_currDispLives = livesRemaining;
            LCD_currDispScore = score;

            state = LCD_Endless;
          }
          else
          {
            state = LCD_MainMenu;
          }
        }
      }
      break;
    case LCD_Training:
      if (gamemode == 0)
      {
        LCD_i = 0;
        LCD_x = 0;
        state = LCD_MainMenu;
      }
      else if (feedbackSound == 1 || feedbackSound == 2)
      {
        lcd_clear();
        lcd_goto_xy(0,3);
        if (feedbackSound == 1)
        {
          lcd_write_str("Good job!");
        }
        else
        {
          lcd_write_str("Try Again");
        }
        state = LCD_Feedback;
      }
      else
      {
        state = LCD_Training;
      }
      break;
    case LCD_Feedback:
      if (feedbackDone)
      {
        lcd_clear();
        lcd_goto_xy(0,0);
        lcd_write_str("Use Blue Button");
        lcd_goto_xy(1,0);
        lcd_write_str("to enter code");
        state = LCD_Training;
      }
      else
      {
        state = LCD_Feedback;
      }
      break;
    case LCD_Endless:
      if (gamemode > 0)
      {
        state = LCD_Endless;
      }
      else
      {
        LCD_i = 0;
        LCD_x = 0;
        state = LCD_MainMenu;
      }
      break;
    default: break;  
  }

  switch (state) // Actions
  {
    case LCD_MainMenu:
      if (LCD_i == 0)
      {
        if (LCD_x == 0)
        {
          lcd_clear();
          lcd_goto_xy(0,3);
          lcd_write_str("Morse Code");
          lcd_goto_xy(1,5);
          lcd_write_str("Mania!");

        }
        else if (LCD_x == 1)
        {
          lcd_clear();
          lcd_goto_xy(0,3);
          lcd_write_str("Red Button");
          lcd_goto_xy(1,2);
          lcd_write_str("for Training");
        }
        else if (LCD_x == 2)
        {
          lcd_clear();
          lcd_goto_xy(0,2);
          lcd_write_str("Blue Button");
          lcd_goto_xy(1,2);
          lcd_write_str("for Endless");
        } 
        else if (LCD_x == 3)
        {
          lcd_clear();
          lcd_goto_xy(0,2);
          lcd_write_str("Prev Endless");
          lcd_goto_xy(1,3);
          lcd_write_str("Score: ");
          lcd_goto_xy(1,10);
          snprintf(LCD_scoreStr, sizeof(LCD_scoreStr), "%lu", score);
          lcd_write_str(LCD_scoreStr);
        }
        LCD_x++;
        if (LCD_x > 3) LCD_x = 0;
      }
      LCD_i++;
      if (LCD_i > 10) LCD_i = 0;
      break;
    case LCD_Training: break;
    case LCD_Endless:
      if ((LCD_currDispLives != livesRemaining) || (LCD_currDispScore != score))
      {
        lcd_clear();
        lcd_goto_xy(0,0);
        lcd_write_str("Score: ");
        lcd_goto_xy(0, 8);
        snprintf(LCD_scoreStr, sizeof(LCD_scoreStr), "%lu", score);
        lcd_write_str(LCD_scoreStr);
        lcd_goto_xy(1,0);
        lcd_write_str("Lives: ");
        lcd_goto_xy(1, 8);
        snprintf(LCD_livesStr, sizeof(LCD_livesStr), "%u", livesRemaining);
        lcd_write_str(LCD_livesStr);

        LCD_currDispScore = score;
        LCD_currDispLives = livesRemaining;
      }
      break;
      case LCD_Feedback: break;
    default: break;
  }

  return state;
}

// User Input Task
enum UI_States { UI_Start, UI_Idle, UI_Wait, UI_Press };
int UI_Tick(int state)
{
  static unsigned char UI_duration;
  static unsigned char UI_pos;
  static unsigned char UI_ansLen;

  switch (state) // Transitions
  {
    case UI_Start: 
      UI_ansLen = 0;
      UI_pos = 0;
      inputDone = 0;
      state = UI_Idle;
      break;
    case UI_Idle:
      if (gamemode == 0)
      {
        UI_pos = 0;
        inputDone = 0;
        state = UI_Idle;
      }
      else
      {
        if (!waitInput)
        {
          state = UI_Idle;
        }
        else
        {
          UI_ansLen = 0;
          for (unsigned char i = 0; i < 4; i++)
          {
            userAnswer[i] = 0;
            if (code[currLetterIdx][i] > 0) UI_ansLen++;
          }
          UI_pos = 0;
          inputDone = 0;
          state = UI_Wait;
        }
      }
      break;
    case UI_Wait:
      if (gamemode == 0)
      {
        UI_pos = 0;
        inputDone = 0;
        state = UI_Idle;
      }
      else
      {
        if (UI_pos == UI_ansLen)
        {
          state = UI_Idle;
        }
        else
        {
          if (GetBit(PINC, 2) == 0x01)
          {
            UI_duration = 0;
            state = UI_Press;
          }
          else
          {
            state = UI_Wait;
          }
        }
      }
      break;
    case UI_Press:
      if (gamemode == 0)
      {
        UI_pos = 0;
        inputDone = 0;
        state = UI_Idle;
      }
      else
      {
        if (GetBit(PINC, 2) == 0x01)
        {
          state = UI_Press;
        }
        else
        {
          if (UI_duration <= 4)
          {
            userAnswer[UI_pos] = 1;
          }
          else
          {
            userAnswer[UI_pos] = 2;
          }
          UI_pos++;
          state = UI_Wait;
        }
      }
      break;
    default: break;
  }

  switch (state) // Actions
  {
    case UI_Idle: break;
    case UI_Wait: if (UI_pos == UI_ansLen) inputDone = 1; break;
    case UI_Press: UI_duration++; break;
    default: break;
  }

  return state;
}

int main(void) {
    //TODO: initialize all your inputs and ouputs (PORTB = output, PORTC = C0-C2 input, rest output, PORTD = output)
    DDRC    = 0xF8;
    PORTC   = 0x00;

    DDRB     = 0xFF;
    PORTB    = 0x00;

    DDRD   = 0xFF;
    PORTD  = 0x00;

    lcd_init();
    SPI_INIT();

    // ST7735 initialization
    // hardware reset (RESET pin)
    PORTB = SetBit(PORTB, 0, 0);
    _delay_ms(200);
    PORTB = SetBit(PORTB, 0, 1);
    _delay_ms(200);

    // set cs (ss) pin to 0
    PORTB = SetBit(PORTB, 2, 0);

    // SWRESET (Send_Command(SWRESET);)
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0
    SPI_SEND(0x01);
    _delay_ms(150);

    // SLPOUT (Send_Command(SLPOUT);)
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0
    SPI_SEND(0x11);
    _delay_ms(200);

    // COLMOD (Send_Command(COLMOD);)
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0
    SPI_SEND(0x3A);

    // color mode (Send_Data(0x06) //for 18 bit color mode)
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1
    SPI_SEND(0x06);
    _delay_ms(10);

    // DISPON (Send_Command(DISPON);)
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0
    SPI_SEND(0x29);
    _delay_ms(200);

    // set background color to black
    setBackground(0x00, 0x00, 0x00);


    //TODO: Initialize the passive buzzer(timer1)
    TCCR1A |= (1 << WGM11) | (1 << COM1A1); //COM1A1 sets it to channel A
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); //CS11 sets the prescaler to be 8
    //WGM11, WGM12, WGM13 set timer to fast pwm mode

    ICR1 = 0;

    OCR1A =  0;


    //TODO: Initialize tasks here
    // e.g. 
    // tasks[0].period = ;
    // tasks[0].state = ;
    // tasks[0].elapsedTime = ;
    // tasks[0].TickFct = ;

    // Passive Buzzer Task
    tasks[0].state = PB_Start;
    tasks[0].period = PB_PERIOD;
    tasks[0].elapsedTime = tasks[0].period;
    tasks[0].TickFct = &PB_Tick;

    // SPI Display Task
    tasks[1].state = SPI_Start;
    tasks[1].period = SPI_PERIOD;
    tasks[1].elapsedTime = tasks[1].period;
    tasks[1].TickFct = &SPI_Tick;

    // Gamemode Select Task
    tasks[2].state = G_Start;
    tasks[2].period = G_PERIOD;
    tasks[2].elapsedTime = tasks[2].period;
    tasks[2].TickFct = &G_Tick;

    // 16x2 LCD Screen Task
    tasks[3].state = LCD_Start;
    tasks[3].period = LCD_PERIOD;
    tasks[3].elapsedTime = tasks[3].period;
    tasks[3].TickFct = &LCD_Tick;

    // User Input Task
    tasks[4].state = UI_Start;
    tasks[4].period = UI_PERIOD;
    tasks[4].elapsedTime = tasks[4].period;
    tasks[4].TickFct = &UI_Tick;

    TimerSet(GCD_PERIOD);
    TimerOn();

    while (1) {}

    return 0;
}
