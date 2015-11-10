#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial BT(10,11); //10 RX, 11 TX.
char values[255];
int i = 0;
int x, y, z, xm, ym, zm;
int xmin, ymin, zmin, xmax, ymax, zmax;
#define LED_PIN 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, LED_PIN, NEO_GRB + NEO_KHZ800);

const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)


void setup() {
  
  // BT setup
  Serial.begin(9600);
  BT.begin(9600);
  //BT.println("Hello?");
 
  // LEDs setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  //-------------ACC CONFIG ----------------//
  analogReference(EXTERNAL);  // sets the serial port to 9600
  x = analogRead(0);
  y= analogRead(1);
  z = analogRead(2);
}

void loop(){
  
  x = analogRead(0);       // read analog input pin 0
  delay(1);
  y = analogRead(1);       // read analog input pin 1
  delay(1);
  z = analogRead(2);       // read analog input pin 2
  delay(1);
  
   //zero_G is the reading we expect from the sensor when it detects
  //no acceleration.  Subtract this value from the sensor reading to
  //get a shifted sensor reading.
  float zero_G = 512.0; 

  //scale is the number of units we expect the sensor reading to
  //change when the acceleration along an axis changes by 1G.
  //Divide the shifted sensor reading by scale to get acceleration in Gs.
  float scale = 102.3;

  Serial.println("Mapped values");
  Serial.print(constrain(round(((float)x - zero_G)/scale*127+127), 0, 255));
  Serial.print("\t");  
  Serial.print(constrain(round(((float)y - zero_G)/scale*127+127), 0, 255));
  Serial.print("\t");  
  Serial.print(constrain(round(((float)z - zero_G)/scale*127+127), 0, 255));
  Serial.print("\n");  // delay before next reading:
  
  Serial.println("Real values");
  Serial.print(x);  
  Serial.print("\t");  
  Serial.print(y);
  Serial.print("\t");  
  Serial.print(z);
  Serial.print("\n");  // delay before next reading:
  
  //contuniar...

  xm = constrain(round(((float)x - zero_G)/scale*127+127), 0, 255);
  ym = constrain(round(((float)y - zero_G)/scale*127+127), 0, 255);
  zm = constrain(round(((float)z - zero_G)/scale*127+127), 0, 255);
  /*xm = map(x,-1,1,0,255);
  ym = map(y,-1,1,0,255);
  zm = map(z,-1,1,0,255);*/
  delay(50);
  
  //enviar per BT
   BT.write(xm);
   delay(50);
   BT.write(ym);
   delay(50);
   BT.write(zm);

  
 
  // Get bluetooth messages and turn on lights
  if(BT.available()){
    
    int value = BT.read();
    if(value == 'R'){
    colorWipe(strip.Color(255, 0, 0), 50);
    }
    if(value == 'G'){
    colorWipe(strip.Color(0, 255, 0), 50);
    }
    if(value == 'P'){
    colorWipe(strip.Color(120, 0, 120), 50);
    }
    if(value == 'B'){
    rainbowCycle(50);
    }
  }
}    

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

