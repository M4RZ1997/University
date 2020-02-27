package exercise01;

import exercise01.Counter;

public class IncrementThread implements Runnable{
    private int repetitions;

    public IncrementThread(int rep){
        this.repetitions = rep;
    }
    @Override
    public void run(){
        for (int i = 0; i < repetitions; i++) {
            IncrementManager.getCounter().increment();
        }
    }
}
