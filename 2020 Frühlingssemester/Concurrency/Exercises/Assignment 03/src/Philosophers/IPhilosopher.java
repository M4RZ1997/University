package Philosophers;

public interface IPhilosopher extends Runnable {
    void eat();
    void sleep();
    boolean hasNotEatenYet();
}
