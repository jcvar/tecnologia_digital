class Paddle
{
	public:
	int posX;
	int posY;
	int oldPosY;
	int deltaY;
	unsigned long deltaTime;
	unsigned long previousMillis;
	
	Paddle(int t_posX,int t_posY,int t_deltaY,int t_deltaTime)
	{
		posX=t_posX;
		posY=t_posY;
		deltaY=t_deltaY;
		oldPosY=posY;
		deltaTime=deltaTime;
		previousMillis=0;	
	}
	
	void move(int t_posY)
	{
		const int paddleWidth=5;
		const int paddleHeight=20;
		unsigned long currentMillis=0;
		int w = paddleWidth;
		int h = paddleHeight;
		int row;
		int col;
		int buffidx=0;
		
		currentMillis=millis(); 
		if(currentMillis-previousMillis>deltaTime){
			previousMillis=currentMillis;
			
			posY=t_posY;
			
			// erase the paddle's previous position		
			if (oldPosY != posY) {
				TFTscreen.fillRect(posX,oldPosY,w,h,0x0099);
			}
			
			//image reading:					
			for (row=0; row<h; row++) {
				for (col=0; col<w; col++) {
					word p=pgm_read_word(paddle_a + buffidx);
					TFTscreen.drawPixel(col+posX,row+posY,p);
					buffidx++;
				}
			}  	
			
			//update
			oldPosY=posY;		
		}
	}
};