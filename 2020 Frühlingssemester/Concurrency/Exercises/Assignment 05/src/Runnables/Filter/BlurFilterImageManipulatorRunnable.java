package Runnables.Filter;

import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;

public class BlurFilterImageManipulatorRunnable extends AbstractFilterManipulatorRunnable{
    protected final int[][] KERNEL = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
    };

    protected final double SCALE_FACTOR = 0.085;

    public BlurFilterImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
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
