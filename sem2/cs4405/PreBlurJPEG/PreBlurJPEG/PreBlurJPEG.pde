static final String filename = "test-01.tga";

PImage source;
PImage originalImage;
PImage spotImage;

float blurQuantum = 1.0;

int blockSize = 8;
float [] contrastList;

void setup() {
  source = loadImage(filename);

  originalImage = createImage(source.width, source.height, RGB);
  originalImage.set(0, 0, source);

  size(source.width, source.height);
  image(originalImage, 0, 0);
}

void draw() {
}


float getContrastOf(PImage blk) {
  blk.loadPixels();
  color [] p = blk.pixels;
  float iMean = 0;

  for (int i = 0; i < p.length; i += 1) {
    iMean += brightness(p[i]);
  }

  iMean /= p.length;


  float c = 0;
  for (int i = 0; i < p.length; i += 1) {
    c += abs(brightness(p[i]) - iMean);
  }

  c /= (255.0);

  return c;
}


void blurBlock(int x, int y, PImage p) {
  PImage b = p.get(x, y, blockSize, blockSize); 
  b.filter(BLUR, blurQuantum);
  p.set(x, y, b);
}


void mousePressed() {
  if (mouseButton == LEFT) {
    noFill(); stroke(color(255,0,0));
    int x = floor(mouseX/blockSize) * blockSize; 
    int y = floor(mouseY/blockSize) * blockSize; 
    
    blurBlock(x,y, originalImage);
    image(originalImage, 0, 0);
    rect(x,y,blockSize,blockSize);
  }
}

void keyPressed() {
  saveAsJPEG(originalImage,"as-jpeg-"+frameCount + ".jpg", 0.5);
}
