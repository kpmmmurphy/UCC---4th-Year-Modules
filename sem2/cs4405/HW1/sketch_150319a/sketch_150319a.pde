import java.io.File;
import java.util.Iterator;
import javax.imageio.*;
import javax.imageio.stream.*;
import javax.imageio.stream.FileImageOutputStream;
import java.awt.image.BufferedImage;
import java.util.Collections;

final int BLOCKSIZE = 16; //----------------> BLOCK SIZE??

String sourceFile = "test-01.tga";

PImage source;
PImage originalImage;
PImage spotImage;

float blurQuantum = 2.0;
ArrayList<Float> contrastList;

void setup(){
   source = loadImage(sourceFile);
   size(source.width, source.height);
   noFill();
   
   originalImage = createImage(source.width, source.height, RGB);
   originalImage.set(0, 0, source);
   
   
   contrastList = getContrastList(originalImage);
   Collections.sort(contrastList);  
    
   //float median = contrastList.get(Math.round(contrastList.size() / 2));
   //System.out.println("Median Value : " + median);
   
   float avgContrast = getAvgContrast(contrastList);
   doBlurFilter(originalImage, avgContrast);
   image(originalImage, 0, 0);
   
}

void doBlurFilter(PImage source, float avgContrast){
 for(int y = 0; y < source.height; y += BLOCKSIZE){
      for(int x = 0; x < source.width ; x += BLOCKSIZE){
        PImage blk = source.get(x, y, BLOCKSIZE, BLOCKSIZE);
        float contrast = getContrastOf(blk);
        if(contrast >= avgContrast * 2){
            System.out.println("Above Avg Contrast");
            blurBlock(x, y, originalImage);
        }
      }
    } 
    image(originalImage, 0, 0);
}
  

ArrayList<Float> getContrastList(PImage source){
  ArrayList<Float> contrastList = new ArrayList<Float>();
  float contrast = 0.0;
  
  for(int y = 0; y < source.height; y += BLOCKSIZE){
      for(int x = 0; x < source.width ; x += BLOCKSIZE){
        PImage blk = source.get(x, y, BLOCKSIZE, BLOCKSIZE);
        contrast = getContrastOf(blk);
        contrastList.add(contrast);
      }
   }
   return contrastList;
}

float getAvgContrast(ArrayList<Float> contrastList){
   float contrastTally = 0.0;
   for(float v : contrastList){
      contrastTally += v;
   }
   float avgContrast = contrastTally / contrastList.size();
   System.out.println("Avg Contrast Value : " + avgContrast);
   return avgContrast;
}

void draw(){
    
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
  PImage b = p.get(x, y, BLOCKSIZE, BLOCKSIZE); 
  b.filter(BLUR, blurQuantum);
  p.set(x, y, b);
}

void mousePressed() {
  if (mouseButton == LEFT) {
    noFill(); stroke(color(255,0,0));
    int x = floor(mouseX/BLOCKSIZE) * BLOCKSIZE; 
    int y = floor(mouseY/BLOCKSIZE) * BLOCKSIZE; 
    
    blurBlock(x,y, originalImage);
    image(originalImage, 0, 0);
    rect(x,y,BLOCKSIZE,BLOCKSIZE);
  }
}

void keyPressed() {
  System.out.println("Writing Out File");
  saveAsJPEG(originalImage,"as-jpeg-"+frameCount + ".jpg", 0.5);
}

void saveAsJPEG(PImage p, String fn, float quality) {
  fn = dataPath(fn);
  BufferedImage javaImage = (BufferedImage) p.getNative();

  Iterator iter = ImageIO.getImageWritersByFormatName("jpeg");
  ImageWriter writer = (ImageWriter)iter.next();
  ImageWriteParam iwp = writer.getDefaultWriteParam();
  iwp.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);
  iwp.setCompressionQuality(quality);

  File file = new File(fn);
  try {
    FileImageOutputStream output = new FileImageOutputStream(file);
    writer.setOutput(output);
    IIOImage image = new IIOImage(javaImage, null, null);
    writer.write(null, image, iwp);
    writer.dispose();
  }
  catch (Exception e) {
    println("unable to create " + fn);
  }
}


