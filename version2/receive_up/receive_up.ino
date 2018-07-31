#include "SimbleeCOM.h"

void setup() {
  Serial.begin(57600);

  SimbleeCOM.begin();
}
uint8_t data[5] = {0xaa, 0xaa, 0x04, 0x80, 0x02};
uint8_t i = 0;
uint8_t k = 0;
uint8_t check;
void loop() {}
void SimbleeCOM_onReceive(unsigned int esn, const char *payload, int len, int rssi)
{

    for (i = 0; i < len-1;i){  
       check=0x82+payload[i]+payload[i+1];
       check=0xff-check;
       for (k = 0; k < 5; k++)
          Serial.write(data[k]);
       Serial.write(payload[i++]);
       Serial.write(payload[i++]);
       Serial.write(check);
              
     }

}
