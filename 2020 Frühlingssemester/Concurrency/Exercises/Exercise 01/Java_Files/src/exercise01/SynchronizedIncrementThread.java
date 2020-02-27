package exercise01;

public class SynchronizedIncrementThread implements Runnable{
    private int repetitions;

    public SynchronizedIncrementThread(int rep){
        this.repetitions = rep;
    }
    @Override
    public void run(){
        for (int i = 0; i < repetitions; i++) {
            IncrementManager.getCounter().syncIncrement();
        }
    }
}
