package exercise01;

import java.util.ArrayList;
import java.util.List;

public class IncrementManager {
    private static Counter counter = new Counter();
    private static List<Thread> threads;
    private static List<Thread> syncThreads;
    private static List<Thread> stSyncThreads;

    public IncrementManager(){
        this.threads = new ArrayList<>();
        this.syncThreads = new ArrayList<>();
        this.stSyncThreads = new ArrayList<>();
        for(int i = 0; i < 100; i++){
            this.threads.add(new Thread(new IncrementThread(100000)));
        }
        for(int i = 0; i < 100; i++){
            this.syncThreads.add(new Thread(new SynchronizedIncrementThread(100000)));
        }
        for(int i = 0; i < 100; i++){
            this.stSyncThreads.add(new Thread(new StSynchronizedIncrementThread(100000)));
        }
    }

    public void runThreads(){
        for (Thread thread : this.threads){
            thread.start();
        }
        try {
            for (Thread thread : threads){
                thread.join();
            }
        }catch(Exception e){
            System.out.println("Threads were interrupted");
        }

        for (Thread thread : this.syncThreads){
            thread.start();
        }
        try {
            for (Thread thread : syncThreads){
                thread.join();
            }
        }catch(Exception e){
            System.out.println("Threads were interrupted");
        }

        for (Thread thread : this.stSyncThreads){
            thread.start();
        }
        try {
            for (Thread thread : stSyncThreads){
                thread.join();
            }
        }catch(Exception e){
            System.out.println("Threads were interrupted");
        }
    }

    public static Counter getCounter(){
        return counter;
    }

    public static void main(String args[]){
        IncrementManager manager = new IncrementManager();
        manager.runThreads();
        System.out.println("Counter: " + getCounter().getCount());
        System.out.println("synchronized Counter: " + getCounter().getSyncCount());
        System.out.println("synchronized Statement Counter: " + getCounter().getStSyncCount());
    }
}
