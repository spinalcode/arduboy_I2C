#include "Arduboy.h"
#include <Wire.h>

// Make an instance of arduboy used for many functions
Arduboy arduboy;

// Variables for your game go here.
char text[] = "Press Buttons!";
byte x;
byte y;

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define NUM_CHARS (sizeof(text) - 1)
#define X_MAX (WIDTH - (NUM_CHARS * CHAR_WIDTH) + 1)
#define Y_MAX (HEIGHT - CHAR_HEIGHT)

void setup() {
  //initiate arduboy instance
  arduboy.begin();
  arduboy.setFrameRate(60);
  
  // set x and y to the middle of the screen
  x = (WIDTH / 2) - (NUM_CHARS * CHAR_WIDTH / 2);
  y = (HEIGHT / 2) - (CHAR_HEIGHT / 2);

  if (!checkI2C()){
      arduboy.println("\nCheck your circuit!");
  }

}

void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  if(arduboy.pressed(RIGHT_BUTTON) && (x < X_MAX)) {
    x++;
  }

  if(arduboy.pressed(LEFT_BUTTON) && (x > 0)) {
    x--;
  }

  if((arduboy.pressed(UP_BUTTON) || arduboy.pressed(B_BUTTON)) && (y > 0)) {
    y--;
  }

  if((arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(A_BUTTON)) && (y < Y_MAX)) {
    y++;
  }

  arduboy.clear();
  arduboy.setCursor(x, y);
  arduboy.print(text);
  arduboy.display();
}

bool checkI2C() {
  arduboy.clear();
  arduboy.println("Check i2c"); arduboy.display();
  arduboy.println("Wire.begin();"); arduboy.display();
  Wire.begin();
  byte error, address;
  int nDevices;
  arduboy.println("I2C bus Scanning..."); arduboy.display();
  nDevices = 0;
  char tempText[20];
  for(address = 8; address < 127; address++ ) {
    sprintf(tempText,"Address:%d",address);
    arduboy.setCursor(0, 24);
    arduboy.println(tempText); arduboy.display();
    
    Wire.beginTransmission(address);
    arduboy.println("begin.Transmission"); arduboy.display();
    error = Wire.endTransmission();
    arduboy.println("end.Transmission"); arduboy.display();
    if (error == 0) {
      arduboy.print("\nI2C device found at address 0x"); arduboy.display();
      if (address<16) {
        arduboy.print("0"); arduboy.display();
      }
      arduboy.println(address,HEX); arduboy.display();
      nDevices++;
    }
    else if (error==4) {
      arduboy.print("\nUnknow error at address 0x"); arduboy.display();
      if (address<16) {
        arduboy.print("0"); arduboy.display();
      }
      arduboy.println(address,HEX); arduboy.display();
    }    
  }
  if (nDevices == 0) {
    arduboy.println("No I2C devices found\n"); arduboy.display();
    return false;
  }
  else {
    arduboy.println("done\n"); arduboy.display();
    return true;
  }
}
