package primeFinder;

public class TicketManager {
    private static final int CORE_NUMBER = 4;
    private int ticket;
    double maximum;
    private int power = 10;

    public TicketManager() {
        ticket = 0;
        maximum = Math.pow(CORE_NUMBER, power);
    }

    public synchronized int getTicketandIncrement(){
        int temp = ticket;
        if (ticket <= maximum){
            ticket++;
        } else {
            temp = -1;
        }
        return temp;
    }
}
