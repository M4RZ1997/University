package Executors;

import Broadcast.SubscribtionNotPossibleException;
import Runnables.AbstractImageManipulatorRunnable;
import Runnables.Filter.BlurFilterImageManipulatorRunnable;

public class BlurExecutor extends AbstractExecutor {
    public BlurExecutor() throws SubscribtionNotPossibleException {
        super();
    }

    public static void main(String[] args) {
        for (int i = 0; i < IMAGES.length; i++) {
            try {
                AbstractExecutor executor = new BlurExecutor();
                executor.execute(i);
            } catch (SubscribtionNotPossibleException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected String getGeneratedFolderPath() {
        return "src/Executors/Images/blurGenerated/";
    }

    @Override
    protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException {
        return new BlurFilterImageManipulatorRunnable(imageParts.get(i),this, i);
    }
}
