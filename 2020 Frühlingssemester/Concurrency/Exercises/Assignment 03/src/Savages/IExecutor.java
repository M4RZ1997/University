package Savages;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public interface IExecutor {
    Lock lock = new ReentrantLock();
    boolean isAlwaysHungry();
    void runThreads();
}
