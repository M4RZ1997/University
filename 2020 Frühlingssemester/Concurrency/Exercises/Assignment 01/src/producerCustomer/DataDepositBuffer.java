package producerCustomer;

import java.util.ArrayList;
import java.util.List;

public class DataDepositBuffer {
    private int storageSize;
    private List<Data> dataStorage;
    private int bufferProdPlace;
    private int bufferCustPlace;

    public DataDepositBuffer(int size){
        this.dataStorage = new ArrayList<>();
        this.storageSize = size;
        for (int i = 0; i < storageSize; i++){
            dataStorage.add(i, null);
        }
        this.bufferProdPlace = 0;
        this.bufferCustPlace = 0;
    }

    public boolean isEmpty(){
        for (int i = 0; i < storageSize; i++){
            if (dataStorage.get(i) != null){
                return false;
            }
        }
        return true;
    }

    public boolean isFull(){
        for (int i = 0; i < storageSize; i++){
            if (dataStorage.get(i) == null){
                return false;
            }
        }
        return true;
    }

    public int size(){
        return storageSize;
    }

    public boolean isOccupied(int index){
        return !(this.dataStorage.get(index) == null);
    }

    public synchronized int getNextBufferPlace(ProducerThread pt){
        int temp  = bufferProdPlace;
        bufferProdPlace++;
        if (bufferProdPlace == storageSize){
            bufferProdPlace = 0;
        }
        return temp;
    }

    public synchronized int getNextBufferPlace(CustomerThread ct){
        int temp  = bufferCustPlace;
        bufferCustPlace++;
        if (bufferCustPlace == storageSize){
            bufferCustPlace = 0;
        }
        return temp;
    }

    public void addData(Data data, int index){
        this.dataStorage.set(index, data);
    }

    public Data getData(int index){
        Data data = this.dataStorage.get(index);
        this.dataStorage.set(index, null);
        return data;
    }
}
