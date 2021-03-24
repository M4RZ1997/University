import java.util.Random;

public class ElGamal {
    int g = 0;
    int q = 0;
    int p = 0;

    public int power(int a, int x, int p) {
        int res = 1;
        a = a % p;
        if (a == 0)
            return 0;
        while (x > 0) {
            if ((x & 1) == 0) {
                a = (a * a) % p;
                x = x >> 1;
            }
            else {
                res = (res * a) % p;
                x = x - 1;
            }
        }
        return res;
    }

    public void randomSubGroup(int q, int p) {
        Random rand = new Random();
        while (true) {
            int h = rand.nextInt(p-3) + 2;
            g = power(h, (p-1)/q, p);
            if (g != 1)
                break;
        }
    }

    public void parameters(int qbits, int pbits) {
        while (true) {
            while (true) {

            }
        }
    }

}
