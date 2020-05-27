package Runnables.Filter;

import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;

public class EdgeDetectionImageManipulatorRunnable extends AbstractFilterManipulatorRunnable{
    protected final int[][] KERNEL = {
            {0, 1, 0},
            {1, -4, 1},
            {0, 1, 0}
    };

    protected final double SCALE_FACTOR = 1;

    public EdgeDetectionImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
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
