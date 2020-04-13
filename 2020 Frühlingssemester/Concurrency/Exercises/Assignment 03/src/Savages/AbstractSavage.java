package Savages;

import java.util.Observable;

public abstract class AbstractSavage implements ISavage {
    protected IPot foodPot;
    protected ICook cook;
    protected IExecutor executor;
    private int counter;

    public AbstractSavage(IPot pot, ICook ct, IExecutor exec){
        this.foodPot = pot;
        this.cook = ct;
        this.executor = exec;
        this.counter = 0;
    }

    @Override
    public void eat() {
        if (foodPot.isEmpty() && !cook.isPreparing()) {
            System.out.println("I have ordered");
            orderRefill();
        }
        while (foodPot.isEmpty()) {}
        try{
            this.foodPot.getFood();
        }catch(PotIsEmptyException e){};
    }

    @Override
    public void run() {
        do {
            this.executor.getLock().lock();
            try {
                this.eat();
                System.out.println("Thread " + this + " has eaten " + ++counter + " times");
            } finally {
                this.initWaitForTurn();
                this.executor.getLock().unlock();
                this.waitForTurn();
            }
        } while (this.executor.isAlwaysHungry());
    }

    @Override
    public void orderRefill() {
        cook.order();
    }

    @Override
    public void update(Observable observable, Object o) {
        //Do nothing in case fairness is not required
        //Override if something should happen
    }

    @Override
    public String toString() {
        return super.toString().split("@")[1];
    }

    protected abstract void initWaitForTurn();
    protected abstract void waitForTurn();
}
