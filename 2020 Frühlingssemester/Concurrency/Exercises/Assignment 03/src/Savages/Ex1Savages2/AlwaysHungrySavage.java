package Savages.Ex1Savages2;

import Savages.*;

import java.util.Observable;
import java.util.Observer;

public class AlwaysHungrySavage extends AbstractSavage implements Observer{
    private int numberOfSavages;

    private volatile int countdown;

    public AlwaysHungrySavage(IPot pot, ICook ct, IExecutor exec, int nOSavages, int id, ObservableLock lock){
        super(pot, ct, exec);
        this.numberOfSavages = nOSavages;

        lock.addObserver(this);
    }

    @Override
    protected void waitForTurn() {
        while (this.countdown > 0);
    }

    @Override
    public void update(Observable observable, Object o) {
        //Count how many times critical section was accessed
        if (observable instanceof ObservableLock) {
            if (o instanceof String && o.equals(ObservableLock.UNLOCKED)) {
                this.countdown--;
            }
        }
    }

    @Override
    protected void initWaitForTurn() {
        this.countdown = numberOfSavages;
    }
}
