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

	void draw(TFT TFTscr){
		if (posX != oldX || posY != oldY){
			TFTscr.fillRect(oldX, oldY, w, h, COLOR_BLACK);
			for (int row = 0; row < h; row++) {
				for (int col = 0; col < w; col++) {
					word p = pgm_read_word(tennis_ball + row*w + col);
					TFTscr.drawPixel(col+posX, row+posY, p);
				}
			}
		}
	} // draw()

	void move(court_t crt){	// Return -1 or 1 for goal on left or right ends, else 0
		oldX = posX; 
		oldY = posY;

		
		if (posY <= crt.top || posY >= crt.bottom - h) {
			speedY = -speedY;
		}
		
		posX += speedX;
		posY += speedY;
		
	} // move()

	int check_goal(court_t crt){
		if (posX <= crt.left)  {
			return -1;
		} else if (posX >= crt.right - w) {
			return 1;
		}
		return 0;
	} // check_goal()

}; // class Ball

