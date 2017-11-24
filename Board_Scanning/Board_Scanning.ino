#include <Arduino.h>
#include <string.h>
int arrRow[] = {29,37,28,36,27,35,26,34};
int arrCol[] = {30,22,31,23,32,24,33,25};

// khai bao ham
void ConfigSwitch();
void check_switch();

void setup() {
  Serial.begin(9600);
  ConfigSwitch();

}

void loop() {
 check_switch();
 delay(300);

}
void ConfigSwitch(){
    for(int i=0;i<8;i++){
        pinMode(arrRow[i],OUTPUT);
        pinMode(arrCol[i],INPUT_PULLUP);
      }
  }
void check_switch() { //Ki?m tra nút nh?n
  String str="";
  int checkSwitch=0;
  for(int i=0;i<8;i++) {
    for(int j=0;j<8;j++){
         digitalWrite(arrRow[j],HIGH);
         delay(1);
      }
     digitalWrite(arrRow[i],LOW);
     delay(1);
     for(int j=0;j<8;j++){
      checkSwitch=0;
        if(digitalRead(arrCol[j])==LOW){
            checkSwitch=1;
          }
        delay(10);
         if(digitalRead(arrCol[j])==LOW && checkSwitch==1){
            str+="1";
          }
          else str+="0";
          delay(10);
      }
     digitalWrite(arrRow[i],HIGH);
     delay(10);
    }
     Serial.println(str);
}
