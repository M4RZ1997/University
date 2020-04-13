package Savages;

import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ObservableLock extends Observable implements Lock {

    public static final String LOCKED = "locked";
    public static final String UNLOCKED = "unlocked";

    private final Lock lock;

    public ObservableLock(List<Observer> observers) {
        this();
        for (Observer observer : observers) {
            this.addObserver(observer);
        }
    }

    public ObservableLock() {
        this.lock = new ReentrantLock();
    }

    @Override
    public void lock() {
        this.lock.lock();
        this.notifyLocked();
    }

    @Override
    public void lockInterruptibly() throws InterruptedException {
        this.lock.lockInterruptibly();
        this.notifyLocked();
    }

    @Override
    public boolean tryLock() {
        boolean success = this.lock.tryLock();
        if (success) {
            this.notifyLocked();
        }
        return success;
    }

    @Override
    public boolean tryLock(long l, TimeUnit timeUnit) throws InterruptedException {
        boolean success = this.lock.tryLock(l, timeUnit);
        if (success) {
            this.notifyLocked();
        }
        return success;
    }

    @Override
    public void unlock() {
        this.notifyUnlocked();
        this.lock.unlock();
    }

    @Override
    public Condition newCondition() {
        return this.lock.newCondition();
    }

    private void notifyLocked() {
        this.setChanged();
        this.notifyObservers(LOCKED);
    }

    private void notifyUnlocked() {
        this.setChanged();
        this.notifyObservers(UNLOCKED);
    }
}
