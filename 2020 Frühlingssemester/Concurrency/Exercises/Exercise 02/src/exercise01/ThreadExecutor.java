package exercise01;

import java.util.ArrayList;
import java.util.List;

public class ThreadExecutor {
    private static int n;
    private static int m;
    private static int i;
    //private static ICounter counter = new Counter();
    private static ICounter counter = new LockedCounter();
    private static List<Thread> threads;

    public ThreadExecutor(){
        this.threads = new ArrayList<>();
        for(int k = 0; k < n; k++){
            this.threads.add(new Thread(new IncrementThread(counter, i)));
        }
        for(int j = 0; j < m; j++){
            this.threads.add(new Thread(new DecrementThread(counter, i)));
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
    }

    public static void main(String args[]){
        i = Integer.parseInt(args[0]);
        m = Integer.parseInt(args[1]);
        n = Integer.parseInt(args[2]);
        ThreadExecutor executor = new ThreadExecutor();
        long startingTime = System.nanoTime();
        executor.runThreads();
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("Counter:" + counter.getCount());
        System.out.println("Time:" + pastTime + "ms");
    }
}
