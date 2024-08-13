#include <Keypad.h>  //Matrix Keypad Library
#include <String.h>  //String Handler Library
#include <BleMouse.h>  //Bluetooth Low Energy Mouse Library https://github.com/sirfragles/ESP32-BLE-Mouse/tree/dev

const byte ROWS = 4;  //Keypad Rows Count
const byte COLS = 4;  //Keypad Columns Count
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};  //Keypad Character Keys Arrays
byte rowPins[ROWS] = {17,16,27,14};  //Board Pins for Rows
byte colPins[COLS] = {12,13,5,23};  //Board Pins for Columns

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);  //Keypad Object declaration
BleMouse blemouse("Edzwan's Tik Tok Scroller","EDZLink Corporation");  //BLE mouse object declaration

void setup() {
  Serial.begin(115200);  //serial output for debugging purpose
  blemouse.begin();  //initialize blemouse object, enabling bluetooth operations
}

void loop() {
  char key = keypad.getKey();  //get key from the matrix keypad
  if(key && blemouse.isConnected()){  //if there is any key press and the bluetooth is connected
    Serial.println(key);  //print key, debugging purpose
    if(key == 'A'){  //if press key 'A'
      blemouse.move(0,0,1);  //mouse scroll up
    }
    else if(key == 'D'){  //if press key 'D'
      blemouse.move(0,0,-1);  //mouse scroll down
    }
    else if(key == 'B'){  //if press key 'B'
      blemouse.click(MOUSE_LEFT);
      delay(50);
      blemouse.click(MOUSE_LEFT);  //simulate double clicking
    }
  }
}