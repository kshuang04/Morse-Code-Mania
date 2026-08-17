/*        Your Name & E-mail: Sky Huang (shuan284@ucr.edu)

 *         Discussion Section: 021 (Wed 2pm)

 *         Assignment: Custom Lab Final Demo

 *         Exercise Description: Morse Code Mania

 *        

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link: https://www.youtube.com/watch?v=a2vPrjBrxrs

 */

#ifndef MISC_H
#define MISC_H

#include "spiAVR.h"
#include "helper.h"

// letters array
const unsigned char alphabet[][8] = {
    // 'A' (Index 0)
    {0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66}, 
    // 'B' (Index 1)
    {0x7C, 0x7E, 0x66, 0x7C, 0x7C, 0x66, 0x7E, 0x7C},
    // 'C' (Index 2)
    {0x3E, 0x7E, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x3E},
    // 'D' (Index 3)
    {0x7C, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7C},
    // 'E' (Index 4)
    {0x7E, 0x7E, 0x60, 0x7E, 0x7E, 0x60, 0x7E, 0x7E},
    // 'F' (Index 5)
    {0x7E, 0x7E, 0x60, 0x78, 0x78, 0x60, 0x60, 0x60},
    // 'G' (Index 6)
    {0x3E, 0x7E, 0x60, 0x6E, 0x6E, 0x66, 0x7E, 0x3C},
    // 'H' (Index 7)
    {0x66, 0x66, 0x66, 0x7E, 0x7E, 0x66, 0x66, 0x66},
    // 'I' (Index 8)
    {0x7E, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x7E},
    // 'J' (Index 9)
    {0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x7E, 0x3C},
    // 'K' (Index 10)
    {0x66, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x66},
    // 'L' (Index 11)
    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x7E},
    // 'M' (Index 12)
    {0xC3, 0xE7, 0xE7, 0xDB, 0xDB, 0xC3, 0xC3, 0xC3},
    // 'N' (Index 13)
    {0x66, 0x66, 0x76, 0x7E, 0x6E, 0x66, 0x66, 0x66},
    // 'O' (Index 14)
    {0x3C, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x3C},
    // 'P' (Index 15)
    {0x7C, 0x7E, 0x66, 0x7E, 0x7C, 0x60, 0x60, 0x60},
    // 'Q' (Index 16)
    {0x3C, 0x7E, 0x66, 0x66, 0x66, 0x6E, 0x7E, 0x3F},
    // 'R' (Index 17)
    {0x7C, 0x7E, 0x66, 0x7E, 0x7C, 0x6C, 0x66, 0x66},
    // 'S' (Index 18)
    {0x7E, 0x7E, 0x60, 0x7E, 0x7E, 0x06, 0x7E, 0x7E},
    // 'T' (Index 19)
    {0x7E, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
    // 'U' (Index 20)
    {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E, 0x7E},
    // 'V' (Index 21)
    {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x3C, 0x18},
    // 'W' (Index 22)
    {0xC3, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xE7, 0xE7},
    // 'X' (Index 23)
    {0x66, 0x66, 0x66, 0x18, 0x18, 0x66, 0x66, 0x66},
    // 'Y' (Index 24)
    {0x66, 0x66, 0x66, 0x7E, 0x3C, 0x18, 0x18, 0x18},
    // 'Z' (Index 25)
    {0x7E, 0x7E, 0x0E, 0x1C, 0x38, 0x70, 0x7E, 0x7E},
};

// morse code
const unsigned char code[][4] = { // 1 - dot, 2 - dash, 0 - unsused/extra placeholder
    // A (.-)
    {1, 2, 0, 0},
    // B (-...)
    {2, 1, 1, 1},
    // C (-.-.)
    {2, 1, 2, 1},
    // D (-..)
    {2, 1, 1, 0},
    // E (.)
    {1, 0, 0, 0},
    // F (..-.)
    {1, 1, 2, 1},
    // G (--.)
    {2, 2, 1, 0},
    // H (....)
    {1, 1, 1, 1},
    // I (..)
    {1, 1, 0, 0},
    // J (.---)
    {1, 2, 2, 2},
    // K (-.-)
    {2, 1, 2, 0},
    // L (.-..)
    {1, 2, 1, 1},
    // M (--)
    {2, 2, 0, 0},
    // N (-.)
    {2, 1, 0, 0},
    // O (---)
    {2, 2, 2, 0},
    // P (.--.)
    {1, 2, 2, 1},
    // Q (--.-)
    {2, 2, 1, 2},
    // R (.-.)
    {1, 2, 1, 0},
    // S {...}
    {1, 1, 1, 0},
    // T (-)
    {2, 0, 0, 0},
    // U (..-)
    {1, 1, 2, 0},
    // V (...-)
    {1, 1, 1, 2},
    // W (.--)
    {1, 2, 2, 0},
    // X (-..-)
    {2, 1, 1, 2},
    // Y (-.--)
    {2, 1, 2, 2},
    // Z (--..)
    {2, 2, 1, 1},
};

