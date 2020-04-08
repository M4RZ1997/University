import java.util.concurrent.atomic.AtomicInteger;

public class Counter {
    private /*volatile*/ int counter;

    public Counter(){
        counter = 0;
    }

    public void increment(){
        counter++;
    }
}
