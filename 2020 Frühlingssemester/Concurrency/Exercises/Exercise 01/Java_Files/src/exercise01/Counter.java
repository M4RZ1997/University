package exercise01;

public class Counter {
    private int count;
    private int syncCount;
    private int stSyncCount;

    public Counter(){
        count = 0;
    }

    public void increment(){
        count++;
    }

    public synchronized void syncIncrement(){
        syncCount++;
    }

    public void stSyncIncrement(){
        synchronized(this){
            stSyncCount++;
        }
    }

    public int getCount(){
        return count;
    }

    public int getSyncCount(){
        return syncCount;
    }

    public int getStSyncCount(){
        return stSyncCount;
    }

}
