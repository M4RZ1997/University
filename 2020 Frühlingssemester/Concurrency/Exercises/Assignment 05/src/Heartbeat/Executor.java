package Heartbeat;

import ImageIO.*;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Executor {
    public static final int D = 3;
    private final int THREAD_COUNT = 2;
    private int[][] imageArray;
    private Image image;
    private List<PixelThread> pixelThreads;
    private List<Thread> threads;
    private List<Image> imageParts;

    private List<Image> imagePartsProcessed;

    private volatile List<Boolean> changeInformation;

    public Executor(){
        this.changeInformation = new ArrayList<>();
        this.pixelThreads = new ArrayList<>();
        this.threads = new ArrayList<>();
        this.imageParts = new ArrayList<>();
        imagePartsProcessed = new ArrayList<>();
        for (int i = 0; i < THREAD_COUNT; i++)
            imagePartsProcessed.add(null);
    }

    private void initialiseThreads(){
        for (int i = 0; i < THREAD_COUNT; i++){
            this.pixelThreads.add(new PixelThread(this, i, imageParts.get(i)));
        }
        for (PixelThread t : this.pixelThreads) {
            PixelThread threadAbove = null;
            PixelThread threadBelow = null;
            try{
                threadAbove = this.pixelThreads.get(t.getIdentificationNumber() - 1);
            } catch (Exception e){}
            try{
                threadBelow = this.pixelThreads.get(t.getIdentificationNumber() + 1);
            } catch (Exception e){}
            t.setNeighbours(threadAbove, threadBelow);
            this.threads.add(new Thread(t));
        }
    }

    private void readAndParseFile(String pathname){
        ImageParser iParser = new ImageParser();
        imageArray = iParser.parseImage(pathname);
        image = new Image(imageArray);
//        for (int w = 0; w < iParser.getWidth(); w++){
//            for (int h = 0; h < iParser.getHeight(); h++){
//                System.out.print(image[w][h] + " ");
//            }
//            System.out.print("\n");
//        }
//        System.out.println("");
//        for (int i = 0; i < iParser.getWidth(); i++){
//            System.out.print(image[0][i] + " ");
//        }
//        System.out.print("\n");
//        System.out.println("");
//        for (int i = 0; i < iParser.getWidth(); i++){
//            System.out.print(image[image[0].length - 1][i] + " ");
//        }
//        System.out.print("\n");
    }

    private void splitImage(){
        int[][] partImageArray;
        int rowCount = image.getHeight();
        int stepSize = rowCount /  THREAD_COUNT;
        for (int i = 0; i < THREAD_COUNT; i++){
            if (i != THREAD_COUNT-1){
                partImageArray = new int[stepSize][image.getWidth()];
                for (int j = 0; j < image.getWidth(); j++) {
                    partImageArray[0] = imageArray[3 * i];
                    partImageArray[1] = imageArray[3 * i + 1];
                    partImageArray[2] = imageArray[3 * i + 2];
                }
                imageParts.add(new Image(partImageArray));
            }
            else{
                int overflow = rowCount % THREAD_COUNT;
                int lastSize = overflow + stepSize;
                partImageArray = new int[lastSize][image.getWidth()];
                for (int h = 0; h < lastSize; h++){
                    for (int j = 0; j < image.getWidth(); j++) {
                        partImageArray[h] = imageArray[3*i + h];
                    }
                }
                imageParts.add(new Image(partImageArray));
                int v = 0;
                String filename = "src/Images/testImage_10x10_v" + v++ + ".pgm";
                writeFile(filename, partImageArray);
            }
        }
    }

    private void writeFile(String filename, int[][] image){
        ImageCreator iCreator = new ImageCreator();
        iCreator.createFile(image, filename);
    }

    private void runThreads() {
        for (Thread thread : this.threads) {
            thread.start();
        }
        try {
            for (Thread thread : threads) {
                thread.join();
            }
        } catch (Exception e) {
            System.out.println("Threads were interrupted");
        }
    }

    public synchronized void receiveThreadInformation(boolean hasChanged){
        System.out.println("Executor received information!");
        this.changeInformation.add(hasChanged);
        System.out.println(this.changeInformation.size());
        if (this.changeInformation.size() == THREAD_COUNT){
            System.out.println("Executor going on!");
            for (Boolean b : this.changeInformation) {
                if (b){
                    for (PixelThread pt: pixelThreads) {
                        pt.triggerNextTurn(false);
                    }
                    this.changeInformation = new ArrayList<>();
                    return;
                }
            }
            for (PixelThread pt: pixelThreads) {
                pt.triggerNextTurn(true);
            }
            this.changeInformation = new ArrayList<>();
        }
    }

    public static void main(String[] args){
        Executor executor = new Executor();
        executor.readAndParseFile("Images/testImage_10x10.pgm");
        executor.splitImage();
        executor.initialiseThreads();
        executor.runThreads();
        executor.reconstructImage();
        String filename = "src/Images/testImage_10x10_D" + D + ".pgm";
        executor.writeFile(filename, executor.reconstructImage());
    }

    public int[][] reconstructImage(){
        for (int h = 0; h < image.getHeight(); h++){
            for (Image image : imagePartsProcessed) {
                int[][] imagePartsProcessArray = image.getValues();
                for (int partHeight = 0; partHeight < imagePartsProcessArray[0].length; partHeight++){
                    for (int w = 0; w < image.getWidth(); w++){
                        imageArray[h][w] = imagePartsProcessArray[partHeight][w];
                    }
                }
            }
        }
        return imageArray;
    }

    public void setImageArray (int i, Image newImage){
        imagePartsProcessed.set(i, newImage);
    }
}
