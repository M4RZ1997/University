package Runnables.Filter;

import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;

public class EdgeEnhancementImageManipulatorRunnable extends AbstractFilterManipulatorRunnable{
    protected final int[][] KERNEL = {
            {0, 0, 0},
            {-1, 1, 0},
            {0, 0, 0}
    };

    protected final double SCALE_FACTOR = -1;

    public EdgeEnhancementImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
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
