#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// put function declarations here:
const int Led = 12;
const int numOfinput = 4;
const int InputPins[numOfinput] = {2, 3, 4, 5};
int InputState[numOfinput];
int lastInputState[numOfinput] = {LOW, LOW, LOW, LOW};
bool inputFlags[numOfinput] = {LOW, LOW, LOW, LOW};
long DebounceTime[numOfinput] = {0, 0, 0, 0};
long DebounceDelay = 10;

/// Lcd Menu
const byte numOfScreens = 2;
int currentScreen = 0;
String Screen[numOfScreens][2] = {{"Menu de Project", "  "}, {"Led1", "Perfect"}};
String state;

void setup()
{
  Serial.begin(115200);

  for (int i = 0; i < numOfinput; i++)
  {

    pinMode(InputPins[i], INPUT);
    digitalWrite(InputPins[i], HIGH);
  }
  pinMode(Led, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  setInputFlags();
  resolveInputFlags();
}
void resolveInputFlags()
{
  for (int i = 0; i < numOfinput; i++)
  {
    if (inputFlags[i] == LOW)
    {
      inputActions(i);
      inputFlags[i] = HIGH;
      printScreen(i);
    }
  }
}
void inputActions(int input)
{
  if (input == 0)
  {
    if (currentScreen == 0)
    {
      currentScreen = numOfScreens - 1;
    }
    else
    {
      currentScreen--;
    }
  }
  else if (input == 1)
  {
    if (currentScreen == numOfScreens - 1)
    {
      currentScreen = 0;
    }
    else
    {
      currentScreen++;
    }
  }
  else if (input == 2)
  {
    blinkLed(0);
    // ON
  }
  else if (input == 3)
  {
    blinkLed(1);
    // OFF
  }
}
void blinkLed(int key)
{
  if (key == 0 && currentScreen == 1)
  {
    state = "Led OFF";
    digitalWrite(Led, LOW);
  }
  else if (key == 1 && currentScreen == 1)
  {
    state = "LED ON";
    digitalWrite(Led, HIGH);
  }
}
void printScreen(int input)
{
  // My Solution :)
  /* lcd.clear();
  lcd.print(Screen[currentScreen][0]);
  if (currentScreen == 0 && (input == 2 || input ==3))
  {
    lcd.clear();
    lcd.print(Screen[0][0]);
  }
  else if (currentScreen == 1 && (input == 2 || input ==3) )
  {
    lcd.clear();
    lcd.print(Screen[1][0]);
    lcd.setCursor(0,1);
    lcd.print(state);

  }*/
  lcd.clear();
  lcd.print(Screen[currentScreen][0]);
  if (currentScreen == 1)
  {
    lcd.clear();
    lcd.print(Screen[currentScreen][0]);
    lcd.setCursor(0, 1);
    lcd.print(state);
  }
}

void setInputFlags()
{
  for (int i = 0; i < numOfinput; i++)
  {
    int reading = digitalRead(InputPins[i]);
    if (reading != lastInputState[i])
    {
      DebounceTime[i] = millis();
    }
    if (millis() - DebounceTime[i] > DebounceDelay)
    {
      if (reading != InputState[i])
      {
        InputState[i] = reading;
        if (InputState[i] == LOW)
        {
          inputFlags[i] = LOW;
        }
      }
    }
    lastInputState[i] = reading;
  }
}
