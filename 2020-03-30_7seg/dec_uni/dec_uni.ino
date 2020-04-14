/* This is a default program--
	Use File->Load Prog to load a different program
*/

int count,cen,dec,uni;

void setup()
{
	count=925;
}

void loop()
{
	//contador modulo 1000 
	if(count<999)count=count+1;
	else count=0;
	//extraer centenas,decenas,unidades
	cen=(count/100)%10;
	dec=(count/10)%10;
	uni=count%10;
	delay(250);
}
