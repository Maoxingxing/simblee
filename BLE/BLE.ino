#include "SimbleeBLE.h"

void setup() {
  SimbleeBLE.begin();
}

void loop() {
  SimbleeBLE.send("kkxxkkxx",8);
  delay(10);
}

