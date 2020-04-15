package Philosophers;

public class DiningPhilosopher implements IPhilosopher {

    private final IFork leftFork;
    private final IFork rightFork;
    private static final int SLEEP_TIME = 1000;

    private boolean hasEaten;
    private int eatCounter = 0;
    private int identificationNumber;
    private Executor executor;

    public DiningPhilosopher(IFork left, IFork right, int id, Executor exec) {
        this.leftFork = left;
        this.rightFork = right;
        this.identificationNumber = id;
        this.executor = exec;
    }

    @Override
    public void run() {
        while (true) {
            hasEaten = false;
            this.eat();
            hasEaten = true;
            this.sleep();
            while (executor.getPhilosopher(identificationNumber-1).hasNotEatenYet() && executor.getPhilosopher(identificationNumber+1).hasNotEatenYet()){}
        }
    }

    @Override
    public void eat() {
        try {
            this.acquireForks();
            System.out.println("Thread "+ identificationNumber + ": I am eating " + ++eatCounter + "  times");
        } finally {
            this.releaseForks(true, true);
        }
    }

    @Override
    public void sleep() {
        try {
            System.out.println("Thread "+ identificationNumber + ": zzzzzzzzzzzzzz");
            Thread.sleep((long) Math.random() * SLEEP_TIME + SLEEP_TIME);
        }catch(InterruptedException e){}
    }

    @Override
    public boolean hasNotEatenYet() {
        return !hasEaten;
    }

    private void acquireForks() {
        boolean success = false;
        while (!success) {
            boolean lsuccess = this.leftFork.setTaken();
            boolean rsuccess = this.rightFork.setTaken();
            success = lsuccess && rsuccess;
            // If it was not possible to acquire both forks, put forks back on table, to avoid deadlocks
            if (!success) {
                releaseForks(lsuccess, rsuccess);
            }
        }
    }

    private void releaseForks(boolean l, boolean r) {
        if (l)
            this.leftFork.setNotTaken();
        if (r)
            this.rightFork.setNotTaken();
    }

    @Override
    public String toString() {
        return super.toString().split("@")[1];
    }
}
