// libraries
#include "SimpleTimer.h"    // load the SimpleTimer library to make timers, instead of delays & too many millis statements 
#include "OneWire.h"        // load the onewire library
 
//pins
  const int photocellPin = A0;                 // photocell connected to analog 0  
  const int upRelayCoopDoor = 2;          // relay coop door up
  const int downRelayCoopDoor = 3;  // relay coop door down
  const int upButton = A1; 
  const int downButton = A2; 
// vars
  const int doorTimer = 15000;
  bool doorClosed;
  
// photocell
  int photocellReading;                        // analog reading of the photocel
  int photocellReadingLevel;                   // photocel reading levels (dark, twilight, light)
 
// Create a first timer and specify its interval in milliseconds
  SimpleTimer firstTimer(10000);
  SimpleTimer sensorTimer(1000);

// ************************************** the setup **************************************
 
void setup(void) {
  Serial.begin(9600);
 
// welcome message
  Serial.println(" Checking doCoopDoor: every 10 minutes for light levels to open or close door");
  Serial.println();
 
// coop door  
  doorClosed = false; 

// buttons
  pinMode(upButton, OUTPUT); // Generally, in push-button we take INPUT as a parameter but here we take OUTPUT because ANALOG PIN 
  digitalWrite(upButton, HIGH); // Make button condition HIGH
  pinMode(downButton, OUTPUT); // Generally, in push-button we take INPUT as a parameter but here we take OUTPUT because ANALOG PIN 
  digitalWrite(downButton, HIGH); // Make button condition HIGH

// coop door motor
  pinMode (upRelayCoopDoor, OUTPUT);    // coop door relay up  = output
  pinMode (downRelayCoopDoor, OUTPUT);     // coop door relay down = output

  digitalWrite(upRelayCoopDoor, LOW);
  digitalWrite(downRelayCoopDoor, LOW);
}
 
// functions
 
// operate the coop door
 
// photocel to read levels of exterior light
 
 void readPhotoCell() { // function to be called repeatedly - per cooptimer set in setup
 
    photocellReading = analogRead(photocellPin);
      Serial.print(" Photocel Analog Reading = ");
      Serial.println(photocellReading);
 
//  set photocel threshholds
        if (photocellReading >= 0 && photocellReading <= 50) {
        photocellReadingLevel = '1';
        Serial.print(" Photocel Reading Level:");
        Serial.println(" - Dark");
      }  else if (photocellReading  > 50 && photocellReading <= 125){
         photocellReadingLevel = '2';
         Serial.print(" Photocel Reading Level:");
         Serial.println(" - Twilight");
      }  else if (photocellReading  > 125 ) {
         photocellReadingLevel = '3';
         Serial.print(" Photocel Reading Level:");
         Serial.println(" - Light");
       }
     }
 
// open the coop door manually

void openCoopManual()
{
  // Now, normal push-button operation, LED blink when we press push button
  if(digitalRead(upButton) == LOW & !digitalRead(downButton) == LOW)  // If button pressed
  {
    digitalWrite(upRelayCoopDoor, HIGH);   // Blink LED
  }
  else digitalWrite(upRelayCoopDoor, LOW); // Else LED is off
}

void closeCoopManual()
{
  // Now, normal push-button operation, LED blink when we press push button

  if(digitalRead(downButton) == LOW & !digitalRead(upButton) == LOW)  // If button pressed
  {
    digitalWrite(downRelayCoopDoor, HIGH);   // Blink LED
  }
  else digitalWrite(downRelayCoopDoor, LOW); // Else LED is off
}


// open the coop door motor
  void openCoop(){
    digitalWrite (upRelayCoopDoor, HIGH);
    delay(doorTimer);
    digitalWrite (upRelayCoopDoor, LOW);
    doorClosed = false;
  }

// close the coop door motor
  void closeCoop(){
    digitalWrite (downRelayCoopDoor, HIGH);
    delay(doorTimer);
    digitalWrite (downRelayCoopDoor, LOW);
    doorClosed = true;
  }
 
 
  void doCoopDoor(){
      Serial.println("10 min have passed");
      if (photocellReadingLevel  == '1' && !doorClosed) {              // if it's dark
        if (photocellReadingLevel != '2' ) {             // if it's not twilight
          if (photocellReadingLevel != '3') {           // if it's not light 
            Serial.println("Closing door");
            closeCoop();                      // close the door
            Serial.println("Door closed");
          }
        }
      } 
       if (photocellReadingLevel  == '3' && doorClosed) {             // if it's light
        if (photocellReadingLevel != '2') {             // if it's not twilight
          if (photocellReadingLevel != '1') {           // if it's not dark 
            Serial.println("Opening door");
            openCoop();                       // Open the door
            Serial.println("Door open");
          }
        }
      }
    }
 
// ************************************** the loop **************************************
void loop() {
  openCoopManual();
  closeCoopManual();
  if (sensorTimer.isReady()) {
    readPhotoCell();
    sensorTimer.reset();
    }
  if (firstTimer.isReady() & !digitalRead(upButton) == LOW  & !digitalRead(downButton) == LOW) {            // Check is ready a first timer
        Serial.println("10 min have passed");
        doCoopDoor();
        firstTimer.reset();
  } 
}
