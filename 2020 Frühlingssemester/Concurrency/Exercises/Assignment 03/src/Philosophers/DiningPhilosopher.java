package Philosophers;

public class DiningPhilosopher implements IPhilosopher {

    private final IFork leftFork;
    private final IFork rightFork;
    private static final int SLEEP_TIME = 100;
    private static final int ITERATIONS = 100;

    public DiningPhilosopher(IFork left, IFork right) {
        this.leftFork = left;
        this.rightFork = right;
    }

    @Override
    public void run() {
        for (int i = 0; i < ITERATIONS; i++) {
            this.eat();
            this.sleep();
        }
    }

    @Override
    public void eat() {
        try {
            this.acquireForks();
            System.out.println(this + ": I am eating");
        } finally {
            this.releaseForks(true, true);
        }
    }

    @Override
    public void sleep() {
        try {
            System.out.println(this + ": zzzzzzzzzzzzzz");
            Thread.sleep((long) Math.random() * SLEEP_TIME);
        }catch(InterruptedException e){}
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
