package Heartbeat;

public class PixelThread implements Runnable {

    private boolean terminated = false;
    private int[] dataAbove;
    private boolean receivedAbove;
    private int[] dataBelow;
    private boolean receivedBelow;

    private PixelThread threadAbove;
    private PixelThread threadBelow;

    private int identificationNumber;

    private int[][] processingImage;

    public PixelThread(PixelThread nAbove, PixelThread nBelow, int id, int[][] imagePart){
        receivedAbove = false;
        receivedBelow = false;
        this.threadAbove = nAbove;
        this.threadBelow = nBelow;
        this.identificationNumber = id;
        this.processingImage = imagePart;
    }

    @Override
    public void run() {
        while (this.isRunning()){
            this.sendData();
            this.awaitData();
            this.manipulateData();
            this.evaluateState();
        }
    }

    private void sendData(){
        if (threadAbove != null){
            threadAbove.mailboxAbove(processingImage[0]);
        }
        if (threadBelow != null){
            threadBelow.mailboxAbove(processingImage[processingImage[0].length]);
        }
    }

    private boolean isRunning(){
        return !terminated;
    }

    public void mailboxBelow(int[] lowerRow){
        this.dataAbove = lowerRow;
        receivedAbove = true;
    }

    public void mailboxAbove(int[] upperRow){
        this.dataBelow = upperRow;
        receivedBelow = true;
    }
}
