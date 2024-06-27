/*
Attempting to create an animation for an eye movement
Author: Edzwan Shah
Begin Date: 27 June, 2024
Board: WeMos D1 R32 ESP32
*/

//I2C (SCL/SDA OLED Display) Requires these libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//using the basic ESP Timer library, everything else doesn't work!
#include <esp_timer.h>

//definitions required for some of the settings for the OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1        //-1 means we share ARDUINO's RESET Pin
#define SCREEN_ADDRESS 0x3C  //OLED I2C Address

Adafruit_SSD1306 MyDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //Display Object Constructor

//Some eyes definitions and variables
#define EYE_WIDTH 46
#define EYE_HEIGHT 46
#define EYE_ROUND_RADIUS 7
#define SCREEN_MID_X SCREEN_WIDTH / 2
#define SCREEN_MID_Y SCREEN_HEIGHT / 2
volatile int BlinkFactor = 0;
volatile bool Blinking = false;
volatile int BlinkStep = 10;

//Timer Callback
void MyTimerFunc() {
  if (!Blinking) {
    Blinking = true;
  }
}

//Timer Objects creation
esp_timer_handle_t MyTimer;
const esp_timer_create_args_t MyTimerParams = { .callback = reinterpret_cast<esp_timer_cb_t>(&MyTimerFunc) };

void DrawEyes() {
  MyDisplay.clearDisplay();
  //Left Eye
  MyDisplay.fillRoundRect(SCREEN_MID_X - EYE_WIDTH - 10, SCREEN_MID_Y - EYE_HEIGHT / 2 + BlinkFactor / 2, EYE_WIDTH, EYE_HEIGHT - BlinkFactor, EYE_ROUND_RADIUS, SSD1306_WHITE);
  //Right Eye
  MyDisplay.fillRoundRect(SCREEN_MID_X + 10, SCREEN_MID_Y - EYE_HEIGHT / 2 + BlinkFactor / 2, EYE_WIDTH, EYE_HEIGHT - BlinkFactor, EYE_ROUND_RADIUS, SSD1306_WHITE);
  MyDisplay.display();
}

void HandleBlinking() {
  if (Blinking) {
    if (BlinkFactor < EYE_HEIGHT - BlinkStep) {
      BlinkFactor += BlinkStep;
    } else {
      Blinking = false;
    }
  } else {
    if (BlinkFactor > 0) {
      BlinkFactor -= BlinkStep;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  MyDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  //create Timer and set it to interrupt every 3 seconds
  esp_timer_create(&MyTimerParams, &MyTimer);
  esp_timer_start_periodic(MyTimer, 3000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  HandleBlinking();
  DrawEyes();
  delay(1);
}
