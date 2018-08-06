#include <IRremote.h>
#include "Arduino.h"

const byte TRIGGER_PIN                      = 12;     // Primary Trigger
const byte RELOAD_PIN                       = 13;     // Reload Button
const byte MUZZLE_LED_PIN                   = 8;     // Muzzle LED
const byte STATUS_LED_PIN                   = 9;     // Muzzle LED
const byte IR_INPUT                         = 5;
int rounds = 2;
IRrecv irrecv(IR_INPUT);
IRsend irsend;
decode_results results;

void flashStatusLed(){
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW);
}

void statusLedHit(){
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(1000);
    digitalWrite(STATUS_LED_PIN, LOW); 
}

void infraRedReceive(){
    flashStatusLed();
}

void sendInfraredBurst(){
    Serial.println("Send infrared burst");
    for (int i = 0; i < 3; i++) {
		irsend.sendSony(0xa90, 12);
		delay(40);
	}
	delay(2000); //5 second delay between each signal burst
}

void reloadRounds(int buttonState) {
    if (buttonState == LOW){
        rounds = 10;   
        flashStatusLed();
    }
}

void writeMuzzleFlash(int buttonState) {
    if (buttonState == LOW && rounds > 0){
      rounds--;  
      sendInfraredBurst();
      digitalWrite(MUZZLE_LED_PIN, HIGH); 
      delay(500);     
    }else{
        digitalWrite(MUZZLE_LED_PIN, LOW);
    }
}

void readButtonStates(){
    int fireButtonState = digitalRead(TRIGGER_PIN);
    int reloadButtonState = digitalRead(RELOAD_PIN);
    reloadRounds(reloadButtonState);
    writeMuzzleFlash(fireButtonState);
}

void readInfraredReceive(){
    if (irrecv.decode(&results)) {
        Serial.println("Receive loop");
        Serial.println(results.value, HEX);
        irrecv.resume(); // Receive the next value
  }
}


void setupPins(){
 pinMode(TRIGGER_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(RELOAD_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(MUZZLE_LED_PIN, OUTPUT);
 pinMode(STATUS_LED_PIN, OUTPUT);
 irrecv.enableIRIn();
}
/**
 * Main setup
 **/
void setup() {
    Serial.begin(9600);
    Serial.println("Serial setup....");
    setupPins();
    Serial.println("Pins all set...");
}

void loop() {
    readButtonStates();
    readInfraredReceive();
}

