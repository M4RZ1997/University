package producerCustomer;

import java.util.concurrent.TimeoutException;
import java.util.concurrent.locks.Lock;

public class CustomerThread implements Runnable {
    private int produceNumber;
    private Data data;
    private final int BIG_NUMBER = 100000;
    private DataDepositBuffer dataStorage;
    private Lock customerLock;

    public CustomerThread(DataDepositBuffer ddb, Lock lock){
        this.dataStorage = ddb;
        this.customerLock = lock;
    }

    @Override
    public void run() {
        while (true){
            while (dataStorage.isEmpty()){}
            customerLock.lock();
            try {
                int index = dataStorage.getNextBufferPlace(this);
                while (!dataStorage.isOccupied(index)) {
                    index = dataStorage.getNextBufferPlace(this);
                }
                data = dataStorage.getData(index);
                System.out.println(data.talk() + " from deposit " + index);
            }catch(Exception e){
                System.out.println("Data-Storage Failure from Customer");
                e.printStackTrace();
            }finally {customerLock.unlock();}
            try{
                long waitTime = (long) (Math.random() * 1000) + 1000L; // You can change the number sto increase or decrease the waiting time
                //wait(waitTime);
                Thread.sleep(waitTime);
            }catch(InterruptedException e){
                System.out.println("Interruption Exception from Customer");
            }
        }
    }
}
