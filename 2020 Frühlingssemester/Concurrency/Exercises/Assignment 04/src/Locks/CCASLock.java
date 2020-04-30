package Locks;

public class CCASLock extends AbstractLock{

    public CCASLock(){
        super();
    }

    @Override
    public void lock(){
        while (true) {
            while (state.get() == LOCKED);
            if(attemptLock())
                return;
        }
    }
}
