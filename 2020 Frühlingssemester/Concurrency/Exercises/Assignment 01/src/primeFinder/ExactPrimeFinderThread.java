package primeFinder;

import primeFinder.AbstractPrimeFinderThread;

import java.util.ArrayList;
import java.util.List;

public class ExactPrimeFinderThread extends AbstractPrimeFinderThread {
    private PrimeFinder primeFinder;
    private TicketManager ticketManager;
    private List<Integer> primes;

    public ExactPrimeFinderThread(TicketManager tm, PrimeFinder pf){
        primes = new ArrayList<>();
        this.ticketManager = tm;
        this.primeFinder = pf;
    }

    @Override
    public void run(){
        int i = 0;
        long startingTime = System.nanoTime();
        while (i != -1) {
            i = ticketManager.getTicketandIncrement();
            if (isPrime(i)) {
                primes.add(i);
            }
        }
        long pastTime = (System.nanoTime() - startingTime)/1000000;
        primeFinder.addTime(pastTime);
        primeFinder.addExactPrimes(primes);
    }
}
