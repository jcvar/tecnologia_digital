class Paddle{
	public:
	static const int w = 5;
	static const int h = 20;
	int posX;
	int posY;
	int oldY;
	int speedY;
	unsigned long speedTime;
	unsigned long previousMillis;
	
	Paddle(int t_posX,int t_posY,int t_speedY,int t_speedTime){
		posX=t_posX;
		posY=t_posY;
		speedY=t_speedY;
		oldY=posY;
		speedTime=speedTime;
		previousMillis=0;	
	}

	void draw(){
		if (oldY != posY) {
			TFTscreen.fillRect(posX, oldY, w, h, COLOR_BLACK);
			for (int row=0; row<h; row++) {
				for (int col=0; col<w; col++) {
					word p=pgm_read_word(paddle_a + row*w + col);
					TFTscreen.drawPixel(col+posX, row+posY, p);
				}
			}
		}
	}	// draw()
	
	void move(int t_posY){
		posY=t_posY;
		draw();
		oldY=posY;		
	}	// move()

};	// class Paddle
