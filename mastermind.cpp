#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <Arduino.h>
#include "mastermind.h"
#include "lcd_wrapper.h"

void play_game(char *secret)
{
  int peg_a = 0, peg_b = 0;
  int entry_nr = 0;
  char guess[4];

  char **history;
  history = (char **)calloc(10, sizeof(char));

  bool bEnter_pressed = false;
  bool b1_pressed = false;
  bool b2_pressed = false;
  bool b3_pressed = false;
  bool b4_pressed = false;

  for (int idx = 0; idx < 4; idx++)
  {
    guess[idx] = '0';
  }

  for (int row = 0; row < 10; row++)
  {
    history[row] = (char *)calloc(4, sizeof(char));
  }

  turn_off_leds();
  lcd_clear();
  lcd_print("Welcome to game");
  lcd_print_at(0, 1, "of mastermind!");
  delay(1000);
  lcd_clear();
  lcd_print("I am thinking a");
  lcd_print_at(0, 1, "number ...");
  delay(1500);
  lcd_clear();

  lcd_print("Your guess: ");
  lcd_print_at(12, 0, guess);

  for (int idx = 0; idx < 10; idx++)
  {
    int Enter_state;
    peg_a = peg_b = 0;
    entry_nr = idx - 1;
    while (true)
    {
      Enter_state = digitalRead(BTN_ENTER_PIN);
      int b1_state = digitalRead(BTN_1_PIN);
      int b2_state = digitalRead(BTN_2_PIN);
      int b3_state = digitalRead(BTN_3_PIN);
      int b4_state = digitalRead(BTN_4_PIN);

      if (b1_state == HIGH && b1_pressed == true && b2_state == HIGH && b2_pressed == false)
      {
        if (entry_nr != 0)
        {
          entry_nr = entry_nr - 1;
          render_history(secret, history, entry_nr);
        }
        b1_pressed = true;
        b2_pressed = true;
      }
      if (b1_state == HIGH && b1_pressed == true && b3_state == HIGH && b3_pressed == false)
      {
        if (entry_nr != idx - 1)
        {
          entry_nr = entry_nr + 1;
          render_history(secret, history, entry_nr);
        }
        b1_pressed = true;
        b3_pressed = true;
      }

      if (b1_state == HIGH && b1_pressed == false)
      {
        guess[0] = guess[0] + 1;
        if (guess[0] >= 58)
          guess[0] = '0';
        lcd_clear();
        char num[] = {(idx+1) + '0', '\0'};
        lcd_print("Guessing ");
        lcd_print_at(9, 0 , num);
        lcd_print_at(10, 0 ,":");
        lcd_print_at(12, 0, guess);
        b1_pressed = true;
      }
      if (b1_state == LOW && b1_pressed == true)
      {
        b1_pressed = false;
      }

      if (b2_state == HIGH && b2_pressed == false)
      {
        guess[1] = guess[1] + 1;
        if (guess[1] >= 58)
          guess[1] = '0';
        lcd_clear();
        char num[] = {(idx+1) + '0', '\0'};
         lcd_print("Guessing ");
        lcd_print_at(9, 0 , num);
        lcd_print_at(10, 0 ,":");
        lcd_print_at(12, 0, guess);
        b2_pressed = true;
      }
      if (b2_state == LOW && b2_pressed == true)
      {
        b2_pressed = false;
      }

      if (b3_state == HIGH && b3_pressed == false)
      {
        guess[2] = guess[2] + 1;
        if (guess[2] >= 58)
          guess[2] = '0';
        lcd_clear();
        char num[] = {(idx+1) + '0', '\0'};
         lcd_print("Guessing ");
        lcd_print_at(9, 0 , num);
        lcd_print_at(10, 0 ,":");
        lcd_print_at(12, 0, guess);
        b3_pressed = true;
      }
      if (b3_state == LOW && b3_pressed == true)
      {
        b3_pressed = false;
      }

      if (b4_state == HIGH && b4_pressed == false)
      {
        guess[3] = guess[3] + 1;
        if (guess[3] >= 58)
          guess[3] = '0';
        lcd_clear();
        char num[] = {(idx+1) + '0', '\0'};
        lcd_print("Guessing ");
        lcd_print_at(9, 0 , num);
        lcd_print_at(10, 0 ,":");
        lcd_print_at(12, 0, guess);
        b4_pressed = true;
      }
      if (b4_state == LOW && b4_pressed == true)
      {
        b4_pressed = false;
      }
      if (Enter_state == HIGH && bEnter_pressed == false)
      {
        bEnter_pressed = true;
      }
      if (Enter_state == LOW && bEnter_pressed == true)
      {
        bEnter_pressed = false;
        lcd_clear();
        break;
      }
    }
    int secret_length = strlen(secret);
    for (int idx2 = 0; idx2 < secret_length; idx2++)
    {
      history[idx][idx2] = guess[idx2];
    }

    get_score(secret, guess, &peg_a, &peg_b);
    turn_off_leds();
    render_leds(peg_a, peg_b);
    lcd_clear();
    lcd_print("Guess ");
    char num[] = {(idx+1) + '0', '\0'};
    lcd_print_at(5, 0, num);
    lcd_print_at(6, 0, ": ");
    lcd_print_at(12, 0, guess);

    if (peg_a == 4)
    {
      lcd_clear();
      lcd_print("YOU WON!");
      turn_off_leds();
      for (int i = 0; i < 4; i++)
      {
        digitalWrite(LED_BLUE_1, HIGH);
        digitalWrite(LED_BLUE_2, HIGH);
        digitalWrite(LED_BLUE_3, HIGH);
        digitalWrite(LED_BLUE_4, HIGH);
        delay(500);
        digitalWrite(LED_BLUE_1, LOW);
        digitalWrite(LED_BLUE_2, LOW);
        digitalWrite(LED_BLUE_3, LOW);
        digitalWrite(LED_BLUE_4, LOW);
        delay(500);
      }
      return;
    }
  }
  lcd_clear();
  lcd_print("You have lost!");
  delay(2000);
  lcd_clear();
  lcd_print("My number was:");
  lcd_print_at(0, 1, secret);
  turn_off_leds();
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(LED_RED_1, HIGH);
    digitalWrite(LED_RED_2, HIGH);
    digitalWrite(LED_RED_3, HIGH);
    digitalWrite(LED_RED_4, HIGH);
    delay(500);
    digitalWrite(LED_RED_1, LOW);
    digitalWrite(LED_RED_2, LOW);
    digitalWrite(LED_RED_3, LOW);
    digitalWrite(LED_RED_4, LOW);
  }
}

