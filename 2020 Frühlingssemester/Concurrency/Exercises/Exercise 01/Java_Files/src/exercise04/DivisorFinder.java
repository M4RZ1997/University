package exercise04;

import java.util.ArrayList;
import java.util.List;

public class DivisorFinder {
    private static final int threadAmount = 10;
    private static final int lowerBound = 1;
    private static final int upperBound = 100000;
    private List<Thread> threadList;
    private int numberOfInterest;
    private static int maxDivisors = 0;
    private static int maxNumber = 0;


    public DivisorFinder(){
        this.threadList = new ArrayList<>();
        numberOfInterest = lowerBound;
        for (int i = 0; i < threadAmount; i++){
            this.threadList.add(new Thread(new NumberHandlerThread(this)));
        }
    }

    public void runThreads(){
        for (Thread thread : this.threadList){
            thread.start();
        }
        try {
            for (Thread thread : threadList){
                thread.join();
            }
        }catch(Exception e){
            System.out.println("Threads were interrupted");
        }
    }

    public synchronized void setMaxDivisorsAndMaxNumber(int maxDivisors, int maxNumber) {
        if (this.maxDivisors < maxDivisors){
            this.maxDivisors = maxDivisors;
            this.maxNumber = maxNumber;
        }
    }

    public synchronized int getNumberAndIncrement(){
        if (numberOfInterest <= upperBound) {
            int temp = numberOfInterest;
            numberOfInterest += 1;
            return temp;
        }
        else {
            return 0;
        }
    }

    public static void main(String args[]){
        DivisorFinder finder = new DivisorFinder();
        long startingTime = System.nanoTime();
        finder.runThreads();
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        System.out.println("The number " + maxNumber + " has " + maxDivisors + " divisors");
        System.out.println("Past time [ms]: " + pastTime);
    }
}
