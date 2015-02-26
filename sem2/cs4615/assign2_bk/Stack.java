import java.io.*;
import java.security.*;

public class Stack {
   private int maxStack;
   private int emptyStack;
   private int top;
   private char[] items;

   private String permissionUse  = "use";
   private String permissionSave = "save";

   public Stack(int size) {
      maxStack= size;
      emptyStack = -1;
      top = emptyStack;
      items = new char[maxStack];
   }

   public void push(char c) {
      checkStackPermission(permissionUse);
      items[++top] = c;
   }

   public char pop() {
      checkStackPermission(permissionUse);
      return items[top--];
   }

   public boolean full()  {
      return top + 1 == maxStack;
   }

   public boolean empty()  {
      return top == emptyStack;
   }

   public void save() throws FileNotFoundException, IOException {
	checkStackPermission(permissionSave);
	String path = System.getProperty("user.home") + File.separator + "stackFile";
        File stackFile = new File(path);
	Writer output = new BufferedWriter(new FileWriter(stackFile));	
	for (int i=0; i<=top; i++){
	    output.write(items[i]);
	}
	output.close();
    }

   private void checkStackPermission(String permission){
	SecurityManager sm = System.getSecurityManager();
        if(sm != null){
	    if(permission.equals(permissionUse)){
		sm.checkPermission(new StackUsePermission(permission));
	    }else if(permission.equals(permissionSave)){
		sm.checkPermission(new StackSavePermission(permission));
	    }
        }
   }

}
