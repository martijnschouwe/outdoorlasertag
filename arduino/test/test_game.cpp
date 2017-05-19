#include <Arduino.h>
#include "Game.h"
#include "Mediator.h"
#include "unity.h"


#ifdef UNIT_TEST
#define DEBUG 1

Game game;
Mediator mediator;

void test_function_isActive() {
    TEST_ASSERT_EQUAL(true, game.isActive());
}

void test_function_ammoBoxSize() {
    TEST_ASSERT_EQUAL(20, game.getAmmoBoxSize());
}

void test_function_getMediBoxSize() {
    TEST_ASSERT_EQUAL(50, game.getMediBoxSize());
}

void test_gameNotStarted(){
    TEST_ASSERT_EQUAL(true, game.isNotStarted());
}

void test_gameEnded(){
    TEST_ASSERT_EQUAL(false, game.isActive());
}

void setup() {
    Serial.begin(9600);
    game.init(mediator);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    RUN_TEST(test_gameNotStarted);
    UNITY_BEGIN();
    game.start(500);
    RUN_TEST(test_function_ammoBoxSize);
    RUN_TEST(test_function_getMediBoxSize);
    delay(6000);//wait for game to start, delay is 5000
    mediator.update();
    RUN_TEST(test_function_isActive);
    game.end();
    RUN_TEST(test_gameEnded);
    UNITY_END();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#endif
