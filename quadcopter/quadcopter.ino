#include <Servo.h>
#include <SoftwareSerial.h>
 
//Create the 4 esc objects
Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

//Esc pins
int escPin1 = 8;
int escPin2 = 9;
int escPin3 = 10;
int escPin4 = 12;

//Min and max pulse
int minPulseRate   = 1000;
int maxPulseRate   = 2000;
int throttleChange = 50;

//Radio
int RX  = 0;
int TX  = 1;
int SET = 7;
//SoftwareSerial apc220(RX, TX);

//SETUP
void setup() {
  //Turn on the radio
  digitalWrite(SET, HIGH);
  
  Serial.begin(9600); 
  //apc220.begin(9600);
  
  //Init escs
  initEscs();
}

//LOOP
void loop() {
  /*char msg = apc220.read();
  
  if (msg == 'l') {
    startUpMotors();
    
  } else if (msg == 'q') {
    writeTo4Escs(0);
    
  } else if (msg != 'l' && msg != 'q' && msg > 0) {
    int throttle = normalizeThrottle(apc220.parseInt());
    
    writeTo4Escs(throttle);
  }*/
  
  if (Serial.available() > 0) {

    // Read the new throttle value
    int throttle = normalizeThrottle(Serial.parseInt());
    
    // Print it out
    Serial.print("Setting throttle to: ");
    Serial.println(throttle);
    
    // Change throttle to the new value
    writeTo4Escs(throttle);
    
  }
}
 
//Change throttle value
void writeTo4Escs(int throttle) {
  int currentThrottle = readThrottle();
  
  int step = 1;
  if(throttle < currentThrottle) {
    step = -1;
  }
  
  // Slowly move to the new throttle value 
  while(currentThrottle != throttle) {
    esc1.write(currentThrottle + step);
    esc2.write(currentThrottle + step);
    esc3.write(currentThrottle + step);
    esc4.write(currentThrottle + step);
    
    currentThrottle = readThrottle();
    delay(throttleChange);
  }
}

//Read the throttle value
int readThrottle() {
  int throttle = esc1.read();
  
  Serial.print("Current throttle is: ");
  Serial.println(throttle);
  
  return throttle;
}

//Init escs
void initEscs() {
  esc1.attach(escPin1, minPulseRate, maxPulseRate);
  esc2.attach(escPin2, minPulseRate, maxPulseRate);
  esc3.attach(escPin3, minPulseRate, maxPulseRate);
  esc4.attach(escPin4, minPulseRate, maxPulseRate);
  
  esc1.write(0);
  esc2.write(0);
  esc3.write(0);
  esc4.write(0);
}

//Start the motors
void startUpMotors() {
  writeTo4Escs(50);
}
 
// Ensure the throttle value is between 0 - 180
int normalizeThrottle(int value) {
  if(value < 0) {
    return 0;
    
  } else if(value > 180) {
    return 180;
  }
  
  return value;
}
