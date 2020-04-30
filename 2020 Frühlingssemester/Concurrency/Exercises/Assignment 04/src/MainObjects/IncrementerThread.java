package MainObjects;

import java.util.concurrent.locks.Lock;


public class IncrementerThread implements Runnable{
    private Lock filter;
    private Counter counter;
    private static final int MAX_COUNT = 300000;
    private ThreadManager threadManager;

    public IncrementerThread(ThreadManager trdMngr, Lock ftr, Counter ctr){
        this.threadManager = trdMngr;
        this.filter = ftr;
        this.counter = ctr;
    }

    @Override
    public void run() {
        int turns = 0;
        while (counter.getCount() < MAX_COUNT){
            try
            {
                filter.lock();
                if (counter.getCount() < MAX_COUNT) {
                    counter.increment();
                    turns++;
                }
            }catch(Exception e){}
            finally{
                filter.unlock();
            }
        }
        threadManager.setTurns(turns, Thread.currentThread());
    }
}
