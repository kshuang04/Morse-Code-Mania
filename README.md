# Morse Code Mania

[![Embedded C](https://img.shields.io/badge/Language-Embedded%20C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Hardware](https://img.shields.io/badge/Microcontroller-ATmega328P%2FArduino%20Uno-red.svg)](https://www.microchip.com/)

An embedded microcontroller-based arcade and learning game designed for the ATmega328P / Elegoo UNO R3 platform. **Morse Code Mania** features dual display outputs, custom graphics scaling, handshaked multi-tasking state machines, interactive sound, and multiple gameplay modes to teach and test Morse code proficiency in real time.

---

## Project Overview

Morse Code Mania offers two interactive gameplay modes:

*   **Training Mode:** Designed for learning. Players progress through the alphabet sequentially with visual cues on-screen displaying both the letter and its corresponding Morse code sequence.
*   **Endless Mode:** An arcade-style survival mode where players are presented with random letters and a shrinking timer. Players start with 3 lives and earn points for correct inputs. The input window progressively shortens as the score increases.

---

## Key Features

*   **Dual Display System:** Simultaneous rendering using a 1.44" SPI Color LCD for primary graphics and a 16x2 Character LCD for HUD stats/textual feedback.
*   **State-Driven Differential UI Updates:** Prevents visual lag by tracking state transitions and only redrawing modified screen regions.
*   **Concurrent Multi-Tasking:** Non-blocking state machines utilizing task handshaking between the **Game Task** and **User Input Task**.
*   **Dynamic Audio Feedback:** Integrated background music running on a loop using a passive buzzer, featuring dynamic interrupts for auditory feedback on correct/incorrect inputs.
*   **Precise Timing Input Engine:** Distinguishes between short presses (dots $\le 200\text{ ms}$) and long presses (dashes $> 200\text{ ms}$).

---

## Hardware Components

| Category | Component | Description / Details |
| :--- | :--- | :--- |
| **Microcontroller** | Elegoo UNO R3 | ATmega328P based development board |
| **Primary Display** | 1.44" SPI Color LCD | ST7735 controller (SPI Interface) |
| **Secondary Display** | 16x2 Character LCD | Parallel interface with potentiometer contrast adjustment |
| **Audio** | Passive Buzzer | Plays dynamic frequencies for background music & sound effects |
| **Controls** | 3x Push Buttons | Color-coded inputs (Red, Blue, Black) |
| **Peripherals** | Potentiometer | Used for 16x2 LCD contrast tuning |

---

## Software & Libraries

*   **Embedded C / Custom Synchronous State Machines (SynchSM)**
*   **`stdlib.h`:** `rand()` for dynamic letter generation in Endless Mode.
*   **`stdio.h`:** `snprintf()` for string formatting and numeric-to-string conversions on the LCD screens.

---

## System Architecture & Complexity

### 1. 1.44" SPI Color LCD
Custom 8x8 bitmap assets (alphanumeric characters, heart icons, dot/dash symbols) scaled dynamically to fit the screen. Employs memory-efficient local state tracking variables to avoid constant SPI writes.

### 2. 16x2 Character LCD
Handles menu instructions, high scores, textual feedback, and player HUD info. Frame-rate optimization techniques ensure that slow character LCD write cycles do not block real-time game ticks.

### 3. Audio & Music Interrupt System
A sound task runs a looped background melody over a passive buzzer. Input validation triggers audio interrupts that replace the music task with audio cues (success/fail tones), resuming the main melody exactly where it left off.

### 4. Input Processing & Task Handshaking
The architecture separates hardware timing from game logic using a strict **Handshaking Protocol**:
*   The **Game Task** signals readiness for user action.
*   The **User Input Task** samples button state/duration (detecting $\le 200\text{ ms}$ for dots and $> 200\text{ ms}$ for dashes).
*   Upon input completion, a completion flag releases the **Game Task** to evaluate correctness.