// numbers array
const unsigned char numbers[][8] = {
    // 0
    {0x7E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7E},
    // 1
    {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E},
    // 2
    {0x7E, 0x06, 0x06, 0x7E, 0x60, 0x60, 0x60, 0x7E},
    // 3
    {0x7E, 0x06, 0x06, 0x3E, 0x06, 0x06, 0x06, 0x7E},
    // 4
    {0x66, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x06, 0x06},
    // 5
    {0x7E, 0x60, 0x60, 0x7E, 0x06, 0x06, 0x06, 0x7E},
    // 6
    {0x7E, 0x60, 0x60, 0x7E, 0x66, 0x66, 0x66, 0x7E},
    // 7
    {0x7E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
    // 8
    {0x7E, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x7E},
    // 9
    {0x7E, 0x66, 0x66, 0x7E, 0x06, 0x06, 0x06, 0x7E},
};

void displayCode(unsigned char letterIdx)
{
    unsigned char xStart = 30;
    unsigned char yStart = 25;
    unsigned char yEnd   = 30;
    unsigned char boxWidth = 12;
    unsigned char spacing = 20;

    for (unsigned char i = 0; i < 4; i++)
    {
        unsigned char currentSymbol = code[letterIdx][i];
        
        // get current box's boundaries
        unsigned char boxXStart = xStart + (i * spacing);
        unsigned char boxXEnd   = boxXStart + (boxWidth - 1);

        // set drawing region
        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2A); // CASET
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
        SPI_SEND(0x00); SPI_SEND(boxXStart); 
        SPI_SEND(0x00); SPI_SEND(boxXEnd);

        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2B); // RASET
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
        SPI_SEND(0x00); SPI_SEND(yStart); 
        SPI_SEND(0x00); SPI_SEND(yEnd);

        // draw
        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2C); // RAMWR
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)

        for (unsigned char row = yStart; row <= yEnd; row++)
        {
            for (unsigned char col = boxXStart; col <= boxXEnd; col++)
            {
                unsigned char currX = col - boxXStart;

                if (currentSymbol == 1) // dot
                {
                    if (currX >= 3 && currX <= 8)
                    {
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                    }
                    else
                    {
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                    }
                }
                else if (currentSymbol == 2) // dash
                {
                    SPI_SEND(0xFF);
                    SPI_SEND(0xFF);
                    SPI_SEND(0xFF);
                }
                else // empty
                {
                    SPI_SEND(0x00);
                    SPI_SEND(0x00);
                    SPI_SEND(0x00);
                }
            }
        }
    }
}

void displayLetter(unsigned char letterIdx)
{
    unsigned char scale = 3;

    // set drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    // pixel 52-75
    SPI_SEND(0x00); SPI_SEND(0x34); 
    SPI_SEND(0x00); SPI_SEND(0x4B);

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    // pixel 52-75
    SPI_SEND(0x00); SPI_SEND(0x34); 
    SPI_SEND(0x00); SPI_SEND(0x4B);

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)

    for (unsigned char row = 0; row < 8; row++)
    {
        for (unsigned char rRep = 0; rRep < scale; rRep++)
        {
            unsigned char currRow = alphabet[letterIdx][row];

            for (char bit = 7; bit >= 0; bit--)
            {
                unsigned char currBit = GetBit(currRow, bit);

                for (unsigned char cRep = 0; cRep < scale; cRep++)
                {
                    if (currBit)
                    {
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                    }
                    else
                    {
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                    }
                }
            }
        }
    }
}

void setBackground(unsigned char red, unsigned char green, unsigned char blue) {
    // set drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(0x00);
    SPI_SEND(0x00); SPI_SEND(0x81); // +2 pixels extra

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(0x00);
    SPI_SEND(0x00); SPI_SEND(0x80); // +1 pixel extra

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    for (unsigned int i = 0; i < 16770; i++) { // 130 x 128 = 16770
        SPI_SEND(red);
        SPI_SEND(green);
        SPI_SEND(blue);
    }
}

