package exercise01;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class LockedCounter implements ICounter{
    private Lock lock;
    private int count;
    private int syncCount;
    private int stSyncCount;

    public LockedCounter(){
        lock = new ReentrantLock(true);
        count = 0;
    }

    @Override
    public int getCount(){
        lock.lock();
        return count;
    }

    @Override
    public void setCount(int c){
        count = c;
        lock.unlock();
    }
}
