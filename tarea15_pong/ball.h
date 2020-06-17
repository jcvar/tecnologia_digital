class Ball {
	public:
	static const int w = 5;
	static const int h = 5;
	int posX;
	int posY;
	int oldX;
	int oldY;
	int speedX;
	int speedY;
	unsigned long previousMillis;
	
	Ball(int t_posX,int t_posY,int t_speedX,int t_speedY) {
		posX=t_posX;
		posY=t_posY;
		speedX=t_speedX;
		speedY=t_speedY;
		oldX=posX;
		oldY=posY;
		previousMillis=0;	
	}
	
	void draw(){
		if (posX != oldX || posY != oldY){
			TFTscreen.fillRect(oldX, oldY, w, h, COLOR_BLACK);
			for (int row = 0; row < h; row++) {
				for (int col = 0; col < w; col++) {
					word p = pgm_read_word(ballImage + row*w + col);
					TFTscreen.drawPixel(col+posX, row+posY, p);
				}
			}
		}
	} // draw()
	
	int move(){	// Return -1 or 1 for goal on left or right ends, else 0
		if (posX <= court.left)  {
			return -1;
		} else if (posX >= court.right - w) {
			return 1;
		}
		
		if (posY <= court.top || posY >= court.bottom - h) {
			speedY = -speedY;
		}
		
		posX += speedX;
		posY += speedY;
		draw();
		oldX = posX; 
		oldY = posY;
		
		return 0;
	} // move()
	
}; // class Ball

