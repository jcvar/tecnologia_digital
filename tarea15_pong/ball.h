class Ball
{
	public:
	int posX;
	int posY;
	int oldPosX;
	int oldPosY;
	int deltaX;
	int deltaY;
	unsigned long previousMillis;
	
	Ball(int t_posX,int t_posY,int t_deltaX,int t_deltaY)
	{
		posX=t_posX;
		posY=t_posY;
		deltaX=t_deltaX;
		deltaY=t_deltaY;
		oldPosX=posX;
		oldPosY=posY;
		previousMillis=0;	
	}
	
	void move()
	{
		const int ballWidth=5;
		const int ballHeight=5;
		unsigned long currentMillis=0;
		int w = ballWidth;
		int h = ballHeight;
		int row;
		int col;
		int buffidx=0;
		
		currentMillis=millis(); 
		if(currentMillis-previousMillis>50){
			previousMillis=currentMillis;
			
			// move restrictions
			if (posX > (TFTscreen.width()-8)) {
				deltaX = -deltaX;
			} 
			
			if (posX <4) {
				deltaX = -deltaX;
			} 
			
			if (posY > 110)
			{
				deltaY = -deltaY;
			}
			
			if (posY < 15) 
			{
				deltaY = -deltaY;
			}       				
			
			// erase the ball's previous position			
			if (posX != oldPosX)
			{
				TFTscreen.fillRect(oldPosX, oldPosY, 5, 5,0x0099);
			}				
			if (posY != oldPosY)
			{
				TFTscreen.fillRect(oldPosX, oldPosY, 5, 5,0x0099);
			}	
			
			//image reading:					
			for (row=0; row<h; row++) {
				for (col=0; col<w; col++) {
					word p=pgm_read_word(ballImage + buffidx);
					TFTscreen.drawPixel(col+posX,row+posY,p);
					buffidx++;
				}
			}  	
			
			//update
			oldPosX=posX; 
			oldPosY=posY;
			posX+=deltaX;
			posY+=deltaY;					
		}
	}
};

