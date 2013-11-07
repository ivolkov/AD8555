/* Programming an Analog Devices AD8555 op-amp
 * Basic usage:
 *  1. Create an AD8555 class object with output pin number as parameter
 *  2. Set Second Stage Gain code, First Stage Gain code and Offset code
 *     according to IC datasheet
 *  3. Simulate or program device */

// include library header
#include <AD8555.h>

// define output pin for opamp object
int outpin = 12;

// create AD8555 object
AD8555 opamp(outpin);

void setup() {
  // initialize serial communication at 9600 bits per second
  Serial.begin(9600);
}

void loop() {
  // get Second Stage Gain code
  Serial.println("Input Second Stage Gain code (0..7)");
  // wait for user to enter value
  while (!Serial.available());
  
  // set Second Stage Gain code
  if (!opamp.setSecondStageGain(Serial.parseInt())) {
    // opamp will return false if code is out of range
    Serial.println("Invalid Second Stage Gain code. Valid range is 0..7");
    return;
  }
  
  // First Stage Gain code
  Serial.println("Input First Stage Gain code (0..127)");
  while (!Serial.available());   
  if (!opamp.setFirstStageGain(Serial.parseInt())) {
    Serial.println("Invalid First Stage Gain code. Valid range is 0..127");
    return;
  }
  
  // Offset code
  Serial.println("Input Offset code (0..255)");
  while (!Serial.available());
  if (!opamp.setOffset(Serial.parseInt())) {
    Serial.println("Invalid Offset code. Valid range is 0..255");
    return;
  }
  
  // Programming mode
  Serial.println("Choose programming mode: Enter \"0\" for simulation, \"1\" for permanent programming");

  while (!Serial.available());
  int mode = Serial.parseInt();
  if (mode == 0) {
    // simulation mode
    opamp.simulate();
    Serial.println("Done!");
  } else if (mode == 1) {
    // permanent programming mode
    Serial.println("Make sure to meet programming requirements described in AD8555 datasheet:");
    Serial.println("- A 5.5 V supply is required");
    Serial.println("- The power supply must be able to deliver 250 mA of current");
    Serial.println("- At least 0.1 μF of decoupling capacitance is needed across the power pins of the device");
    
    Serial.println("\nWARNING: This operation can not be undone, all programming values are permanent");
    Serial.println("Continue? [y/N]");
    
    while (!Serial.available());
    if (Serial.read() == 'y') {
      opamp.program();
      Serial.println("Programming... done");
    } else {
      Serial.println("Operation canceled");
    }
  }
}
