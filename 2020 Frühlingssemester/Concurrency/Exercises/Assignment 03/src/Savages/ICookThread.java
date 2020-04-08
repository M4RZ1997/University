package Savages;

public interface ICookThread extends Runnable {
    void terminate();
    void cookAndRefill() throws PotNotEmptyException;
    void order();
    boolean isPreparing();
}
