package Philosophers;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class PhilosophersFork implements IFork {
    private volatile boolean taken;
    private final Lock lock;

    public PhilosophersFork() {
        this.lock = new ReentrantLock();
    }

    @Override
    public boolean isTaken() {
        return taken;
    }

    @Override
    public boolean setTaken() {
        if (this.lock.tryLock()) {
            this.taken = true;
            return true;
        }
        return false;
    }

    @Override
    public void setNotTaken() {
        try {
            this.taken = false;
        } finally {
            this.lock.unlock();
        }
    }
}
