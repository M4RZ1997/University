import java.util.ArrayList;
import java.util.List;

public class ThreadExecutor {
    private static int n = 100;
    private static int m = 100;
    private static int i = 10000;
    //private static ICounter counter = new Counter();
    private static ICounter volatileCounter = new VolatileCounter();
    private static ICounter atomicCounter = new AtomicCounter();
    private static List<Thread> threads;

    public ThreadExecutor(ICounter counter){
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
        ThreadExecutor executor = new ThreadExecutor(atomicCounter);
        long startingTime = System.nanoTime();
        executor.runThreads();
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("atomic Counter:" + atomicCounter.getCounter());
        System.out.println("Time:" + pastTime + "ms");
        executor = new ThreadExecutor(volatileCounter);
        startingTime = System.nanoTime();
        executor.runThreads();
        pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("volatile Counter:" + volatileCounter.getCounter());
        System.out.println("Time:" + pastTime + "ms");
    }
}
