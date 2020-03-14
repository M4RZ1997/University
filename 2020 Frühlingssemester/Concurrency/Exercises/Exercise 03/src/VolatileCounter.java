public class VolatileCounter implements ICounter{
    private volatile int counter;

    public VolatileCounter(){
        this.counter = 0;
    }

    public void increment(){
        this.counter++;
    }

    public int getCounter(){
        return this.counter;
    }
}
