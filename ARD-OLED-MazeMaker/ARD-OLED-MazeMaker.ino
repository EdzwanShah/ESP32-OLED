//required includes
#include<SPI.h>
#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

//basic definitions for OLED I2C usage
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//create the display object
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);

//bit arrays for slash
const uint8_t PROGMEM Slash[] = {
  0b10000000,
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001,
};

//bit arrays for back slash
const uint8_t PROGMEM BackSlash[] = {
  0b00000001,
  0b00000010,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b10000000,
};

//boolean array for maze map
bool MyImage[SCREEN_HEIGHT/8][SCREEN_WIDTH/8] = {};

//setup function, run once on boot
//create a random seed from analog readings,
//initialize random maze boolean map and display object
void setup() {
  randomSeed(analogRead(0));
  for(int j=0;j<SCREEN_HEIGHT/8;j++){
    for(int i=0;i<SCREEN_WIDTH/8;i++){
      MyImage[j][i] = (bool)random(2);
    }
  }
  display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS);
}

void loop() {
  //clear the display buffer
  display.clearDisplay();
  for(int j=0;j<SCREEN_HEIGHT/8;j++){
    for(int i=0;i<SCREEN_WIDTH/8;i++){
      //on each element of the maze boolean map,
      //display either a slash or back slash based on the maze boolean
      switch(MyImage[j][i]){
        case 0:
          display.drawBitmap(i*8,j*8,Slash,8,8,SSD1306_WHITE);
          break;
        case 1:
          display.drawBitmap(i*8,j*8,BackSlash,8,8,SSD1306_WHITE);
          break;
      }
      //shift one row of the boolean map and randomize the last row
      if(j!=SCREEN_HEIGHT/8-1){
        MyImage[j][i] = MyImage[j+1][i];
      }
      else{
        MyImage[j][i] = (bool)random(2);
      }
    }
  }
  //display and delay 0.1 second
  display.display();
  delay(100);
}
