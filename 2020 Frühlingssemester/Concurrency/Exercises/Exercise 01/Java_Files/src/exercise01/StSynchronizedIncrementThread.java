package exercise01;

public class StSynchronizedIncrementThread implements Runnable{
    private int repetitions;

    public StSynchronizedIncrementThread(int rep){
        this.repetitions = rep;
    }
    @Override
    public void run(){
        for (int i = 0; i < repetitions; i++) {
            IncrementManager.getCounter().stSyncIncrement();
        }
    }
}
