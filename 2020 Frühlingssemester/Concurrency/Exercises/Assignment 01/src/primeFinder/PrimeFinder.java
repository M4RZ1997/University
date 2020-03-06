package primeFinder;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class PrimeFinder {
    private static final int CORE_NUMBER = 4;
    private TicketManager ticketManager;
    private Thread referenceThread;
    private List<Thread> threads;
    private List<Thread> exactThreads;
    private static List<Integer> primes;
    private static List<Integer> exactPrimes;
    private static List<Long> times;

    public PrimeFinder(){
        this.threads = new ArrayList<>();
        this.exactThreads = new ArrayList<>();
        this.ticketManager = new TicketManager();
        for (int i=0; i < CORE_NUMBER; i++){
            this.threads.add(new Thread(new RangePrimeFinderThread(i, CORE_NUMBER, this)));
        }
        for (int i=0; i < CORE_NUMBER; i++){
            this.exactThreads.add(new Thread(new ExactPrimeFinderThread(ticketManager, this)));
        }
        this.ticketManager = new TicketManager();
        referenceThread = new Thread(new ExactPrimeFinderThread(ticketManager, this));

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
    public void runExactThreads(){
        for (Thread thread : this.exactThreads){
            thread.start();
        }
        try {
            for (Thread thread : exactThreads){
                thread.join();
            }
        }catch(Exception e){
            System.out.println("Threads were interrupted");
        }
    }
    public void runReferenceThread(){
        referenceThread.start();
        try {
            referenceThread.join();
        }catch(Exception e){
            System.out.println("Thread was interrupted");
        }
    }

    public synchronized void addTime(long time){
        times.add(time);
    }

    public synchronized void addPrimes(List<Integer> list){
        primes.addAll(list);
    }

    public synchronized void addExactPrimes(List<Integer> list){
        exactPrimes.addAll(list);
    }

    public static void main(String args[]){
        System.out.println();
        primes = new ArrayList<>();
        exactPrimes = new ArrayList<>();
        times = new ArrayList<>();
        PrimeFinder primeFinder = new PrimeFinder();
        long startingTime = System.nanoTime();
        primeFinder.runThreads();
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("Number of Primes: " + primes.size());
        System.out.println("Time with RangeFinder: " + pastTime + " ms");
        for (int i = 0; i < times.size(); i++){
            System.out.println("Thread " + i + " finished at time: " + times.get(i) + " ms");
        }
        System.out.println();
        times = new ArrayList<>();
        startingTime = System.nanoTime();
        primeFinder.runExactThreads();
        pastTime = (System.nanoTime() - startingTime)/1000000;
        Collections.sort(exactPrimes);
        System.out.println("Number of Primes: " + exactPrimes.size());
        System.out.println("Time with TicketManager: " + pastTime + " ms");
        for (int i = 0; i < times.size(); i++){
            System.out.println("Thread " + i + " finished at time: " + times.get(i) + " ms");
        }
        System.out.println();
        exactPrimes = new ArrayList<>();
        startingTime = System.nanoTime();
        primeFinder.runReferenceThread();
        pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("Number of Primes: " + exactPrimes.size());
        System.out.println("Reference-Time with 1 Thread: " + pastTime + " ms");
    }
}
