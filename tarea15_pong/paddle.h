class Paddle{
	public:
	static const int w = 5;
	static const int h = 20;
	int posX;
	int posY;
	int oldY;
	
	Paddle(int t_posX, int t_posY){
		posX=t_posX;
		posY=t_posY;
		oldY=posY;	
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
