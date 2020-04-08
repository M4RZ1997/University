package Savages;

public interface IPot{
    boolean isEmpty();
    boolean isNotEmpty();
    int getCapacity();
    int getFillAmount();
    void getFood() throws PotIsEmptyException;
    void refill();
}