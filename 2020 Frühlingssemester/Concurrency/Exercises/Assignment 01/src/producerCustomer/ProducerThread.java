package producerCustomer;

import java.util.concurrent.locks.Lock;

public class ProducerThread implements Runnable {
    private int produceNumber;
    private Data data;
    private final int BIG_NUMBER = 100000;
    private DataDepositBuffer dataStorage;
    private Lock producerLock;

    public ProducerThread(DataDepositBuffer ddb, Lock lock){
        this.dataStorage = ddb;
        this.producerLock = lock;
    }

    @Override
    public void run() {
        while (true){
            produceNumber = (int) (Math.random() * BIG_NUMBER);
            data = new Data(produceNumber);
            while (dataStorage.isFull()){}
            producerLock.lock();
            try {
                int index = dataStorage.getNextBufferPlace(this);
                while (dataStorage.isOccupied(index)) {
                    index = dataStorage.getNextBufferPlace(this);
                }
                dataStorage.addData(data, index);
                System.out.println("Stored data " + produceNumber + " in deposit " + index);
            }catch(Exception e){
                System.out.println("Data-Storage Failure from Producer");
                e.printStackTrace();
            }finally {producerLock.unlock();}
            try{
                long waitTime = (long) (Math.random() * 1000) + 1000L; // You can change the number sto increase or decrease the waiting time
                //wait(waitTime);
                Thread.sleep(waitTime);
            }catch(InterruptedException e){
                System.out.println("Interruption Exception from Producer");
            }
        }
    }
}
