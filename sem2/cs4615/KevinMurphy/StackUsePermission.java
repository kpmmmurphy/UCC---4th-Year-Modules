import java.security.*;

public final class StackUsePermission extends BasicPermission {

    public StackUsePermission(String name)
    {
	super(name);
    }

    public StackUsePermission(String name, String actions) 
    {
	super(name, actions);
    }
}
