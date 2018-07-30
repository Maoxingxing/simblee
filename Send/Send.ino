#include "SimbleeCOM.h"
uint8_t data[8] = {0xaa, 0xaa, 0x04};
void setup() {
  SimbleeCOM.begin();
  Serial.begin(57600);
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
      //Serial.flush();
      return false;
    }
    data[7] = 0;
    for (uint8_t i = 3; i < 7; i++)
    {
      data[i] = Serial.read();
      //No need to check because fixedData alread check rest num
      /*
      if (tempBag[i] == -1) {
        Serial.println("Check Err");
        return Serial.flush();
      }
      */
      data[7] += data[i];
    }
    if (data[7] + Serial.read() != 0xff) {
      //Serial.flush();
      return false;
    }
    data[7] = 0xff - data[7];
    return true;
  }
  return false;
}
#define TimeSlice 1 //ms
#define LoopTime TimeSlice*500
unsigned long count = 0;
unsigned long t = 0;
bool newData = false, SendTri = false, MonitorTri = false, RecordTri = false;
void loop() {
//  t = micros();
  if (count % 1/TimeSlice == 0)RecordTri = true;
  if (count % 1/TimeSlice == 0)SendTri = true;
//  if (count % 1000/TimeSlice == 0)MonitorTri = true;
  

  if (newData && SendTri) {
    SendTri = false;
    newData = false;
    SimbleeCOM.send((const char *)data, 8);
  }else if (RecordTri) {
    RecordTri = false;
    newData = ReadRawData();
  } 
/*  
    else if ((MonitorTri) {
    MonitorTri = false;
    Serial.print("读取：");
    Serial.println(count);
    Serial.print("本轮耗时：");
    Serial.println(millis() - t);
  }
*/
  count++;
//  delayMicroseconds((LoopTime-(micros()-t))%LoopTime);
}

