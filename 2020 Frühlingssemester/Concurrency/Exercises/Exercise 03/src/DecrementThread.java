public class DecrementThread implements Runnable{
    private ICounter counter;
    private int i;

    public DecrementThread(ICounter counter, int i){
        this.counter = counter;
        this.i = i;
    }

    @Override
    public void run(){
        for (int j = 0; j < i; j++) {
            counter.decrement();
        }
    }
}
