/*
 * lcd.h
 *
 * Class providing basic access to the LCD of the Chronos watch
 *
 * Currently, only initialization (in the constructor) and
 * clearing all display segments are implemented.
 */

 #ifndef LCD_H
 #define LCD_H
 
 #include <msp430.h>
 
 class LCD {
 public:
   // Constants to specify the LCD line for output methods
   enum class Line : bool {
     UPPER,
     LOWER
   };
 
 private:
   static LCD lcd; // Singleton instance
   static const unsigned char charset[]; // Character data for numbers and letters
 
   LCD(const LCD &copy); // Prevent copies
   LCD();
 
   // Hint: Memory range of the LCD controller (chapter 23 in slau259)
   enum { LCD_MEMORY_START = 0x0a00 + 0x20, /* LCDM1  */
          LCD_MEMORY_END   = 0x0a00 + 0x2d  /* LCDM14 */ };
 
 
   char *getSegmentAddress(unsigned int pos, Line line);
 
   char adjustForWiring(char value, Line line);
 
 public:
   // Get singleton instance
   static LCD& instance() {
     return lcd;
   }
 
   static constexpr unsigned int lineLength(Line line) {
     switch (line) {
       case Line::UPPER: return 4;
       case Line::LOWER: return 5;
     }
     return 0; // This return should never be reached
   }
 
   // Clear all display segments
   void clear();
 
   /*
    * Conventions for the methods you have to implement:
    *
    * "pos" legt die Position innerhalb einer Zeile fest, von links nach rechts:
    * "pos" designates a position in a line, counting from left to right, starting from 1.
    * The upper line has 4 positions:    1234
    * The lower line has 5 positions:   12345
    */
   
   // Show a single digit "digit"
   void show_digit(unsigned int digit, unsigned int pos, Line line = Line::LOWER);
   
   // Show a single character "letter"
   void show_char(const char letter, unsigned int pos, Line line = Line::UPPER);
   
   // Show an integer "number"
   void show_number(long int number, Line line = Line::LOWER);
   
   // Show the string "text"
   void show_string(const char *text, Line line = Line::UPPER);
 
   // Show decimal point 1
   void show_DP1() {
     LCDM9 |= 0b10000000; // DP1 maps to SEG17, COM0
   }
 
   // Clear decimal point 1
   void clear_DP1() {
     LCDM9 &= ~0b10000000;
   }
 
   // Show decimal point 2
   void show_DP2() { LCDM5 |= 0b00000100; } // DP2 maps to SEG8, COM1 (wrong)
 
   // Clear decimal point 2
   void clear_DP2() { LCDM5 &= ~0b00000100; }
 
   // Show decimal point 3
   void show_DP3() { LCDM1 |= 0b00100000; } // DP3 maps to SEG1, COM1
 
   // Clear decimal point 3
   void clear_DP3() { LCDM1 &= ~0b00100000; }
 
   // Show degree symbol
   void show_DEG() { LCDM5 |= 0b00000010; } // DEG maps to SEG8, COM2 (wrong)
 
   // Clear degree symbol
   void clear_DEG() { LCDM5 &= ~0b00000010; }
 };
 
 #endif
 