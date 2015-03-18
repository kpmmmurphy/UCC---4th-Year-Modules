import edu.emory.mathcs.jtransforms.dct.FloatDCT_2D;

PImage source;
String sourceFile = "Original-me.png";

final int BLOCKSIZE = 8;

FloatDCT_2D dct2D = new FloatDCT_2D(BLOCKSIZE, BLOCKSIZE);
ArrayList<float[][]> dctBlocks;

void setup(){
   size(1280, 720); 
   source = loadImage(sourceFile);
   noFill();
   
   image(source, 0, 0);
   
   dctBlocks = getDCTBlocks(source);
   print(dctBlocks.get(2).toString());
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
