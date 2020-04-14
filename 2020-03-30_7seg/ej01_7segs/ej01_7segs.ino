/*	display 7 segs: muestra las unidades de un contador
	modulo 100. Código para mostrar unidades en display
	se encuentra en funcion loop
*/

const int pinUnidades[5]={2,3,4,5,6};
const int pinDecenas[5]={7,8,9,10,11};
//const byte digits[10]={0b0000,0b0001};
int count,dec,uni;

void setup()
{
	for(int i=2;i<12;i++) pinMode(i,OUTPUT);
	count=-1;	
}

void loop()
{
	
	//contador modulo 100
	if(count<99) count++;
	else count=0;
	
	//extraer decenas,unidades
	dec=(count/10)%10;
	uni=count%10;
	
	//codigo para mostrar unidades en display 7 segs
	for(int i=0;i<4;i++){
		digitalWrite(pinUnidades[i],bitRead(uni,i));
		digitalWrite(pinDecenas[i],bitRead(dec,i));
	}
	
	//retardo
	delay(500);
	
}
