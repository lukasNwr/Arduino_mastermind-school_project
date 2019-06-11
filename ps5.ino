#include "mastermind.h"
#include "lcd_wrapper.h";

#define LENGTH 4
//LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);


void setup() {
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  lcd_init();
}

void loop() {
  bool repeat = false;
  char* code = generate_code(repeat , LENGTH);
  lcd_print(code); 
  delay(1000);
  lcd_clear();
  play_game(code);
  free(code);
  delay(4000);
}
