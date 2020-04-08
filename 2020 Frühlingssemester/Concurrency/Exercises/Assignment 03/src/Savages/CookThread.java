package Savages;

import java.util.Observable;

public class CookThread extends Observable implements ICookThread {
    private IPot foodPot;
    private boolean work = true;
    private volatile boolean orderFlag;

    public CookThread(IPot pot){
        this.foodPot = pot;
    }

    public void terminate(){ work = false;}

    @Override
    public void run() {
        while (work){
            if (orderFlag){
                try{
                    cookAndRefill();
                }catch(PotNotEmptyException e){}
            }
        }
    }

    @Override
    public void cookAndRefill() throws PotNotEmptyException {
        orderFlag = false;
        if (foodPot.isNotEmpty())
            throw new PotNotEmptyException();
        System.out.println("I have refilled.");
        foodPot.refill();
        this.notifyObservers();
    }

    @Override
    public void order() {
        orderFlag = true;
    }

    @Override
    public boolean isPreparing() {
        return orderFlag;
    }
}
