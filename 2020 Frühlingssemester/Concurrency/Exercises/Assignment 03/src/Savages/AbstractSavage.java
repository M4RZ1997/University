package Savages;

public abstract class AbstractSavage implements ISavageThread {
    protected IPot foodPot;
    protected ICookThread cook;
    protected IExecutor executor;

    public AbstractSavage(IPot pot, ICookThread ct, IExecutor exec){
        this.foodPot = pot;
        this.cook = ct;
        this.executor = exec;
    }

    @Override
    public void eat() {
        if (foodPot.isEmpty() && !cook.isPreparing()) {
            System.out.println("Have ordered");
            orderRefill();
        }
        while (foodPot.isEmpty()) {}
        try{
            this.foodPot.getFood();
        }catch(PotIsEmptyException e){};
    }

    @Override
    public void orderRefill() {
        cook.order();
    }
}
