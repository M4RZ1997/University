package Runnables.Filter;

import Runnables.AbstractImageManipulatorRunnable;
import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Pixel;
import ImageAbstraction.Image;

import java.awt.*;
import java.util.List;

public abstract class AbstractFilterManipulatorRunnable extends AbstractImageManipulatorRunnable {
    public AbstractFilterManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
        super(image, executor, identifier);
    }

    @Override
    protected Pixel handlePixel(Pixel pixel, List<Pixel> neighbours) {
        int accumulator = 0;
        for (Pixel neigbour : neighbours) {
            Point pixelPosition = pixel.getPosition();
            Point neigbourPosition = neigbour.getPosition();
            Point relativeNeighbourPosition = new Point(neigbourPosition.x - pixelPosition.x, neigbourPosition.y - pixelPosition.y);
            accumulator += neigbour.getValue() * getKernel()[1 - relativeNeighbourPosition.y][1 - relativeNeighbourPosition.x];
        }
        accumulator *= getScaleFactor();
        if (accumulator > 255)
            accumulator = 255;
        if (accumulator < 0)
            accumulator = 0;
        return new Pixel(pixel.getPosition(), accumulator, pixel.isActive());
    }

    @Override
    protected void evaluateState() {
        //this algorithm should only iterate once
        executor.setState(this.identifier, false);
    }

    abstract double getScaleFactor();

    abstract int[][] getKernel();
}
