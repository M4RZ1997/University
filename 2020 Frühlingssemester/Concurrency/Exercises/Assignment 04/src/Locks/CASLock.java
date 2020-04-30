package Locks;

public class CASLock extends AbstractLock{
    public CASLock(){
        super();
    }

    @Override
    public void lock(){
        while (!attemptLock());
    }
}
