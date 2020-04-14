
bool btnpsed=false;

typedef enum{
  OFF,ON
}ledState_t;
ledState_t ledState=OFF;

bool debounce(int pin);

void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT_PULLUP);
  pinMode(6,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  btnpsed=debounce(2);

  if (btnpsed){
    btnpsed=false;
    ledState=~ledState;
    digitalWrite(6,ledState);
  }
    
}

bool debounce(int pin){
  typedef enum{
    detectPressing,detectReleasing
  }buttonState_t;
  
  static buttonState_t buttonState=detectPressing;

  bool flag=false;

  switch(buttonState){
    case detectPressing:
      if(digitalRead(pin)==LOW){
        delay(30);
        if(digitalRead(pin)==LOW){
          buttonState=detectReleasing;
        }
      }
      break;
    case detectReleasing:
      if(digitalRead(pin)==HIGH){
        delay(30);
        if(digitalRead(pin)==HIGH){
          flag=true;
          buttonState=detectPressing;
        }
      }
      break;       
  }

  return flag;

}
