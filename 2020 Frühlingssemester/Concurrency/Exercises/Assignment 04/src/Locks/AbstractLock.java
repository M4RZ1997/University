package Locks;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;

public abstract class AbstractLock implements Lock {

    protected static final int LOCKED = 1;
    protected static final int UNLOCKED = 0;

    protected AtomicInteger state;

    public AbstractLock(){
        this.state = new AtomicInteger();
    }

    public boolean attemptLock(){
        return this.state.compareAndSet(UNLOCKED, LOCKED);
    }

    @Override
    public void unlock(){
        this.state.set(UNLOCKED);
    }

    @Override
    public void lockInterruptibly() throws InterruptedException {
        //Not implemented
    }

    @Override
    public boolean tryLock() {
        //Not implemented
        return false;
    }

    @Override
    public boolean tryLock(long l, TimeUnit timeUnit) throws InterruptedException {
        //Not implemented
        return false;
    }

    @Override
    public Condition newCondition() {
        //Not implemented
        return null;
    }
}
