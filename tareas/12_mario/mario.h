enum state_t {	t0,t1};

class Mario {
	public:
	state_t state;
	int posX;
	int posY;
	int jumpPosY;
	static const int w = 16; // Width
	static const int h = 28; // Height
	unsigned int walkingDelay;
	bool isJumping;
	
	void walking()
	{
		static int index=0;
		static unsigned int previousMillis=0,currentMillis;
		currentMillis=millis();
		if((currentMillis-previousMillis) > walkingDelay  && isJumping==false)
		{
			previousMillis=currentMillis; 
			//image reading:
			int buffidx=0;
			for (int row=0; row<h; row++) 
			{
				for (int col=0; col<w; col++) 
				{
					word p;
					switch(index)
					{
						case 0:
							 p=pgm_read_word(m0 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;
						case 1:
							 p=pgm_read_word(m1 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;  
						case 2:
							 p=pgm_read_word(m2 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;      
						case 3:
							 p=pgm_read_word(m3 + buffidx);
							TFTscreen.drawPixel(col+posX,row+posY,p);
							break;
					}   
					buffidx++;
				}
			} 
			
			if (index<3)
			{
				++index;
			}
			else
			{
				index=0;
			} 
		}   
	}
	
	void jumping(unsigned int jumpTime, int jumpButtonPin)
	{
		static unsigned long previousMillis=0;
		static unsigned long previousMillisA=0;
		unsigned long currentMillis;
		currentMillis=millis();   
		if((currentMillis-previousMillis) > walkingDelay)
		{ 
			previousMillis=currentMillis;
			switch(state)
			{   
				case t0:
					if (digitalRead(jumpButtonPin)==HIGH)
					{
						isJumping=true;
						//walking image erase:
						int buffidx=0;
						for (int row=0; row<h; row++) 
						{
							for (int col=0; col<w; col++) 
							{
								TFTscreen.drawPixel(col+posX,row+posY,COLOR_BLACK);
								buffidx++;
							}
						}
						
						//jump image reading:
						buffidx=0;
						for (int row=0; row<h; row++) 
						{
							for (int col=0; col<w; col++) 
							{
								word p=pgm_read_word(m1 + buffidx);
								TFTscreen.drawPixel(col+posX,row+jumpPosY,p);
								buffidx++;
							}
						}
						previousMillisA=millis();
						state=t1;   
					}
					
					break;
				case t1:
					currentMillis=millis(); 
					if((currentMillis-previousMillisA) > jumpTime)
					{
						//jump image erase:
						int buffidx=0;
						for (int row=0; row<h; row++) 
						{
							for (int col=0; col<w; col++) 
							{
								TFTscreen.drawPixel(col+posX,row+jumpPosY,COLOR_BLACK);
								buffidx++;
							}
						}     
						isJumping=false;
						state=t0;
					}
					break;
			} 
			
		}
	}
};
