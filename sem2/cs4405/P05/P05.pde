import ddf.minim.*;
import ddf.minim.ugens.*;
import edu.emory.mathcs.jtransforms.dct.FloatDCT_1D;

Minim              minim;
//allow us to read in samples
MultiChannelBuffer sampleBuffer;
AudioOutput        output;
//Present audio at correct sampling rate to output, match sample rate of output and input
Sampler            sampler;
float              sampleRate;

//Different Range of frequencies in both audio 
String             audioFile2 = "DontTouchThatDial02.wav";
String             audioFile  = "bach-extract.wav";
//frequency representation of cosine functions at different frequensies.
//They occur in diferent amount, so therefor they can be quanitised to zero. 
//Quantise too much an you loose detail.
FloatDCT_1D       dct_1d;


// quantisation 
float [] quantize(float [] data, float scale) {
  float [] res = new float[data.length];
  int count = 0;

  for (int i = 0; i < data.length; i+= 1) {
    //round to nearest integer, remove precision, this will give us the losses

    res[i] = round(data[i] / scale);
  }

  return res;
}

float [] dequantize(float [] data, float scale) {
  float [] res = new float[data.length];
  int count = 0;

  for (int i = 0; i < data.length; i+= 1) {
    res[i] = data[i] * scale;
  }

  return res;
}


void setup() {
  size(512, 200);
  minim  = new Minim(this);
  output = minim.getLineOut();
  sampleBuffer = new MultiChannelBuffer( 1, 1024 );
}


void draw() {
  drawSample();
}

void drawSample()
{
  background(0);
  stroke(255);

  for (int i = 0; i < output.bufferSize() - 1; i += 1)
  {
    //map samples to display window
    float x1 = map(i, 0, output.bufferSize(), 0, width);
    float x2 = map(i+1, 0, output.bufferSize(), 0, width);
    //normalise
    line(x1, 50 - output.left.get(i)*50, x2, 50 - output.left.get(i+1)*50);
    line(x1, 150 - output.right.get(i)*50, x2, 150 - output.right.get(i+1)*50);
  }
}

void keyPressed() 
{
  if ( key == ' ')
  {
    //load file
    sampleRate = minim.loadFileIntoBuffer( audioFile, sampleBuffer );
    //match sample rate, no per second -> if not matched we will speed up or slow down, or frequency represenatations
    sampler = new Sampler( sampleBuffer, sampleRate, 1 );
    sampler.patch( output );
    sampler.trigger();
  }

  if (key == 'e') {
    //ENCODING
    //take original audio,
    float[] data = sampleBuffer.getChannel(0); //left channel
    //apply dct transfomr,
    dct_1d = new FloatDCT_1D(data.length);
    dct_1d.forward(data, false); 
    //then quantise,
    float[] qdata = quantize(data, 100); //add different numbers later
    
    //DECODING
    //how do i efficiently represent the data I decided to keep? 
    //then reconstruct data, 
    float[] rdata = dequantize(qdata, 100);  //add different numbers later
    dct_1d.inverse(rdata, false); //fasle for no scaling
    //then play that back
    playReconstructedData(rdata);
  }
}


// play the reconstructed audio data 
void playReconstructedData(float [] data) {
  MultiChannelBuffer rsampleData  = new MultiChannelBuffer(data.length, 1);
  //put data in channel
  rsampleData.setChannel(0, data);
  //all we done is change the representation
  sampler = new Sampler(rsampleData, sampleRate, 1);
  sampler.patch(output);
  sampler.trigger();
}

//in practise dont apply dct to all audio, use frames instead
//If we divide and multiple by same scale, the audio will remain the same
//Scale too large we will loss frequencies, will return the average
//DCT will vary for differnt audio


