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

//Some Joystick Control Definitions
#define JX_AXIS_PIN 34
#define JY_AXIS_PIN 36
#define MAX_AXIS_MAP 10
#define IDLE_TEST_COUNT 100
#define J_DEAD_ZONE 12
int JX_OFFSET = 0;
int JY_OFFSET = 0;
int JX_CURRENT = 0;
int JY_CURRENT = 0;

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
  MyDisplay.fillRoundRect(SCREEN_MID_X - EYE_WIDTH - 10 + JX_CURRENT, SCREEN_MID_Y - EYE_HEIGHT / 2 + BlinkFactor / 2 + JY_CURRENT, EYE_WIDTH, EYE_HEIGHT - BlinkFactor, EYE_ROUND_RADIUS, SSD1306_WHITE);
  //Right Eye
  MyDisplay.fillRoundRect(SCREEN_MID_X + 10 + JX_CURRENT, SCREEN_MID_Y - EYE_HEIGHT / 2 + BlinkFactor / 2 + JY_CURRENT, EYE_WIDTH, EYE_HEIGHT - BlinkFactor, EYE_ROUND_RADIUS, SSD1306_WHITE);
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
  for(int i=0;i<IDLE_TEST_COUNT;i++){
    JX_OFFSET += analogRead(JX_AXIS_PIN);
    JY_OFFSET += analogRead(JY_AXIS_PIN);
    delay(1000/IDLE_TEST_COUNT);
  }
  JX_OFFSET /= IDLE_TEST_COUNT;
  JY_OFFSET /= IDLE_TEST_COUNT;
}

void loop() {
   //Get the current joystick reading
  JX_CURRENT = analogRead(JX_AXIS_PIN);
  JY_CURRENT = analogRead(JY_AXIS_PIN);
  if(JX_CURRENT<=JX_OFFSET-J_DEAD_ZONE){
    JX_CURRENT = map(JX_CURRENT,0,JX_OFFSET,-MAX_AXIS_MAP,0);
  }
  else if(JX_CURRENT>=JX_OFFSET+J_DEAD_ZONE){
    JX_CURRENT = map(JX_CURRENT,JX_OFFSET,4095,0,MAX_AXIS_MAP);
  }
  else{
    JX_CURRENT = 0;
  }
  if(JY_CURRENT<=JY_OFFSET-J_DEAD_ZONE){
    JY_CURRENT = map(JY_CURRENT,0,JY_OFFSET,-MAX_AXIS_MAP,0);
  }
  else if(JY_CURRENT>=JY_OFFSET+J_DEAD_ZONE){
    JY_CURRENT = map(JY_CURRENT,JY_OFFSET,4095,0,MAX_AXIS_MAP);
  }
  else{
    JY_CURRENT = 0;
  }
  HandleBlinking();
  DrawEyes();
  delay(1);
}
