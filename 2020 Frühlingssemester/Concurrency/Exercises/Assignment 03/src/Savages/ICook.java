package Savages;

public interface ICook extends Runnable {
    void terminate();
    void cookAndRefill() throws PotNotEmptyException;
    void order();
    boolean isPreparing();
}
