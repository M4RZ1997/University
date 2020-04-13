package Savages;

import java.util.concurrent.locks.Lock;

public interface IExecutor {
    Lock getLock();
    boolean isAlwaysHungry();
    void runThreads();
}
