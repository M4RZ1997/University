package primeFinder;

public abstract class AbstractPrimeFinderThread implements Runnable{

    public boolean isPrime(int number){
        if (number < 2){ return false; }
        if (number == 2 || number == 3){ return true; }
        if(number%2 == 0 || number%3 == 0) return false;
        long sqrtN = (long)Math.sqrt(number)+1;
        for(long i = 6L; i <= sqrtN; i += 6) {
            if(number%(i-1) == 0 || number%(i+1) == 0) return false;
        }
        return true;
    }
}
