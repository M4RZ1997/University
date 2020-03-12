import java.util.concurrent.atomic.AtomicInteger;

public class AtomicCounter implements ICounter{
    private AtomicInteger counter;

    public AtomicCounter(){
        counter = new AtomicInteger();
        this.counter.set(0);
    }

    public void increment(){
        int old = this.counter.get();
        this.counter.compareAndSet(old, ++old);
    }

    public void decrement(){
        int old = this.counter.get();
        this.counter.compareAndSet(old, --old);
    }

    public int getCounter(){
        return this.counter.get();
    }
}
