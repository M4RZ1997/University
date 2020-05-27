package Runnables.Filter;

import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;

public class SharpenFilterImageManipulatorRunnable extends AbstractFilterManipulatorRunnable {
    protected final int[][] KERNEL = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
    };

    protected final double SCALE_FACTOR = -0.2;

    public SharpenFilterImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
        super(image, executor, identifier);
    }

    @Override
    double getScaleFactor() {
        return SCALE_FACTOR;
    }

    @Override
    int[][] getKernel() {
        return KERNEL;
    }
}
