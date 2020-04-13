package Savages.Ex1Savages1;

import Savages.AbstractSavage;
import Savages.ICook;
import Savages.IExecutor;
import Savages.IPot;

public class OnceHungrySavage extends AbstractSavage {

    public OnceHungrySavage(IPot pot, ICook ct, IExecutor exec){
        super(pot, ct, exec);
    }

    @Override
    protected void initWaitForTurn() {
        //No fairness required
    }

    @Override
    protected void waitForTurn() {
        //No fairness required
    }
}
