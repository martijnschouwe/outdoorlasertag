/**
  Connectivity layer between arduino board and the android phone
 **/
 #include "Android.h"
 #include "Mediator.h"
 #include <ArduinoJson.h>

 #define DEBUG

 StaticJsonBuffer<200> _jsonBuffer;
 JsonObject& root = _jsonBuffer.createObject();

 void Android::commandMessage(byte in) {
 #ifdef DEBUG
     Serial.print(F("commandMessage(")); Serial.print(in, BIN); Serial.println(F(")"));
 #endif
 }
