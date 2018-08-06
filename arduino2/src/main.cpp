#include <IRLibRecvPCI.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLibDecodeBase.h>
#include <IRLib_P02_Sony.h>
#include <IRLibCombo.h>
#include "Arduino.h"
#include "Timer.h"
#include "Player.h"

const byte TRIGGER_PIN                      = 12;     // Primary Trigger
const byte RELOAD_PIN                       = 13;     // Reload Button
const byte MUZZLE_LED_PIN                   = 8;     // Muzzle LED
const byte STATUS_LED_PIN                   = 9;     // Muzzle LED
const byte IR_INPUT                         = 2;    //pin2 is used for interrupt

int rounds = 2;
int score = 0; //if you hit someone you get +1, if you get hit -1, in the end the one with the highest score wins

IRrecvPCI irReceiver(IR_INPUT);
IRsend irsend;
IRdecode myDecoder;

/**
 * Game settings
 * */
Player player(TeamId::Blue, PlayerId::JAIDEN, true);

const byte PLAYER_OFFSET                           = 6;
const byte TEAM_OFFSET                             = 4;

const unsigned int PACKET_TYPE                     = 0x2000;          // 10000000000000, 0 = shot packet, 1 = message packet
const unsigned int PLAYER_MASK                     = 0x1FC0;          // 01111111000000
const unsigned int SECOND_BYTE_MASK                = 0x003F;          // 00000000111111
const unsigned int TEAM_MASK                       = 0x0030;          // 00000000110000
const unsigned int DAMAGE_MASK                     = 0x000F;          // 00000000001111

const static byte MESSAGE_PACKET_SIZE              = 14;

volatile unsigned int infraRedState = 0;
bool processing = false;
/**
    Generates a shot packet containing the Players Team ID,
    the Players ID and the damage
*/
unsigned long createShotPacket(Damage damage) {
    unsigned long shotPacket = (int(player.getPlayerId()) << PLAYER_OFFSET) | (int(player.getTeamId()) << TEAM_OFFSET) | int(damage);
#ifdef DEBUG
    Serial.print(F("createShotPacket(")); Serial.print(shotPacket, BIN); Serial.println(F(")"));
#endif
    return shotPacket;
}


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
   // irsend.send(rawData,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
   
   irsend.send(SONY,createShotPacket(Damage::DAMAGE10), 20);
   Serial.println("Infrared burst sent");
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


void setupPins(){
 pinMode(TRIGGER_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(RELOAD_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(MUZZLE_LED_PIN, OUTPUT);
 pinMode(STATUS_LED_PIN, OUTPUT);
 
 irReceiver.enableIRIn(); // Start the receiver

}

void processShotPacket(unsigned long in) {

    byte playerId = (in & PLAYER_MASK) >> PLAYER_OFFSET;
    byte teamId = (in & TEAM_MASK) >> TEAM_OFFSET;
    byte damageCode = in & DAMAGE_MASK;


    
    //only apply damage if its not meeeeee
    if(static_cast<PlayerId>(playerId) != player.getPlayerId()){
        Serial.print(F("Je bent geraakt door: ")); Serial.print(player.getNameById(playerId));
        player.applyDamage(damageCode, teamId);
    }
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
    if(player.isAlive()){
        readButtonStates();
        if (irReceiver.getResults()) { 
            irReceiver.disableIRIn();
            myDecoder.decode();           //Decode it
            //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
            processShotPacket(myDecoder.value);
            player.stun();
            delay(500);
            irReceiver.enableIRIn();      //Restart receiver
        }
    }
    player.update();
}


