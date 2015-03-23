import edu.emory.mathcs.jtransforms.dct.FloatDCT_2D;

PImage source, encoded;
String sourceFile = "Original-me.png";

final int BLOCKSIZE = 8;

int qualityJPEG = 50; //For enteries in luminence table, these are used unscaled. 

FloatDCT_2D dct2D = new FloatDCT_2D(BLOCKSIZE, BLOCKSIZE);
ArrayList<float[][]> dctBlocks;

void setup(){
   size(1280, 720); 
   source = loadImage(sourceFile);
   noFill();
   
   image(source, 0, 0);
}

void draw(){
    
}

ArrayList<float[][]> getDCTBlocks(PImage source){
    ArrayList<float[][]> dctBL = new ArrayList<float[][]>();
    
    for(int y = 0; y < source.height; y += BLOCKSIZE){
      for(int x = 0; x < source.width ; x += BLOCKSIZE){
        PImage blk = source.get(x, y, BLOCKSIZE, BLOCKSIZE);
        dctBL.add(getDCT(blk));
        rect(x, y, BLOCKSIZE, BLOCKSIZE);
      }
    }
    
    return dctBL;
}

float[][] getDCT(PImage blk){
   float[][] dct = new float[BLOCKSIZE][BLOCKSIZE];

   for(int y = 0; y < BLOCKSIZE ; y++){
     for(int x = 0; x < BLOCKSIZE ; x++){
       dct[x][y] = brightness(blk.get(x, y));
     }
   }
 
   //Does 2 DCTs, one horizontal and one vertical
   dct2D.forward(dct, true);
     
   return dct; 
}

void keyPressed() {
  if (key == 'e'){
    //Get Coefficiants
    dctBlocks = getDCTBlocks(source);
    quantise(dctBlocks, qualityJPEG);
    quantise(dctBlocks, qualityJPEG);
    //encoded = reconstructImage(dctBlocks, qualityJPEG); //Reconstruct from quantises DCT coefficiants. We need to know quality is was quantised at.
    //image(encoded, 0, 0); 
  }
  
  if(key == 'o'){
     image(source, 0, 0);
  } 
}

void quantise(ArrayList<float[][]> dctL, int quality){
   for(float[][] blk : dctL){
      quantiseBlock(blk, quality); 
   }
}

void quantiseBlock(float[][] blk, int quality){
   int zeros = 0;
   //For a quality of 100, nothing no quantisation is done
   float scale = 2.0 - quality / 50.0; 
   
   for (int i = 0; i < BLOCKSIZE; i += 1){
     for(int j = 0; j < BLOCKSIZE; j += 1){
        blk[i][j] = round(blk[i][j] / (scale * quantiseLuminance[i][j]));
        if (blk[i][j] == 0){
           zeros += 1; 
        }
     } 
     println("# zeros " + zeros);
   }
}

void dequantiseBlock(float[][] blk, int quality){
   //For a quality of 100, nothing no quantisation is done
   float scale = 2.0 - quality / 50.0; 
   
   for (int i = 0; i < BLOCKSIZE; i += 1){
     for(int j = 0; j < BLOCKSIZE; j += 1){
        blk[i][j] = blk[i][j] * (scale * quantiseLuminance[i][j]);
     } 
   }
}

PImage reconstructImage(ArrayList<float[][]> qdctL, int quality){
  PImage rImage = createImage(source.width, source.height, RGB);
  int c = 0;
 
  for(int y = 0; y < rImage.height; y += BLOCKSIZE){
     for(int x = 0; x < rImage.width; x += BLOCKSIZE){
        dequantiseBlock(qdctL.get(c), quality);
        dct2D.inverse(qdctL.get(c), true);
        PImage p = makePixels(qsctL.get(c));
        rImage.set(x, y, p);
        c += 1; 
     }
  } 
  return rImage;
}

PImage makePixels(float[][] blk){
   PImage p = createImage(BLOCKSIZE, BLOCKSIZE, RGB);
  
  for(int i = 0; i < BLOCKSIZE; i += 1){
     for(int j = 0; j < BLOCKSIZE; j += 1){
        //Set same level for R,G, and B values
       color c = color(blk[i][j], blk[i][j], blk[i][j]);
       p.set(i, j, c); 
     }
  } 
  return p;
}
