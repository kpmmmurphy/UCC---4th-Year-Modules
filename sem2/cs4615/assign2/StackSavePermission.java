import java.security.*;

public final class StackSavePermission extends BasicPermission {

    public StackSavePermission(String name)
    {
	super(name);
    }

    public StackSavePermission(String name, String actions) 
    {
	super(name, actions);
    }
}
