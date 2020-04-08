package Savages.Ex1Savages2;

import Savages.*;

import java.util.Observable;
import java.util.Observer;

public class AlwaysHungrySavage extends AbstractSavage implements Observer{
    private int numberOfSavages;
    private int identificationNumber;
    private int eatCounter = 0;

    private int currAmount;
    private int refillCounter;

    public AlwaysHungrySavage(IPot pot, ICookThread ct, IExecutor exec, int nOSavages, int id){
        super(pot, ct, exec);
        if (ct instanceof CookThread)
            ((CookThread)ct).addObserver(this);
        this.numberOfSavages = nOSavages;
        this.identificationNumber = id;
    }

    @Override
    public void run() {
        do {
            IExecutor.lock.lock();
            try{
                this.eat();
                System.out.println("I have eaten!");
            } finally {
                IExecutor.lock.unlock();
                this.waitForTurn();
            }
        } while (this.executor.isAlwaysHungry());
    }

    protected void waitForTurn() {
        int nextTryAt = this.foodPot.getFillAmount() - this.numberOfSavages;
        refillCounter = 0;
        while (nextTryAt < 0) {
            refillCounter++;
            nextTryAt += numberOfSavages;
        }
        while (nextTryAt > this.foodPot.getCapacity()) {
            nextTryAt -= this.foodPot.getCapacity();
            refillCounter++;
        }

        while (refillCounter > 0) {}
        while (this.foodPot.getFillAmount() > nextTryAt);
    }

    public void update(Observable observable, Object o) {
        this.refillCounter--;
    }
}
