import processing.video.*;
//Frames are compressed using motion jpeg
//We must expand this out into individual picturess

String videoFile = "Racers-ref.mov";
Movie sourceMovie;

ArrayList<PImage> lumaPictures;
ArrayList<int[]> motionVectors;

int currentFrame = 0;

final int MBSIZE = 16;
final int SEARCH_WINDOWX = MBSIZE * 7;
final int SEARCH_WINDOWY = MBSIZE * 5;

void setup(){
 size(1000, 1000);
 //match to movie
 frameRate(25);
 
 lumaPictures  = new ArrayList<PImage>();
 motionVectors = new ArrayList<int[]>();
 sourceMovie = new Movie(this, videoFile);
 sourceMovie.play();
}

void movieEvent(Movie m){
  //A lot of work must take place here
  m.read();
  //Create a lumanince version of the current movie frame
  //and add it to the arraylist
  lumaPictures.add(createLuma(m));
}

PImage createLuma(PImage frame){
  PImage lpicture = createImage(frame.width, frame.height, RGB);
  
  //Load linear array for efficiant
  lpicture.loadPixels();
  frame.loadPixels();
  
  for(int i = 0; i < frame.width * frame.height; i += 1){
    //create lumanence representation of the pixel 
    //take so much of the R, G, and B
    float Y = red(frame.pixels[i]) * 0.2126 + 
              green(frame.pixels[i]) * 0.71529 +
              blue(frame.pixels[i]) * 0.0722;
    //Stop it from saturating
    Y = min(Y, 255);
    
    lpicture.pixels[i] = color(Y, Y, Y);
  }
  lpicture.updatePixels();
  
  return lpicture;
}

void draw(){
   if(lumaPictures.size() > 0){
      image(lumaPictures.get(currentFrame), 0, 0);
      currentFrame = (currentFrame + 1) % lumaPictures.size();
   } 
}

void keyPressed(){
   if(key == 'e'){
      makePredictedPicture(currentFrame);
   } 
}

//assuming ibbP and making P
void makePredictedPicture(int n){
   //IBBP - REFEREENCE IS THREE FRAMES BACK
  int reference = n - 3;
  
  //Predicting from
  image(lumaPictures.get(reference), 100, 200);
  //
  image(lumaPictures.get(n), 500, 200);
  
  //using original luma picture to do prediction
  //an encoder must user the reference first, then
  //reconstruct reference and use it as the source for prediction
  
  //motion vector for each macrobock
  motionVectors = getMotionVectors(reference, n);
  
}

ArrayList<int[]> getMotionVectors(int rn, int cn){
  PImage r = lumaPictures.get(rn); //should be reconstructed version of reference picture
  PImage c = lumaPictures.get(cn); 
  
  ArrayList<int []> mvList = new ArrayList<int []>();
  
  for(int y = 0; y < c.height; y += MBSIZE){
     for(int x = 0; x < c.width; x += MBSIZE){
        mvList.add(bestMatch(x, y, r, c));
     } 
  }
  return mvList;
}

int[] bestMatch(int x, int y, PImage r, PImage c){
   int[] mv = new int[2];
   int rx0 = max(0, x - SEARCH_WINDOWX / 2);
   int rx1 = min(x + SEARCH_WINDOWX / 2, r.width);
   int ry0 = max(0, y - SEARCH_WINDOWY / 2);
   int ry1 = min(y + SEARCH_WINDOWY / 2, r.height);
   
   //searching everywhere, and with a 1 pixel offset
   float best = 1.0e7;
   for(int i = ry0; i < ry1; i += 1){
     for(int j = rx0; j < rx1; j += 1 ){
        float current = mbMatch(j, i, x, y, r, c);
        if(current < best){
           best  = current;
           mv[0] = j - x;
           mv[1] = i - y;
        } 
     }
   }
   return mv;
}

float mbMatch(){
    //compare pixel values, subtract and get the differences
}
