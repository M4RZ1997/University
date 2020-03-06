package exercise01;

public class Counter implements ICounter {
    private int count;
    private int syncCount;
    private int stSyncCount;

    public Counter(){
        count = 0;
    }

    @Override
    public int getCount(){
        return count;
    }

    @Override
    public void setCount(int c){
        count = c;
    }
}
