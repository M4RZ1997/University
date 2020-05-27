package Runnables;

import Executors.AbstractExecutor;
import Broadcast.*;
import ImageAbstraction.Image;
import ImageAbstraction.Pixel;


import java.awt.*;
import java.util.List;
import java.util.Map;

public abstract class AbstractImageManipulatorRunnable implements Runnable, IReceiver {
    public final int identifier;

    protected static final int DIRECTION_TOP = -1;
    protected static final int DIRECTION_BOTTOM = 1;

    protected BroadcastBuffer broadcastBuffer;
    protected final AbstractExecutor executor;
    protected volatile boolean running;
    protected Image image;
    protected Map<Integer, BroadcastData> next;
    protected boolean hasChanged;

    public AbstractImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
        this.executor = executor;

        this.executor.broadcast.subscribe(this);
        this.executor.terminationBroadcast.subscribe(this);

        this.running = true;
        this.image = image;
        this.identifier = identifier;
        this.hasChanged = false;
    }

    @Override
    public void run() {
        while(this.isRunning()) {
            this.sendData();
            this.awaitData();
            this.manipulateImage();
            this.evaluateState();
        }
        this.executor.resultBroadcast.broadcast(this, this.image);
    }

    @Override
    public void onReceive(int senderId, Object data) {
        if (senderId != this.executor.identifier && data instanceof BroadcastData) {
            this.broadcastBuffer.add(senderId, (BroadcastData)data);
        }
        if (senderId == this.executor.identifier && data instanceof ExitData) {
            this.running = false;
        }
    }

    public void instanciateBroadcastBuffer() {
        this.broadcastBuffer = new BroadcastBuffer(this.executor.broadcast.getReceiverCount());
    }

    public boolean isRunning() {
        return this.running;
    }

    abstract protected void evaluateState();// {
