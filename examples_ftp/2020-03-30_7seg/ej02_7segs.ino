/*	display 7 segs: muestra las unidades de un contador
	modulo 100. Código para mostrar unidades en display
	se encuentra en funcion displayUnidades()
*/

const int pinUnidades[5]={2,3,4,5,6};
int count,dec,uni;

void setup()
{
	for(int i=2;i<7;i++) pinMode(i,OUTPUT);
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
	
	//llamado a funcion que muestra unidades en display
	displayUnidades(uni);
	
	
	//retardo
	delay(500);
	
}

void displayUnidades(int u){
	digitalWrite(pinUnidades[0],bitRead(u,0));
	digitalWrite(pinUnidades[1],bitRead(u,1));
	digitalWrite(pinUnidades[2],bitRead(u,2));
	digitalWrite(pinUnidades[3],bitRead(u,3));
}