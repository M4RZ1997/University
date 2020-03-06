package primeFinder;

import primeFinder.AbstractPrimeFinderThread;
import primeFinder.PrimeFinder;

import java.util.ArrayList;
import java.util.List;

public class RangePrimeFinderThread extends AbstractPrimeFinderThread {
    int min, max;
    private PrimeFinder primeFinder;
    private List<Integer> primes;
    private int power = 9;

    public RangePrimeFinderThread(int index, int coreNumber, PrimeFinder pf){
        primes = new ArrayList<>();
        this.primeFinder = pf;
        min = (int) (index * Math.pow(coreNumber, power) + 1);
        max = (int) ((index+1) * Math.pow(coreNumber, power));
    }

    @Override
    public void run() {
        long startingTime = System.nanoTime();
        for (int i = min; i <= max; i++){
            if (isPrime(i)){
                primes.add(i);
            }
        }
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        primeFinder.addTime(pastTime);
        primeFinder.addPrimes(primes);
    }
}
