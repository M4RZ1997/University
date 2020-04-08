package Savages;

public class FoodPot implements IPot{
    private final int CAPACITY;
    private volatile int currentFillAmount = 0;

    public FoodPot(int cap){
        this.CAPACITY = cap;
    }

    public FoodPot(int cap, int fillAmount){
        this.CAPACITY = cap;
        this.currentFillAmount = fillAmount;
    }

    @Override
    public boolean isEmpty() {
        return (this.currentFillAmount == 0);
    }

    @Override
    public boolean isNotEmpty() {
        return !isEmpty();
    }

    @Override
    public int getCapacity() {
        return this.CAPACITY;
    }

    @Override
    public int getFillAmount() {
        return this.currentFillAmount;
    }

    @Override
    public void getFood() throws PotIsEmptyException{
        if (isEmpty())
            throw new PotIsEmptyException();
        currentFillAmount--;
    }

    @Override
    public void refill() {
        this.currentFillAmount = this.CAPACITY;
    }
}
