#ifndef CLOCK_H
#define CLOCK_H

class Clock{

  enum colorMostrarTiempo {W,B};    //W: white, B: Blue

  private:
    int HourPositionX,HourPositionY;
    int MinutePositionX,MinutePositionY;

  public:
  volatile bool updateMinute;
  volatile bool updateHour;
  volatile bool updateHalfSecond;
  volatile unsigned char halfsecond;
  volatile unsigned char second;
  volatile unsigned char minute;
  volatile unsigned char hour;
  
  char bufH[3];
  char bufM[3];
  
  bool settingHourFlag;
  bool settingMinuteFlag;

  Clock():HourPositionX(52),HourPositionY(55),MinutePositionX(89),MinutePositionY(55){}
  
  void displayTime();
  void displayHour(int t_colorIndex);
  void displayMinute(int t_colorIndex);
  void setTime(bool& t_flagSelect,bool& t_flagEnter); 
  void incHour();
  void incMinute();
  
};


//================================= DISPLAY TIME ====================================

void Clock::displayTime()
{  

    enum seondDisplayState_t {  ON,OFF};
    static seondDisplayState_t secondDisplayState=OFF;  
    
    if(updateMinute || updateHour) //si la interrupcion ha modificado la hora y/o los minutos
    {  
      if(!settingHourFlag)      //si no se esta cambiando las horas manualmente
      {
        if(updateHour)          //si la interrupcion ha modificado la hora
        {
          updateHour=false;
          displayHour(W);       //mostar horas en blanco
        }
        
      }

      if(!settingMinuteFlag)    //si no se estan cambiando los minutos manualmente
      {
        if(updateMinute)        //si la interrupcion ha modificado los minutos
        {
          updateMinute=false;
          displayMinute(W);     //mostar minutos en blanco
        } 
      }
    }//end if 

  //animacion segundos y mostrar cambio de hora y minutos manualmente
  if(updateHalfSecond)                                            // si ha pasado medio segundo
  {
    updateHalfSecond=false;
    switch(secondDisplayState)
    {
      case OFF:
        TFTscreen.fillRect(128,54,8,18,COLOR_BLACK);              //rectangulo de segundos apagado
        if(settingHourFlag){                                      //si se estan cambiando las horas manualmente
          TFTscreen.stroke(0,0,0);                                //borrar del display las horas
          TFTscreen.text(bufH,HourPositionX,HourPositionY);       //  
        }
        if(settingMinuteFlag){                                    //si se estan cambiando los minutos manualmente
          TFTscreen.stroke(0, 0, 0);                              //borrar del display los minutos
          TFTscreen.text(bufM,MinutePositionX,MinutePositionY);       
        }
        secondDisplayState=ON;
     break;      
     case ON:
        TFTscreen.fillRect(128,54,8,18,COLOR_MAGENTA);             //rectangulo de segundos encendido
        if(settingHourFlag) displayHour(B);                        //si se estan cambiando las horas manualmente: mostrar las horas en azul
        if(settingMinuteFlag) displayMinute(B);                    //si se estan cambiando los minutos manualmente: mostrar los minutos  en azul  
        secondDisplayState=OFF;
     break;  
    }//end switch 
  }
}// end displayTime()   

//=================================== DISPLAY_HOUR =======================================

void Clock::displayHour(int t_colorIndex)
{
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(0,0,0);          //borrar
    TFTscreen.text(bufH, HourPositionX,HourPositionY);
    itoa(hour,bufH,10); 
    if(hour<10)                       //no mostrar 0 en decenas
    {
      bufH[1]=bufH[0];
      bufH[0]=' ';          
    }
    if(t_colorIndex==W)
      TFTscreen.stroke(255, 255, 255);  //mostrar en blanco
    else
      TFTscreen.stroke(255,0,0);        //mostrar en azul
    TFTscreen.text(bufH,HourPositionX,HourPositionY);
}

//=================================== DISPLAY_MINUTE =======================================

void Clock::displayMinute(int t_colorIndex)
{
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(0,0,0);          //borrar
    TFTscreen.text(bufM,MinutePositionX,MinutePositionY);
    itoa(minute,bufM,10);
    if(minute<10)                     //mostrar 0 en decenas
    {
      bufM[1]=bufM[0];
      bufM[0]='0';
    }
    if(t_colorIndex==W)
      TFTscreen.stroke(255, 255, 255);  //mostrar en blanco
    else
      TFTscreen.stroke(255,0,0);        //mostrar en azul
    TFTscreen.text(bufM,MinutePositionX,MinutePositionY);
}


//=================================== SET_TIME =======================================

void Clock::setTime(bool& t_flagSelect,bool& t_flagEnter)
{

  enum setTimeState_t { init,setHour,setMinute,setHourAuto,setMinuteAuto};
  static setTimeState_t setTimeState=init;  
  switch(setTimeState)
  {
    case init:
      if(t_flagSelect)                            //si se pulso el boton Select
      {
        t_flagSelect=false;
        settingHourFlag=true;                     //indicador de cambio manual en las horas
        TFTscreen.setTextSize(1); 
        TFTscreen.stroke(255,0, 0);               //colocar en azul mensaje: SET TIME
        TFTscreen.text("SET TIME",59, 20);
        setTimeState=setHour;   
      }
      break;  
      
    case setHour:
      if(t_flagSelect)                            //si se pulso el boton Select nuevamente
      {
        t_flagSelect=false;
        second=0;
        incHour();                                //incrementar horas
      }
      if(t_flagEnter)                             //si se pulso el boton Enter
      {     
        t_flagEnter=false;
        settingHourFlag=false;
        settingMinuteFlag=true;                   //indicar cambio de minutos manualmemte
        displayHour(W);                           //colocar horas en color blanco
        setTimeState=setMinute;
      }
      break;  
      
    case setMinute:
      if(t_flagSelect)                            //si se pulso el boton Select nuevamente
      {
        t_flagSelect=false;
        second=0;
        incMinute();                              //incrementar minutos
      } 
      if(t_flagEnter)                             //si se pulso el boton Enter
      {     
        t_flagEnter=false;
        settingHourFlag=false;                    //indicadores de cambio manual en falso
        settingMinuteFlag=false;                  //
        displayMinute(W);                         //mostrar minutos en color blanco
        TFTscreen.setTextSize(1); 
        TFTscreen.stroke(0, 0, 0);                //borrar mensaje: SET TIME
        TFTscreen.text("SET TIME",59, 20);
        second=0;
        setTimeState=init;
      }     
      break;  
  }  
}

//================================  INC_HOUR  ==================

void Clock::incHour()
{
  if(hour<23)
  {
    hour++;
  }
  else
  {
    hour=0;
  }
}

//================================  INC_MINUTE  =================

void Clock::incMinute()
{
  if(minute<59)
  {
    minute++;
  }
  else
  {
    minute=0;
  }
}

#endif
