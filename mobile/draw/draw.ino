#include<SimbleeForMobile.h>
int8_t counter=0;
int count=0;
int counterExpand=0;
int8_t ui_counter;
uint8_t ui_food;
int ui_box[100];
int posx[100]={0};
int posnow=0;

char tempBag[9]={0};
uint8_t tempCounter[2]={0};

void setup() {
   Serial.begin(57600);

   SimbleeForMobile.deviceName = "Objects";
   SimbleeForMobile.advertisementData ="Update";
   SimbleeForMobile.begin();
   for(int k=0;k<100;k++)
     posx[k]=10+3*k;
}
void loop() {
     int ynow;
     SimbleeForMobile.process();
     if (SimbleeForMobile.updatable)
     {
          if (Serial.available())
          {
             while(1)
             { 
              char * tempBagPtr= (char *) &tempBag;
              uint8_t i=0;
              while (Serial.available() && i<9)
                  {  
                     *tempBagPtr++ = Serial.read();
                     i++;
                   }         
              if(tempBag[0]==0xAA && tempBag[1]==0xAA )
                  {
                      tempCounter[0]=tempBag[5];
                      tempCounter[1]=tempBag[6];
                      counterExpand=(tempCounter[0]<<8)|tempCounter[1];
                      counter=counterExpand>>8;
                      ynow=counter+300;
                      break;
                   }
              }
          }      
          SimbleeForMobile.updateY(ui_box[posnow],ynow);
          posnow=(posnow+1)%100;
          delay(50);
        }

//      array[count%140]=counter+300;
//      count++;

    }



void ui(){
  SimbleeForMobile.beginScreen(WHITE);
  SimbleeForMobile.drawText(100,40,"Simblee",BLUE,30);
//  SimbleeForMobile.drawText(70,120,"Raw_data",BLACK,20);
  for(int k=0;k<100;k++)
    ui_box[k] = SimbleeForMobile.drawRect(posx[k],200,2,2,BLACK);
  SimbleeForMobile.endScreen();
}

//void SimbleeForMobile_onConnect()
//{
//  count =0;
//  lasttime=millis();
//}
