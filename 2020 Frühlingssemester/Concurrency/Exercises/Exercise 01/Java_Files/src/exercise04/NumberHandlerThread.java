package exercise04;

public class NumberHandlerThread implements Runnable {
    private DivisorFinder divisorFinder;
    private int number;
    private int divisorCount;

    public NumberHandlerThread(DivisorFinder df) {
        this.divisorFinder = df;
    }

    @Override
    public void run(){
        while(true) {
            this.number = divisorFinder.getNumberAndIncrement();
            if (number == 0) {
                return;
            }
            divisorCount = getDivisorCount();
            divisorFinder.setMaxDivisorsAndMaxNumber(divisorCount, number);
        }
    }

    public int getDivisorCount(){
        int count = 1;
        for (int i = 1; i < number; i++){
            if (number % i == 0){
                count++;
            }
        }
        return count;
    }
}
