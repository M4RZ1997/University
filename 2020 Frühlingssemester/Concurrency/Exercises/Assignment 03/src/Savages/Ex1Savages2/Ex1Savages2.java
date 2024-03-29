package Savages.Ex1Savages2;

import Savages.*;

import java.util.ArrayList;
import java.util.List;


import java.util.concurrent.locks.Lock;

public class Ex1Savages2 implements IExecutor {
    private static final int potSize = 10;
    private int numberOfSavages;
    private IPot foodPot;
    private ICook cookRunnable;
    private Thread cookThread;
    private List<Thread> savageThreads;
    private final ObservableLock lock;

    public Ex1Savages2(int nOSavages){
        this.lock = new ObservableLock();
        this.savageThreads = new ArrayList<>();
        this.numberOfSavages = nOSavages;
        foodPot = new FoodPot(potSize);
        cookRunnable = new Cook(foodPot);
        cookThread = new Thread(cookRunnable);
        for (int i = 0; i < this.numberOfSavages; i++){
            Thread thread = new Thread(new AlwaysHungrySavage(foodPot, cookRunnable, this, numberOfSavages, i, lock));
            this.savageThreads.add(thread);
        }
    }

    @Override
    public void runThreads() {
        for (Thread thread : this.savageThreads) {
            thread.start();
        }
        cookThread.start();
        try {
            for (Thread thread : savageThreads) {
                thread.join();
            }
            cookRunnable.terminate();
            cookThread.join();
        } catch (Exception e) {
            System.out.println("Threads were interrupted");
        }
    }

    @Override
    public boolean isAlwaysHungry() {
        return true;
    }

    public static void main(String[] args) {
        IExecutor executor = new Ex1Savages2(7);
        System.out.println("START");
        executor.runThreads();
        System.out.println("FINISHED");
    }

    @Override
    public Lock getLock() {
        return this.lock;
    }
}
