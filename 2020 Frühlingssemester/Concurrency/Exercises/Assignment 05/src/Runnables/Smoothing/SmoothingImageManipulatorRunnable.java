package Runnables.Smoothing;

import Runnables.AbstractImageManipulatorRunnable;
import Executors.AbstractExecutor;
import Broadcast.SubscribtionNotPossibleException;
import ImageAbstraction.Image;
import ImageAbstraction.Pixel;

import java.util.List;

public class SmoothingImageManipulatorRunnable extends AbstractImageManipulatorRunnable {
    public SmoothingImageManipulatorRunnable(Image image, AbstractExecutor executor, int identifier) throws SubscribtionNotPossibleException {
        super(image, executor, identifier);
    }

    @Override
    protected Pixel handlePixel(Pixel pixel, List<Pixel> neighbours) {
        int count = 0;
        for (Pixel neighbour : neighbours) {
            if (neighbour.isActive()) {
                count++;
            }
        }
        if (count < AbstractExecutor.D) {
            return new Pixel(pixel.getPosition(), pixel.getValue(), false);
        }
        return new Pixel(pixel.getPosition(), pixel.getValue(), true);
    }

    @Override
    protected void evaluateState() {
        executor.setState(this.identifier, this.hasChanged);
    }
}
