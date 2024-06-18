#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <DHT.h>

// Define pins for ILI9341
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_MISO 12

// Define pin and type for DHT22
#define DHTPIN 4
#define DHTTYPE DHT22

// Initialize TFT and touch screen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_FT6206 touch = Adafruit_FT6206();
DHT dht(DHTPIN, DHTTYPE);

unsigned long TimePrevious = 0;
float previousTemp = NAN;
float calibrationOffset = 0.0;
int verif1 = 0;

void setup()
{
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  dht.begin();

  // Initialize the touch screen
  if (!touch.begin())
  {
    Serial.println("Unable to start the touch screen.");
  }
  else
  {
    Serial.println("Touch screen started.");
  }

  TimePrevious = millis();
  displayStaticText();
}

void loop()
{
  // Only update the temperature every 2 seconds
  if (millis() - TimePrevious >= 2000)
  {
    displayTemperature();
    TimePrevious = millis();
  }

  // Check for touch input
  checkTouch();
}

void displayStaticText()
{
  tft.setCursor(50, 50);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Temp: ");

  // Draw "+" button
  tft.fillRect(160, 70, 40, 50, ILI9341_GREEN);
  tft.setCursor(163, 87);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2);
  tft.print(" + ");

  // Draw "-" button
  tft.fillRect(210, 70, 40, 50, ILI9341_RED);
  tft.setCursor(213, 87);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.print(" - ");

  // Draw "Reset" button
  tft.fillRect(260, 70, 60, 50, ILI9341_BLUE);
  tft.setCursor(263, 87);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("RST");

  // Display the initial value of verif1
  displayVerif();
}

void displayTemperature()
{
  // Read the temperature from the DHT sensor
  float TempCourant = dht.readTemperature();

  // Add the calibration offset to the temperature
  float TempEdit = TempCourant + calibrationOffset;

  // Update the display only if the temperature has changed
  if (TempEdit != previousTemp)
  {
    // Clear the previous temperature display
    tft.fillRect(50, 80, 100, 30, ILI9341_BLACK);

    // Set the cursor position and text properties
    tft.setCursor(50, 80);
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

    // Print the new temperature to the TFT screen
    tft.print(TempEdit);
    tft.print(" C");

    // Optionally, print to Serial for debugging
    Serial.print("Temperature: ");
    Serial.print(TempEdit);
    Serial.println(" C");

    // Update the previous temperature
    previousTemp = TempEdit;
  }
}

void displayVerif()
{
  // Clear the previous verif1 display
  tft.fillRect(50, 110, 100, 30, ILI9341_BLACK);

  // Set the cursor position and text properties
  tft.setCursor(50, 110);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

  // Print the current value of verif1 to the TFT screen
  tft.print("Verif: ");
  tft.print(verif1);
}

void checkTouch()
{
  if (touch.touched())
  {
    TS_Point p = touch.getPoint();

    // Correctly map the touch coordinates to the display coordinates
    int x = map(p.y, 0, 320, 320, 0);
    int y = p.x;

    handleTouch(x, y);
  }
}

void handleTouch(int x, int y)
{
  bool touched = false;
  // Check if the "+" button is pressed
  if (x > 160 && x < 200 && y > 70 && y < 120)
  {
    calibrationOffset += 0.2;
    verif1 += 1;
    Serial.print("Calibration Offset: ");
    Serial.print(calibrationOffset);
    Serial.print(" verif1: ");
    Serial.println(verif1);
    touched = true;
  }

  // Check if the "-" button is pressed
  if (x > 210 && x < 250 && y > 70 && y < 120)
  {
    calibrationOffset -= 0.2;
    verif1 -= 1;
    Serial.print("Calibration Offset: ");
    Serial.print(calibrationOffset);
    Serial.print(" verif1: ");
    Serial.println(verif1);
    touched = true;
  }

  // Check if the "Reset" button is pressed
  if (x > 260 && x < 320 && y > 70 && y < 120)
  {
    calibrationOffset = 0.0;
    verif1 = 0;
    Serial.println("Values Reset");
    Serial.print("Calibration Offset: ");
    Serial.print(calibrationOffset);
    Serial.print(" verif1: ");
    Serial.println(verif1);
    touched = true;
  }

  // Add a small delay to debounce the touch
  if (touched)
  {
    displayVerif();
    delay(200); // Adjust the delay time as necessary
  }
}
