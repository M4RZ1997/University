package Executors;

import Broadcast.SubscribtionNotPossibleException;
import Runnables.AbstractImageManipulatorRunnable;
import Runnables.Filter.SharpenFilterImageManipulatorRunnable;

public class SharpenExecutor extends AbstractExecutor {
    public SharpenExecutor() throws SubscribtionNotPossibleException {
        super();
    }

    public static void main(String[] args) {
        for (int i = 0; i < IMAGES.length; i++) {
            try {
                AbstractExecutor executor = new SharpenExecutor();
                executor.execute(i);
            } catch (SubscribtionNotPossibleException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected String getGeneratedFolderPath() {
        return "src/Executors/Images/sharpenGenerated/";
    }

    @Override
    protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException {
        return new SharpenFilterImageManipulatorRunnable(imageParts.get(i),this, i);
    }
}
