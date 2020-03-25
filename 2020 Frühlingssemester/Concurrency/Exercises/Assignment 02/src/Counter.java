public class Counter {
    private volatile int counter;

    public Counter(){
        counter = 0;
    }

    public void increment(){
        counter++;
    }

    public int getCount(){
        return counter;
    }
}
