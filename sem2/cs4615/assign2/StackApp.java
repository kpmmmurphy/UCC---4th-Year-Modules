import java.io.*;

public class StackApp {

  public static void main(String[] args)
       throws IOException {
    Stack s = new Stack(10); // 10 chars
    char ch;
    while ((ch = (char)System.in.read())!= '\n')
       if (!s.full()) s.push(ch);
    s.save();
    while (!s.empty())
       System.out.print(s.pop());
    System.out.println();
  }
}
