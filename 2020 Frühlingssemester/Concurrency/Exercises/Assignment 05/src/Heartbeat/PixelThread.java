package Heartbeat;


import java.util.ArrayList;

public class PixelThread implements Runnable {

    private boolean terminated = false;
    private boolean nextTurn = false;
    private int[] dataAbove;
    private boolean receivedAbove;
    private int[] dataBelow;
    private boolean receivedBelow;

    private PixelThread threadAbove;
    private PixelThread threadBelow;

    private int identificationNumber;

    private Image initialImage;
    private Image processingImage;
    private Image newImage;

    private boolean hasChanged = false;

    private Executor executor;

    public PixelThread(Executor exec, int id, Image imagePart){
        this.executor = exec;
        receivedAbove = false;
        receivedBelow = false;
        this.threadAbove = null;
        this.threadBelow = null;
        this.identificationNumber = id;
        this.initialImage = imagePart;
    }

    @Override
    public void run() {
        processingImage = initialImage;
        while (this.isRunning()){
            System.out.println("Start");
            this.sendData();
            System.out.println("Sent Data");
            this.awaitData();
            System.out.println("Received Data");
            this.manipulateData();
            System.out.println("Manipulated Data");
            this.evaluateStatus();
            executor.receiveThreadInformation(hasChanged);
            System.out.println("Evaluated Status");
            this.awaitNext();
            System.out.println("Go");
        }
        executor.setImageArray(identificationNumber, newImage);
    }

    private void sendData(){
        int[][] processImageArray = processingImage.getValues();
        if (threadAbove != null){
            threadAbove.mailboxAbove(processImageArray[0]);
        }
        if (threadBelow != null){
            threadBelow.mailboxBelow(processImageArray[processImageArray[0].length]);
        }
    }

    private void awaitData(){
        while (((this.threadAbove != null) && !receivedAbove) || ((this.threadBelow!= null) && !receivedBelow)){}
        receivedAbove = (this.threadAbove == null);
        receivedBelow = (this.threadBelow == null);
    }

    private void manipulateData(){
        Pixel[][] pixels = processingImage.getPixelArray();
        Pixel[][] duplicatedPixels = duplicatePixels(pixels);
        for (int w = 0; w < initialImage.getWidth(); w++){
            for (int h = 0; h < initialImage.getHeight(); h++){

                ArrayList<Pixel> neighbours = processingImage.getNeighbours(w, h);

                // upper edge points
                if (h == 0){
                    if (!hasNeighbourAbove()) {
                        if (countAliveNeighbours(duplicatedPixels[w][h], neighbours, null) < executor.D)
                            duplicatedPixels[w][h].killPixel();
                    }
                    else {
                        if (countAliveNeighbours(duplicatedPixels[w][h], neighbours, dataAbove) < executor.D)
                            duplicatedPixels[w][h].killPixel();
                    }
                }

                //lower edge points
                if (h == initialImage.getHeight()-1){
                    if (!hasNeighbourBelow()) {
                        if (countAliveNeighbours(duplicatedPixels[w][h], neighbours, null) < executor.D)
                            duplicatedPixels[w][h].killPixel();
                    }
                    else {
                        if (countAliveNeighbours(duplicatedPixels[w][h], neighbours, dataBelow) < executor.D)
                            duplicatedPixels[w][h].killPixel();
                    }
                }

                else {
                    if (countAliveNeighbours(duplicatedPixels[w][h], neighbours, null) < executor.D)
                        duplicatedPixels[w][h].killPixel();
                }
            }
        }
        newImage = new Image(duplicatedPixels);
    }

    private int countAliveNeighbours(Pixel pixel, ArrayList<Pixel> neighbours, int[] edgeData){
        int neighbourCount = 0;
        if (edgeData != null){
            for (int i = -1; i <= 1; i++){
                try {
                    if (edgeData[pixel.getPosition().x + i] != 0)
                        neighbourCount++;
                } catch (Exception e){}
            }
        }
        for (Pixel p : neighbours) {
            if (p.isAlive())
                neighbourCount++;
        }
        return neighbourCount;
    }

    private Pixel[][] duplicatePixels(Pixel[][] dArray){
        Pixel[][] duplicatedArray = new Pixel[dArray.length][dArray[0].length];
        int w = 0;
        for (Pixel[] sArray: dArray) {
            int h = 0;
            for (Pixel p : sArray) {
                duplicatedArray[w][h] = new Pixel(p.getPosition(), p.getValue());
                h++;
            }
            w++;
        }
        return duplicatedArray;
    }

    private void evaluateStatus(){
        Pixel[][] oldPixels = processingImage.getPixelArray();
        Pixel[][] newPixels = newImage.getPixelArray();
        for (int w = 0; w < initialImage.getWidth(); w++){
            for (int h = 0; h < initialImage.getHeight(); h++){
                if (newPixels[w][h].isAlive() != oldPixels[w][h].isAlive()){
                    hasChanged = true;
                    processingImage = newImage;
                    return;
                }
            }
        }
        hasChanged = false;
    }

    private void awaitNext(){
        while (!nextTurn){}
        nextTurn = false;
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

    public void triggerNextTurn(boolean terminate){
        this.terminated = terminate;
        nextTurn = true;
    }

    public void setNeighbours(PixelThread nAbove, PixelThread nBelow){
        this.threadAbove = nAbove;
        this.threadBelow = nBelow;
    }

    public int getIdentificationNumber() {
        return identificationNumber;
    }

    private boolean hasNeighbourAbove(){
        return threadAbove != null;
    }

    private boolean hasNeighbourBelow(){
        return threadBelow != null;
    }
}
