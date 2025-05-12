/*
 * lcd.cc
 *
 * Class providing basic access to the LCD of the Chronos watch
 *
 * Currently, only initialization (in the constructor) and
 * clearing all display segments are implemented.
 */

 #include <msp430.h>
 #include "machine/lcd.h"
 
 LCD LCD::lcd; // Singleton instance
 
 LCD::LCD() {
   // Clear display memory
   LCDBMEMCTL |= LCDCLRBM | LCDCLRM;
 
   // LCD_FREQ = ACLK/16/8 = 256Hz
   // Frame frequency = 256Hz/4 = 64Hz, LCD mux 4, LCD on
   LCDBCTL0 = LCDDIV_15 | LCDPRE__8 | LCD4MUX | LCDON;
 
   // Blink frequency = ACLK/8/4096 = 1Hz
   LCDBBLKCTL = LCDBLKPRE0 | LCDBLKPRE1 |
                LCDBLKDIV0 | LCDBLKDIV1 | LCDBLKDIV2 | LCDBLKMOD0;
 
   // I/O to COM outputs
   P5SEL |= (BIT5 | BIT6 | BIT7);
   P5DIR |= (BIT5 | BIT6 | BIT7);
 
   // Activate LCD output
   LCDBPCTL0 = 0xFFFF;
   LCDBPCTL1 = 0xFF;
 }
 
 void LCD::clear() { LCDBMEMCTL |= LCDCLRBM | LCDCLRM; }
 
 // You code goes here. For example:
 char *LCD::getSegmentAddress(unsigned int pos, Line line) {
   int offset = pos;
   switch (line) {
     case Line::UPPER:
       if (pos == 4) { ++offset; }
       break;
     case Line::LOWER:
       offset = 12 - offset;
       break;
   }
   return (char *)LCD_MEMORY_START + offset;
 }
 
 char LCD::adjustForWiring(char value, Line line)
 {
   if (line == Line::LOWER) {
     // Swap nibbles for the lower line
     return ((value & 0x0f) << 4) | ((value & 0xf0) >> 4);
   }
   return value;
 }
 
 // Show a single digit "digit"
 void LCD::show_digit(unsigned int digit, unsigned int pos, Line line)
 {
   char *lcdpos = getSegmentAddress(pos, line);
   *lcdpos = adjustForWiring(charset[digit + '0'], line);
 }
 
 // Show a single character "letter"
 void LCD::show_char(const char letter, unsigned int pos, Line line)
 {
   char *lcdpos = getSegmentAddress(pos, line);
   *lcdpos = adjustForWiring(charset[(int)letter], line);
 }
 
 // Show an integer
 void LCD::show_number(long number, Line line) {
   bool negative = number < 0;
   int endPosition = (negative) ? 1 : 0;
   if (number < 0) { number = -number; }
   int position = lineLength(line);
 
   while (position > endPosition) {
     int digit = number % 10;
     number /= 10;
     show_digit(digit, position, line);
     --position;
   }
   if (negative) {
     show_char('-', 1, line);
   }
 }
 
 // Show the string "text"
 void LCD::show_string(const char *text, Line line) {
   const int lineLen = lineLength(line);
   for (int pos = 0; text[pos] != 0 && pos < lineLen; ++pos) {
     show_char(text[pos], pos+1, line);
   }
 }
 
 