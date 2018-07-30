#include "SimbleeCOM.h"

void setup() {
  Serial.begin(57600);

  SimbleeCOM.begin();
}
uint8_t data[8] = {0xaa, 0xaa, 0x04};
uint8_t checkSum = 0;
void loop() {}
void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi)
{
  uint8_t i = 0;
  data[7] = 0;
  if (payload[0] == 0xAA && payload[1] == 0xAA &&
      payload[2] == 0x04 && len == 8) {
    for (i = 3; i < 7; i++)
    {
      data[7] += payload[i];
      data[i] = payload[i];
    }
    
    if (data[7] + payload[7] == 0xff) {
      data[7] = payload[7];
      for (i = 0; i < 8; i++)
        Serial.write(data[i]);
    }
    
  }

}
