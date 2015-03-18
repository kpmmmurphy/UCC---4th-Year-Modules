import java.io.File;
import java.util.Iterator;
import javax.imageio.*;
import javax.imageio.stream.*;
import javax.imageio.stream.FileImageOutputStream;
import java.awt.image.BufferedImage;

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
