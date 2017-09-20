///////////////////////////////////////////
////// Electric Skateboard lighting System

////////////////
// ACCELEROMETER
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>


#define BRAKETHRESHOLD        350
#define BRAKETIMETHRESHOLD    200


int start = 0;

int prevX = 0;
int currentX = 0;

int cState = 0;
int dState = 0;

long brakeTime = 0;

/* Assign a unique ID to this sensor at the same time */

Adafruit_LSM303 lsm;

// Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

////////////
// NEOPIXEL
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
int totalLEDs = 60;
int ledFadeTime = 3;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:

Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLEDs, PIN, NEO_GRB + NEO_KHZ800);


void setup(void) {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100);
  strip.show(); // Initialize all pixels to 'off'

// =================================
// ----- SETUP ACCELEROMETER -------
// =================================

 /* Initialise the sensor */
  if(!lsm.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

} //END SETUP


/////////////
// MAIN LOOP


void loop(void) {

  lsm.read();
  currentX = abs(lsm.accelData.x);

  if (start == 0){
    prevX = currentX;
    start = 1;
  }

  int i = currentX - prevX;

  if (abs(i) > BRAKETHRESHOLD) {
    brakeTime = millis();
    int strikes = 0;
    while ((abs(i) > BRAKETHRESHOLD) && (strikes < 3)) {
      if (abs(i) < BRAKETHRESHOLD) {
        strikes = strikes + 1;
      }
      lsm.read();
      currentX = abs(lsm.accelData.x);
      i = currentX - prevX;

      if ((millis() - brakeTime) > BRAKETIMETHRESHOLD) {
        brakeLights(strip.Color(255, 0, 0, 0), 250); // Red
        while (abs(i) > BRAKETHRESHOLD) {
          lsm.read();
          currentX = abs(lsm.accelData.x);
          i = currentX - prevX;
          Serial.println(i);
          delay(100);
        }
        // hideAll();
        idleLights(255, 50, 0, ledFadeTime); // Orange
        brakeTime = millis();
        i = 0;
        lsm.read();
        currentX = abs(lsm.accelData.x);
      }
    }
  }

  prevX = currentX;
  delay(200);

}




  // idleLights(255, 50, 0, ledFadeTime); // Orange
  // idleLights(255, 50, 0, ledFadeTime); // Orange
  // brakeLights(strip.Color(255, 0, 0, 0), 3000); // Red
  // motionLights(strip.Color(32, 107, 255), 75); // Blue

  // colorWipe(strip.Color(255, 83, 13), 50); // Orange
  // theaterChase(strip.Color(255, 255, 255), 70); // White