void displayMCM()
{
    unsigned char scale = 3;

    // clear screen
    setBackground(0x00, 0x00, 0x00);

    // M
    // drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(26);
    SPI_SEND(0x00); SPI_SEND(49);

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(0x34); 
    SPI_SEND(0x00); SPI_SEND(0x4B);

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    for (unsigned char row = 0; row < 8; row++)
    {
        for (unsigned char rRep = 0; rRep < scale; rRep++)
        {
            unsigned char currRow = alphabet[12][row];

            for (char bit = 7; bit >= 0; bit--)
            {
                unsigned char currBit = GetBit(currRow, bit);

                for (unsigned char cRep = 0; cRep < scale; cRep++)
                {
                    if (currBit)
                    {
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                    }
                    else
                    {
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                    }
                }
            }
        }
    }

    // C
    displayLetter(2);

    // M
    // drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(78);
    SPI_SEND(0x00); SPI_SEND(101);

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(0x34); 
    SPI_SEND(0x00); SPI_SEND(0x4B);

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    for (unsigned char row = 0; row < 8; row++)
    {
        for (unsigned char rRep = 0; rRep < scale; rRep++)
        {
            unsigned char currRow = alphabet[12][row];

            for (char bit = 7; bit >= 0; bit--)
            {
                unsigned char currBit = GetBit(currRow, bit);

                for (unsigned char cRep = 0; cRep < scale; cRep++)
                {
                    if (currBit)
                    {
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                        SPI_SEND(0xFF);
                    }
                    else
                    {
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                    }
                }
            }
        }
    }
}

void displayHearts(unsigned char lives)
{
    unsigned char heart[8] = {0x66, 0xFF, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18};

    unsigned char scale = 2;

    unsigned char xStart = 4;
    unsigned char xEnd = 55;
    unsigned char yStart = 104;
    unsigned char yEnd = 119;

    // clear area
    // set drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    // pixel 4-55
    SPI_SEND(0x00); SPI_SEND(xStart); 
    SPI_SEND(0x00); SPI_SEND(xEnd);

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    // pixel 104-119
    SPI_SEND(0x00); SPI_SEND(yStart); 
    SPI_SEND(0x00); SPI_SEND(yEnd);

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)

    for (unsigned int i = 0; i < 832; i++) // 52 x 16 = 832
    {
        SPI_SEND(0x00);
        SPI_SEND(0x00);
        SPI_SEND(0x00);
    }


    // draw hearts
    for (unsigned char h = 0; h < lives; h++)
    {
        unsigned char heartXStart = xStart + (h * 18);
        unsigned char heartXEnd = heartXStart + 15;

        // set drawing region
        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2A); // CASET
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
        SPI_SEND(0x00); SPI_SEND(heartXStart); 
        SPI_SEND(0x00); SPI_SEND(heartXEnd);

        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2B); // RASET
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
        SPI_SEND(0x00); SPI_SEND(yStart); 
        SPI_SEND(0x00); SPI_SEND(yEnd);

        // draw
        PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
        SPI_SEND(0x2C); // RAMWR
        PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)

        for (unsigned char row = 0; row < 8; row++)
        {
            for (unsigned char rRep = 0; rRep < scale; rRep++)
            {
                unsigned char currRow = heart[row];

                for (char bit = 7; bit >= 0; bit--)
                {
                    unsigned char currBit = GetBit(currRow, bit);

                    for (unsigned char cRep = 0; cRep < scale; cRep++)
                    {
                        if (currBit && h < lives)
                        {
                            SPI_SEND(0x00);
                            SPI_SEND(0x00);
                            SPI_SEND(0xFF);
                        }
                        else
                        {
                            SPI_SEND(0x00);
                            SPI_SEND(0x00);
                            SPI_SEND(0x00);
                        }
                    }
                }
            }
        }
    }
}

void displayTime(unsigned char seconds)
{
    unsigned char scale = 2;
    unsigned char xStart = 100;
    unsigned char xEnd = 115;
    unsigned char yStart = 104;
    unsigned char yEnd = 119;

    // clear area
    // set drawing region
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2A); // CASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(xStart); 
    SPI_SEND(0x00); SPI_SEND(xEnd);

    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2B); // RASET
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)
    SPI_SEND(0x00); SPI_SEND(yStart); 
    SPI_SEND(0x00); SPI_SEND(yEnd);

    // draw
    PORTC = SetBit(PORTC, 5, 0); // A0 = 0 (command)
    SPI_SEND(0x2C); // RAMWR
    PORTC = SetBit(PORTC, 5, 1); // A0 = 1 (data)

    for (unsigned char row = 0; row < 8; row++)
    {
        for (unsigned char rRep = 0; rRep < scale; rRep++)
        {
            unsigned char currRow = numbers[seconds][row];

            for (char bit = 7; bit >= 0; bit--)
            {
                unsigned char currBit = GetBit(currRow, bit);

                for (unsigned char cRep = 0; cRep < scale; cRep++)
                {
                    if (currBit)
                    {
                        if (seconds <= 3)
                        {
                            SPI_SEND(0x00);
                            SPI_SEND(0x00);
                            SPI_SEND(0xFF);
                        }
                        else
                        {
                            SPI_SEND(0xFF);
                            SPI_SEND(0xFF);
                            SPI_SEND(0xFF);
                        }
                    }
                    else
                    {
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                        SPI_SEND(0x00);
                    }
                }
            }
        }
    }
}

#endif