//        executor.setState(this.identifier, this.hasChanged);
//    }

    protected void manipulateImage() {
        if (isRunning()) {
            int topNeighbourId = this.getTopNeighbourId();
            int bottomNeighbourId = this.getBottomNeighbourId();
            BroadcastData top = null, bottom = null;
            if (this.next.containsKey(topNeighbourId)) {
                top = this.next.get(topNeighbourId);
            }
            if (this.next.containsKey(bottomNeighbourId)) {
                bottom = this.next.get(bottomNeighbourId);
            }
            this.manipulateImage(top, bottom);
        }
    }

    protected int getBottomNeighbourId() {
        return this.executor.getBottomNeighbourId(this.identifier);
    }

    protected int getTopNeighbourId() {
        return this.executor.getTopNeighbourId(this.identifier);
    }

    protected void manipulateImage(BroadcastData top, BroadcastData bottom) {
        Pixel[] topRow = null, bottomRow = null;
        if (top != null) {
            topRow = top.bottomRow;
        }
        if (bottom != null) {
            bottomRow = bottom.topRow;
        }
        Image nextImage = new Image(this.image.getWidth(), this.image.getHeight(), this.image.topRowIndex, this.image.bottomRowIndex);

        if (this.image.getHeight() > 1) {
            // handle top row
            for (int column = 0; column < this.image.getWidth(); column++) {
                Pixel pixel = this.handleEdgePixel(this.image.getPixel(0, column), topRow, DIRECTION_TOP);
                nextImage.setPixel(pixel);
            }

            // handle regular rows
            for (int row = 1; row < this.image.getHeight() - 1; row++) {
                for (int column = 0; column < this.image.getWidth(); column++) {
                    Pixel pixel = this.handleRegularPixel(this.image.getPixel(row, column));
                    nextImage.setPixel(pixel);
                }
            }

            // handle bottom row
            for (int column = 0; column < this.image.getWidth(); column++) {
                Pixel pixel = this.handleEdgePixel(this.image.getPixel(this.image.getHeight() - 1, column), bottomRow, DIRECTION_BOTTOM);
                nextImage.setPixel(pixel);
            }
        } else {
            // handle single row
            for (int column = 0; column < this.image.getWidth(); column++) {
                Pixel pixel = this.handleDoubleEdgePixel(this.image.getPixel(0, column), topRow, bottomRow);
                nextImage.setPixel(pixel);
            }
        }

        this.hasChanged = !this.image.equals(nextImage);
        this.image = nextImage;
    }

    protected void awaitData() {
        while (running && !this.broadcastBuffer.isReady());
        if (!running) {
            return;
        }
        try {
            this.next = this.broadcastBuffer.receiveNext();
        } catch (NotAllDataReceivedException e) {
            this.awaitData();
        }
    }

    protected void sendData() {
        BroadcastData data = new BroadcastData();
        data.topRow = this.image.getTopRow();
        data.bottomRow = this.image.getBottomRow();
        this.executor.broadcast.broadcast(this, data);
    }

    //TODO refactor
    private Pixel handleDoubleEdgePixel(Pixel pixel, Pixel[] topRow, Pixel[] bottomRow) {
        List<Pixel> neighbours = this.image.getNeighbours(pixel);
        if (topRow != null) {
            //TODO correct?
            Point newPosition = new Point(pixel.getPosition().x + DIRECTION_TOP, pixel.getPosition().y);
            neighbours.add(new Pixel(newPosition, topRow[pixel.getPosition().y].getValue(), topRow[pixel.getPosition().y].isActive()));
            if (pixel.getPosition().y > 0) {
                newPosition = new Point(pixel.getPosition().x + DIRECTION_TOP, pixel.getPosition().y - 1);
                neighbours.add(new Pixel(newPosition, topRow[pixel.getPosition().y - 1].getValue(), topRow[pixel.getPosition().y - 1].isActive()));
            }
            if (pixel.getPosition().y < this.image.getWidth() - 1) {
                newPosition = new Point(pixel.getPosition().x + DIRECTION_TOP, pixel.getPosition().y + 1);
                neighbours.add(new Pixel(newPosition, topRow[pixel.getPosition().y + 1].getValue(), topRow[pixel.getPosition().y + 1].isActive()));
            }
        }

        if (bottomRow != null) {
            //TODO correct?
            Point newPosition = new Point(pixel.getPosition().x + DIRECTION_BOTTOM, pixel.getPosition().y);
            neighbours.add(new Pixel(newPosition, bottomRow[pixel.getPosition().y].getValue(), bottomRow[pixel.getPosition().y].isActive()));
            if (pixel.getPosition().y > 0) {
                newPosition = new Point(pixel.getPosition().x + DIRECTION_BOTTOM, pixel.getPosition().y - 1);
                neighbours.add(new Pixel(newPosition, bottomRow[pixel.getPosition().y - 1].getValue(), bottomRow[pixel.getPosition().y - 1].isActive()));
            }
            if (pixel.getPosition().y < this.image.getWidth() - 1) {
                newPosition = new Point(pixel.getPosition().x + DIRECTION_BOTTOM, pixel.getPosition().y + 1);
                neighbours.add(new Pixel(newPosition, bottomRow[pixel.getPosition().y + 1].getValue(), bottomRow[pixel.getPosition().y + 1].isActive()));
            }
        }
        return handlePixel(pixel, neighbours);
    }

    protected Pixel handleEdgePixel(Pixel pixel, Pixel[] extraRow, int direction) {
        List<Pixel> neighbours = this.image.getNeighbours(pixel);
        if (extraRow != null) {
            //TODO correct?
            Point newPosition = new Point(pixel.getPosition().x + direction, pixel.getPosition().y);
            neighbours.add(new Pixel(newPosition, extraRow[pixel.getPosition().y].getValue(), extraRow[pixel.getPosition().y].isActive()));
            if (pixel.getPosition().y > 0) {
                newPosition = new Point(pixel.getPosition().x + direction, pixel.getPosition().y - 1);
                neighbours.add(new Pixel(newPosition, extraRow[pixel.getPosition().y - 1].getValue(), extraRow[pixel.getPosition().y - 1].isActive()));
            }
            if (pixel.getPosition().y < this.image.getWidth() - 1) {
                newPosition = new Point(pixel.getPosition().x + direction, pixel.getPosition().y + 1);
                neighbours.add(new Pixel(newPosition, extraRow[pixel.getPosition().y + 1].getValue(), extraRow[pixel.getPosition().y + 1].isActive()));
            }
        }
        return handlePixel(pixel, neighbours);
    }

    protected Pixel handleRegularPixel(Pixel pixel) {
        List<Pixel> neighbours = this.image.getNeighbours(pixel);
        return handlePixel(pixel, neighbours);
    }

    abstract protected Pixel handlePixel(Pixel pixel, List<Pixel> neighbours);
}
