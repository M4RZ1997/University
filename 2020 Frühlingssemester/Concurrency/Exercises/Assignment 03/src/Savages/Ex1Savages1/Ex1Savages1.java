package Savages.Ex1Savages1;

import Savages.*;

import java.util.ArrayList;
import java.util.List;

public class Ex1Savages1 implements IExecutor {
    private static final int potSize = 5;
    private int numberOfSavages;
    private IPot foodPot;
    private ICookThread cookRunnable;
    private Thread cookThread;
    private List<Thread> savageThreads;


    public Ex1Savages1(int nOSavages){
        this.savageThreads = new ArrayList<>();
        this.numberOfSavages = nOSavages;
        foodPot = new FoodPot(potSize);
        cookRunnable = new CookThread(foodPot);
        cookThread = new Thread(cookRunnable);
        for (int i = 0; i < this.numberOfSavages; i++){
            Thread thread = new Thread(new OnceHungrySavage(foodPot, cookRunnable, this));
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
        return false;
    }

    public static void main(String[] args) {
        IExecutor executor = new Ex1Savages1(7);
        System.out.println("START");
        executor.runThreads();
        System.out.println("FINISHED");
    }
}
