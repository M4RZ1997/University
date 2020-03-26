import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ThreadManager {
    private static Counter counter = new Counter();
    private static AtomicIntegerArray turns;
    private static Filter filter;
    private HashMap<Long, Integer> threadMap;
    private List<Thread> threads;

    public ThreadManager(int n){
        this.threads = new ArrayList<>();
        this.threadMap = new HashMap<>();
        this.turns = new AtomicIntegerArray(n);
        filter = new Filter(n, threadMap);
        for(int i = 0; i < n; i++){
            Thread thread = new Thread(new IncrementerThread(this, filter, counter));
            this.threads.add(thread);
            this.threadMap.put(thread.getId(), i);
        }
        filter.setMap(threadMap);
    }

    public void runThreads() {
        for (Thread thread : this.threads) {
            thread.start();
        }
        try {
            for (Thread thread : threads) {
                thread.join();
            }
        } catch (Exception e) {
            System.out.println("Threads were interrupted");
        }
    }

    public void setTurns(int t, Thread thread){
        int id = threadMap.get(thread.getId());
        this.turns.set(id, t);
    }

    public static void main(String args[]){
        ThreadManager manager = new ThreadManager(16);
        long startTime = System.nanoTime();
        manager.runThreads();
        long executionTime = (System.nanoTime() - startTime) / 1000000;
        System.out.println("Counter: " + counter.getCount());
        int totalCount = 0;
        for (int i = 0; i < turns.length(); i++){
            System.out.println("Thread " + i + " has accessed the counter " + turns.get(i) + " times");
            totalCount += turns.get(i);
        }
        System.out.println("Total accesses: " + totalCount);
        System.out.println("Execution Time: " + executionTime + " ms");
    }
}
