#include<SimbleeForMobile.h>

uint8_t data[8] = {0xaa, 0xaa, 0x04};

signed int value;
signed int ynow;
int ui_ynow;
int ui_value;
int num=0;
int ui_num;
int dnum=0;
int ui_dnum;

int tempnum=0;

int ui_box[200];
int posx[200];
int posnow=0;

void setup() {
   Serial.begin(57600);
   SimbleeForMobile.deviceName = "Objects";
   SimbleeForMobile.advertisementData ="Update";
   SimbleeForMobile.begin();
   for(int k=0;k<200;k++)
      posx[k]=10+1.5*k;
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
  if (Serial.available() >= 16)
  {
    if (!fixedData()) {
      return false;
    }
    data[7] = 0;
    for (uint8_t i = 3; i < 7; i++)
    {
      data[i] = Serial.read();
      data[7] += data[i];
    }
    if (data[7] + Serial.read() != 0xff) {
      return false;
    }
    data[7] = 0xff - data[7];
    value=(data[5]<<8)|data[6];

    if((value>>8)>=128)
      {
       ynow=(value>>8)-256;
      }
    else
      ynow=(value>>8);
    value=ynow;
    Serial.println(ynow);
    return true;
  }
  return false;
} 
 
#define TimeSlice 5 //ms
#define LoopTime TimeSlice * 1000
unsigned long count = 0;
unsigned long t = 0;
bool newData = false, SendTri = false, MonitorTri = false, RecordTri = false,Update_dnum = false, DeviceConnected = false;
int32_t lastTime;
void loop() {
  t = micros();
  if (count % 1 == 0)RecordTri = true;
  if (count % 1 == 0)SendTri = true;
  if (millis()-lastTime>=1000)Update_dnum = true;
  if (Update_dnum && DeviceConnected)
  {
    dnum=num-tempnum;
    tempnum=num;
    Update_dnum = false;
    lastTime=millis();
    if(SimbleeForMobile.updatable)
      SimbleeForMobile.updateValue(ui_dnum,dnum);      
  } else if (SendTri && newData  && DeviceConnected) {
    SendTri = false;
    newData = false;

    Serial.println(value);
    num++;
    posnow=(posnow+1)%200;  
    if(SimbleeForMobile.updatable){
//      SimbleeForMobile.updateY(ui_box[posnow],ynow+360);
      if(ynow>=0)
      { SimbleeForMobile.updateY(ui_box[posnow],360);
        SimbleeForMobile.updateH(ui_box[posnow],ynow);}
      else
       { SimbleeForMobile.updateY(ui_box[posnow],360+ynow);
         SimbleeForMobile.updateH(ui_box[posnow],abs(ynow));}
7
//      SimbleeForMobile.updateValue(ui_value,value);
//      SimbleeForMobile.updateValue(ui_num,num);
//      SimbleeForMobile.updateValue(ui_ynow,ynow);
    }
  } else if(RecordTri && DeviceConnected) {
    
    RecordTri = false;
    newData = ReadRawData();
    Serial.flush();
  } 
  count++;
  SimbleeForMobile.process();
  delayMicroseconds((LoopTime-(micros()-t))%LoopTime);
}

void ui(){
//  const uint16_t title_x = 70, text_x = 210;
  SimbleeForMobile.beginScreen(WHITE);
//  const uint16_t title_y = 40;
 // SimbleeForMobile.drawText(100,title_y,"Simblee",BLUE,30);
//  const uint16_t Raw_data_y = title_y+60;
//  SimbleeForMobile.drawText(title_x,Raw_data_y,"Raw_data",BLACK,20);
//  ui_value = SimbleeForMobile.drawText(text_x,Raw_data_y,value,BLACK,20);
 //// const uint16_t num_y = Raw_data_y+40;
//  SimbleeForMobile.drawText(title_x,num_y,"num",BLACK,20);
//  ui_num=SimbleeForMobile.drawText(text_x,num_y,num,BLACK,20);
//  const uint16_t dnum_y = num_y+40;
//  SimbleeForMobile.drawText(title_x,dnum_y,"dnum",BLACK,20);
//  ui_dnum=SimbleeForMobile.drawText(text_x,dnum_y,dnum,BLACK,20);
//  const uint16_t ynow_y = dnum_y+40;
//  SimbleeForMobile.drawText(title_x,ynow_y,"ynow",BLACK,20);
//  ui_ynow=SimbleeForMobile.drawText(text_x,ynow_y,ynow,BLACK,20);
  for(int k=0;k<200;k++)
    ui_box[k] = SimbleeForMobile.drawRect(posx[k],360,2,2,BLACK);  
  SimbleeForMobile.endScreen();
}

void SimbleeForMobile_onConnect()
{
  num =0;
  tempnum=0;
  count=0;
  DeviceConnected = true;
}
void SimbleeForMobile_onDisconnect(){
  DeviceConnected = false;
  lastTime=millis();
}

