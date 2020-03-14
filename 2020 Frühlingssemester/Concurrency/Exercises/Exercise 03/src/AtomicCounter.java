import java.util.concurrent.atomic.AtomicInteger;

public class AtomicCounter implements ICounter{
    private AtomicInteger counter;

    public AtomicCounter(){
        counter = new AtomicInteger();
        this.counter.set(0);
    }

    public void increment(){
        while(!this.counter.compareAndSet(this.counter.get(), this.counter.get() + 1));
    }

    public int getCounter(){
        return this.counter.get();
    }
}
