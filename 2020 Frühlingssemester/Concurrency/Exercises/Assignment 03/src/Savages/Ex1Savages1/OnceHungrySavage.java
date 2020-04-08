package Savages.Ex1Savages1;

import Savages.AbstractSavage;
import Savages.ICookThread;
import Savages.IExecutor;
import Savages.IPot;

public class OnceHungrySavage extends AbstractSavage {

    public OnceHungrySavage(IPot pot, ICookThread ct, IExecutor exec){
        super(pot, ct, exec);
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
            }
        } while (this.executor.isAlwaysHungry());
    }
}
