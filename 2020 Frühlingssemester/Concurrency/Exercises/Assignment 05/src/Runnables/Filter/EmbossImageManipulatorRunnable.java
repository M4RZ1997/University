package Runnables.Filter;

import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;

public class EmbossImageManipulatorRunnable extends AbstractFilterManipulatorRunnable {
    protected final int[][] KERNEL = {
            {-2, -1, 0},
            {-1, 1, 1},
            {0, 1, 2}
    };

    protected final double SCALE_FACTOR = 1;

    public EmbossImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
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
