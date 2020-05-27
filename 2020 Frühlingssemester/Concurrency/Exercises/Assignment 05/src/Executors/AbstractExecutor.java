package Executors;

import Broadcast.*;
import ImageIO.*;
import ImageAbstraction.Image;
import Runnables.AbstractImageManipulatorRunnable;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public abstract class AbstractExecutor implements IReceiver{
    public static final int D = 7;
    protected static final int THREAD_COUNT = 4;
    protected static final String FOLDER_PATH = "Executors/Images/";
    protected static final String[] IMAGES = {
            "apollonian_gasket.ascii",
            "baboon.ascii",
            "balloons.ascii",
            "barbara.ascii",
            "body1.ascii",
            "brain_398.ascii",
            "casablanca.ascii",
            "coins.ascii",
            "columns.ascii",
            "dragon.ascii",
            "f14.ascii",
            "fractal_tree.ascii",
            "testImage_10x10"
    };


    public final int identifier;

    public final IBroadcast broadcast;
    public final IBroadcast resultBroadcast;
    public final IBroadcast terminationBroadcast;

    protected final ImageParser imageParser;
    protected final ImageCreator imageCreator;

    protected Image image;
    protected List<Image> imageParts;
    protected Image finalImage;

    protected List<Thread> threads;
    protected Map<Integer, Boolean> threadStates;
    protected List<AbstractImageManipulatorRunnable> manipulators;

    protected boolean isConstructingResult;

    public AbstractExecutor() throws SubscribtionNotPossibleException {
        this.threads = new ArrayList<>();
        this.threadStates = new ConcurrentHashMap<>();
        this.manipulators = new ArrayList<>();

        this.identifier = -1;
        this.isConstructingResult = false;

        this.imageCreator = new ImageCreator();
        this.imageParser = new ImageParser();

        this.broadcast = new ImageDataBroadcast();
        this.terminationBroadcast = new TerminationBroadcast();
        this.resultBroadcast = new ImageDataBroadcast();
        this.resultBroadcast.subscribe(this);
    }

    public void execute(int i) throws SubscribtionNotPossibleException{
        long startTime = System.currentTimeMillis();
        this.readImage(IMAGES[i]);
        this.splitImage();
        this.initAndRunThreads();
        this.saveImage(IMAGES[i]);
        long computationTime = System.currentTimeMillis() - startTime;
        System.out.println("Computation Time: " + computationTime);
    }

    public int getTopNeighbourId(int id) {
        return id - 1;
    }

    public int getBottomNeighbourId(int id) {
        return id + 1;
    }

    public void setState(int identifier, boolean wantsToContinue) {
        this.threadStates.put(identifier, wantsToContinue);
    }

    @Override
    public synchronized void onReceive(int senderId, Object data) {
        Image image = (Image) data;
        if (!isConstructingResult) {
            isConstructingResult = true;
            this.finalImage = new Image(this.image.getWidth(), this.image.getHeight(), this.image.topRowIndex, this.image.bottomRowIndex);
        }
        this.finalImage.setRows(image.topRowIndex, image);
    }

    protected void readImage(String name) {
        int[][] imageArray = this.imageParser.parseImage(FOLDER_PATH + name + ".pgm");
        this.image = new Image(imageArray, 0, imageArray.length);
    }

    protected void splitImage() {
        imageParts = this.image.splitIntoParts(THREAD_COUNT);
    }

    protected void initAndRunThreads() throws SubscribtionNotPossibleException {
        for (int i = 0; i < imageParts.size(); i++) {
            manipulators.add(getManipulator(i));
        }

        for (int i = 0; i < imageParts.size(); i++) {
            threads.add(new Thread(manipulators.get(i)));
            manipulators.get(i).instanciateBroadcastBuffer();
            threadStates.put(i, true);
        }

        for (Thread thread : threads) {
            thread.start();
        }

        this.queryThreadStates();

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                //lazy
            }
        }
    }

    protected void saveImage(String name) {
        this.imageCreator.createFile(this.finalImage.asIntArray(), getGeneratedFolderPath() + name + "_new.pgm");
        System.out.println("Finished manipulating " + name);
    }

    protected void queryThreadStates() {
        boolean changed = true;
        while (changed) {
            changed = false;
            for (Map.Entry<Integer, Boolean> element : this.threadStates.entrySet()) {
                if (element.getValue()) {
                    changed = true;
                }
            }
        }
        this.terminationBroadcast.broadcast(this, new ExitData());
    }

    protected String getGeneratedFolderPath(){
        return "src/Executors/Images/nullGenerated/";
    }

    abstract protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException;
}
