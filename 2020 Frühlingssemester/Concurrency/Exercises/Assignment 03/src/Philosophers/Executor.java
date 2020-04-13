package Philosophers;

import java.util.ArrayList;
import java.util.List;

public class Executor {
    private static final int PHILOSOPHERS_NUMBER = 30;

    protected List<IPhilosopher> philosophers;
    protected List<IFork> forks;
    protected List<Thread> philosopherThreads;

    public Executor(int philosophersNumber){
        this.forks = new ArrayList<>();
        this.philosophers = new ArrayList<>();

        for (int i = 0; i < philosophersNumber; i++) {
            this.forks.add(new PhilosophersFork());
        }

        for (int i = 0; i < philosophersNumber; i++) {
            this.philosophers.add(new DiningPhilosopher(this.forks.get(i), this.forks.get((i + 1) % philosophersNumber)));
        }

        this.philosopherThreads = new ArrayList<>();
        for (IPhilosopher philosopher: this.philosophers) {
            this.philosopherThreads.add(new Thread(philosopher));
        }
    }

    public static void main(String[] args) {
        Executor executor = new Executor(PHILOSOPHERS_NUMBER);
        System.out.println("START");
        executor.runThreads();
        System.out.println("FINISHED");
    }

    protected void runThreads() {
        for (Thread thread : this.philosopherThreads) {
            thread.start();
        }
        try {
            for (Thread thread : this.philosopherThreads) {
                thread.join();
            }
        } catch (Exception e) {
            System.out.println("Threads were interrupted");
        }
    }
}
