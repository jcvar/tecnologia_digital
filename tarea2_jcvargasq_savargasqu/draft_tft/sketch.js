let led_size = 32;
let R = '#FF0000';
let G = '#00FF00';
let B = '#0000FF';
let Y ='#FFFF00';
let BG ='#000000';
let valA, valB;

function turnON(color, x) {
  fill(BG);
  for (let i = led_size; i <= led_size*4; i += led_size) {
    ellipse(i, led_size, led_size, led_size); // (x, y, w, h);
  }
  fill(color);
  ellipse(x, led_size, led_size, led_size); // (x, y, w, h);
}


function setup() {
  createCanvas(160, 128);
  strokeWeight(2);
  stroke(255); // Stroke color = white
}

function draw() {
  valA = mouseX; //map(analogRead(sensorA), 0, 1023, 0, 300);
  valB = mouseY; //map(analogRead(sensorB), 0, 1023, 0, 200);
  fill(BG);
  background(0);
  text('valA: ' + str(valA), 0, 100);
  text('valB: ' + str(valB), 100, 100);

  if (valA >= 250){
    turnON(R, led_size);
  }else if (valA < 180){
    diff = valA - valB;
    if (diff > 30 && diff < 70){
      turnON(Y, led_size*4);
    }else if (diff >= 70){
      turnON(B, led_size*3);
    }else{
      turnON(G, led_size*2);
    }
  }else{
    turnON(G, led_size*2);
  }
}

