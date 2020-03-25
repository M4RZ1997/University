import java.util.HashMap;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicIntegerArray;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;

public class Filter implements Lock {
    private int[] level;
    private AtomicIntegerArray victim;
    private int n;
    private HashMap<Long, Integer> threadMap;

    public Filter(int n, HashMap<Long, Integer> tMap){
        this.threadMap = tMap;
        this.n = n;
        level = new int[n];
        victim = new AtomicIntegerArray(n);
        for (int i = 0; i < n; i++){
            level[i] = 0;
            victim.set(i, -1);
        }
    }

    @Override
    public void lock() {
        int threadID = getThreadID();
        for (int L = 1; L < n; L++){
            level[threadID] = L;
            victim.set(L, threadID);
            while (higherLevelIsOccupied(threadID) && victim.get(level[threadID]) == threadID) {}
        }
    }

    @Override
    public void unlock() {
        int threadID = getThreadID();
        level[threadID] = 0;
    }

    private int getThreadID(){
        long id = Thread.currentThread().getId();
        if (threadMap.containsKey(id)){
            return threadMap.get(id);
        }
        return -1;
    }

    private boolean higherLevelIsOccupied(int threadID){
        boolean exists = false;
        for (int i = 0; i < n; i++) {
            if (i != threadID && !exists){
                exists = (level[i] >= level[threadID]);
            }
        }
        return exists;
    }

    public void setMap(HashMap<Long, Integer> tMap){
        this.threadMap = tMap;
    }

    @Override
    public void lockInterruptibly() throws InterruptedException {
    }

    @Override
    public boolean tryLock() {
        return false;
    }

    @Override
    public boolean tryLock(long time, TimeUnit unit) throws InterruptedException {
        return false;
    }

    @Override
    public Condition newCondition() {
        return null;
    }
}
