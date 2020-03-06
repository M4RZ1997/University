package producerCustomer;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ProducerCustomerManager {
    private int threadNR = 2; // varibale t
    private int storageSize = 10; // variable n

    private List<Thread> producerThreads, customerThreads;
    private Lock prodLock, custLock;
    private DataDepositBuffer dataDepositBuffer;

    public ProducerCustomerManager(){
        producerThreads = new ArrayList<>();
        customerThreads = new ArrayList<>();
        prodLock = new ReentrantLock();
        custLock = new ReentrantLock();
        dataDepositBuffer = new DataDepositBuffer(storageSize);
        for (int i = 0; i < threadNR; i++){
            producerThreads.add(new Thread(new ProducerThread(dataDepositBuffer, prodLock)));
        }
        for (int i = 0; i < threadNR; i++){
            customerThreads.add(new Thread(new CustomerThread(dataDepositBuffer, custLock)));
        }
    }

    public void runThreads(){
        for (Thread thread : producerThreads){
            thread.start();
        }
        for (Thread thread : customerThreads){
            thread.start();
        }
    }

    //It's a never ending process
    public static void main(String[] args){
        ProducerCustomerManager pcm = new ProducerCustomerManager();
        pcm.runThreads();
    }
}
