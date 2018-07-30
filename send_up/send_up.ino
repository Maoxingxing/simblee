
#include "SimbleeCOM.h"
uint8_t data[8] = {0xaa, 0xaa, 0x04};
uint8_t data_send[14];
int k;
void setup() {
  SimbleeCOM.begin();
  Serial.begin(57600);
  k=0;
}

bool fixedData() {
  do {
    if (Serial.read() == 0xAA && Serial.read() == 0xAA &&
        Serial.read() == 0x04 && Serial.available() >= 5) {
      return true;
    }
  } while (Serial.available() >= 8);
  Serial.println("Check Err");
  return false;
}

bool ReadRawData() {
  if (Serial.available() >= 8)
  {
    if (!fixedData()) {
      return false;
    }
    data[7] = 0;
    for (uint8_t i = 3; i < 7; i++)
    {
      data[i] = Serial.read();
//      data[7] += data[i];
    }
/*    if (data[7] + Serial.read() != 0xff) {
      return false;
    }
    data[7] = 0xff - data[7];
*/
    if(k<14){
      data_send[k++]=data[5];
      data_send[k++]=data[6];
      }else if(k=14){
         k=0;
         return true;
      }
    return false;
  }
  return false;
}
#define TimeSlice 3 //ms
#define LoopTime TimeSlice*1000
unsigned long count = 0;
unsigned long t = 0;
bool newData = false, SendTri = false, MonitorTri = false, RecordTri = false;
void loop() {
  t = micros();
  if (count % 1/TimeSlice == 0)RecordTri = true;
  if (count % 1/TimeSlice == 0)SendTri = true;
  
  if (newData && SendTri) {
    SendTri = false;
    newData = false;
    SimbleeCOM.send((const char *)data_send, 14);
  }else if (RecordTri) {
    RecordTri = false;
    newData = ReadRawData();
  } 

  count++;
  delayMicroseconds((LoopTime-(micros()-t))%LoopTime);
}

