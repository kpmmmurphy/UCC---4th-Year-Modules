import java.io.*;
import java.net.*;
import java.lang.*;
import java.security.*;
import javax.crypto.*;
import java.util.*;
import sun.misc.*;

public class Client
{

  public Client()
  {
  }
  
  public Key readInKey(String Name)
  {
    File clientSecKey = new File(Name + ".sec");
    if(clientSecKey.exists())          // There is a Secret Key file...
    {
      try
      {
        ObjectInputStream oin = new ObjectInputStream( 
                    new FileInputStream(clientSecKey));
        Key key = (Key)oin.readObject();  // read in the key
        oin.close();            // close the file
        return(key);            // return the key
      }
      catch(Exception e)
      {
        System.out.println(e.getMessage());
        e.printStackTrace();
        return(null);
      }
    }
    else
      return(null);
  }

  /*
   * Given Challenge and Key, compute and return the response
   */
  public String createResponse(String Challenge, Key clientkey)
  {
    try
    {
        // your code goes here! //////////////////////////////////
        System.out.println("Challenge: " + Challenge);

        //CREATE CIPHER
        Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");

        //INIT CIPHER MODE
        cipher.init(Cipher.ENCRYPT_MODE, clientkey);

        //DECRYPT
	byte[] challengeBytes = Challenge.getBytes();
        byte[] decryptedBytes = cipher.doFinal(challengeBytes);

        //Base64 encode and print
	BASE64Encoder encoder = new BASE64Encoder();

        String BASE64_PlaintextChallenge = encoder.encode(decryptedBytes);
        System.out.println("BASE64_PlaintextChallenge: " + BASE64_PlaintextChallenge);

	return BASE64_PlaintextChallenge;
    }
    catch(Exception e)
    {
      e.printStackTrace();
      return(null);
    }
  }

  public static void main(String[] args) throws IOException 
  {
    Client thisClient   = new Client();
    BufferedReader din  = null;
    BufferedWriter dout = null;
    Socket ClientSock   = new Socket("csg21-01.ucc.ie",3002);
    String Name     = "kpm2";  // This should be changed to your username
    
    boolean keyfilefound = false;
    String response=null;

    try
    {
      din = new BufferedReader(new InputStreamReader(
                   ClientSock.getInputStream()));
      dout = new BufferedWriter(new OutputStreamWriter(
                   ClientSock.getOutputStream()));
    }
    catch(IOException ioe)
    {
      System.out.println(ioe.getMessage());
      ioe.printStackTrace();
      System.exit(1);
    }

    System.out.println("Client Started...");
    
    String challenge = din.readLine();

    // your code goes here! //////////////////////////////////
    // compute challenge, send to Server, and then 
    // see how you did.

	String encrypted_challenge = thisClient.createResponse(challenge, thisClient.readInKey(Name));
	response = Name + "," + encrypted_challenge;

	//....  
	  dout.write(response,0,response.length());  
	  dout.newLine();
	  dout.flush();
	  System.out.println("Server Response: " + din.readLine());  
    
    try
    {
      ClientSock.close();
    }
    catch(IOException ioe)
    {
      System.out.println(ioe.getMessage());
      ioe.printStackTrace();
    }  
    
  }
}
