#include <Arduino.h>
#include "Comm.h"
#include "Mediator.h"
#include "unity.h"


#ifdef UNIT_TEST
#define DEBUG 1
// Radio Consts
const byte NODEID                           = 1;
const byte NETWORKID                        = 100;

Mediator mediator;
Comm comm(NODEID, NETWORKID, mediator);

void test_commandFromJson(){
    DynamicJsonBuffer jb;
    Command currentCommand;
    JsonObject& _object = jb.createObject();
    _object["command"] = "AdminKill";
    currentCommand = comm.commandFromJson(_object);
    TEST_ASSERT_EQUAL(currentCommand, Command::AdminKill);
}

void test_fullAmmoCommandFromJson(){
    DynamicJsonBuffer jb;
    Command currentCommand;
    JsonObject& _object = jb.createObject();
    _object["command"] = "FullAmmo";
    currentCommand = comm.commandFromJson(_object);
    TEST_ASSERT_EQUAL(currentCommand, Command::FullAmmo);
}

void setup() {
    Serial.begin(9600);
    Serial.print("*** Starting comm test");
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_commandFromJson);
    RUN_TEST(test_fullAmmoCommandFromJson);
    mediator.update();
    UNITY_END();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(500);
}

#endif