char *generate_code(bool repeat, int length)
{
  randomSeed(analogRead(12));
  if (length < 1)
    return NULL;
  srand(time(NULL));

  char *code;
  code = (char *)calloc(length + 1, sizeof(int));

  for (int i = 0; i < length; i++)
  {
    code[i] = random(10) + '0';
    if (repeat == false)
    {
      for (int j = 0; j < i; j++)
      {
        if (code[i] == code[j])
        {
          while (code[i] == code[j])
          {
            code[i] = random(10) + '0';
          }
          j = 0;
        }
      }
    }
  }
  return code;
}

void get_score(const char *secret, const char *guess, int *peg_a, int *peg_b)
{
  int secret_length = strlen(secret);
  char temp[secret_length];

  for (int score_idx = 0; score_idx < secret_length; score_idx++)
  {
    temp[score_idx] = secret[score_idx];
  }

  for (int i = 0; i < secret_length; i++)
  {
    if (temp[i] == guess[i])
    {
      *peg_a = *peg_a + 1;
      //temp[i] = 'A';
    }
  }
  for (int i = 0; i < secret_length; i++)
  {
    for (int j = 0; j < secret_length; j++)
    {

      if (temp[i] == guess[j])
      {
        *peg_b = *peg_b + 1;
        //temp[i] = 'B';
      }
    }
  }
}

void render_leds(const int peg_a, const int peg_b)
{
  for (int i = 0, blue = 0; i < peg_a; i++)
  {
    digitalWrite(LED_BLUE_1 + blue, HIGH);
    blue = blue + 2;
  }
  for (int j = 0, red = 0; j < peg_b; j++)
  {
    digitalWrite(LED_RED_1 + red, HIGH);
    red = red + 2;
  }
}

void turn_off_leds()
{
  digitalWrite(LED_BLUE_1, LOW);
  digitalWrite(LED_RED_1, LOW);
  digitalWrite(LED_BLUE_2, LOW);
  digitalWrite(LED_RED_2, LOW);
  digitalWrite(LED_BLUE_3, LOW);
  digitalWrite(LED_RED_3, LOW);
  digitalWrite(LED_BLUE_4, LOW);
  digitalWrite(LED_RED_4, LOW);
}

void render_history(char *secret, char **history, const int entry_nr)
{
  int secret_length = strlen(secret);
  char temp[secret_length];
  int peg_a = 0;
  int peg_b = 0;
  char entry_nbr[] = {(entry_nr+1) + '0', '\0'};
  lcd_clear();
  lcd_print("history");
  lcd_print_at(7, 0, entry_nbr);
  lcd_print_at(8, 0, ":");
  lcd_print_at(12, 0, history[entry_nr]);

  for (int his_idx = 0; his_idx < secret_length; his_idx++)
  {
    temp[his_idx] = history[entry_nr][his_idx];
  }
  get_score(secret, temp, &peg_a, &peg_b);
  turn_off_leds();
  render_leds(peg_a, peg_b);
}
