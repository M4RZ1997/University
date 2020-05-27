package Executors;

import Broadcast.SubscribtionNotPossibleException;
import Runnables.AbstractImageManipulatorRunnable;
import Runnables.Filter.EdgeDetectionImageManipulatorRunnable;

public class EdgeDetectionExecutor extends AbstractExecutor {
    public EdgeDetectionExecutor() throws SubscribtionNotPossibleException {
        super();
    }

    public static void main(String[] args) {
        for (int i = 0; i < IMAGES.length; i++) {
            try {
                AbstractExecutor executor = new EdgeDetectionExecutor();
                executor.execute(i);
            } catch (SubscribtionNotPossibleException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected String getGeneratedFolderPath() {
        return "src/Executors/Images/edgeDetectionGenerated/";
    }

    @Override
    protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException {
        return new EdgeDetectionImageManipulatorRunnable(imageParts.get(i),this, i);
    }
}
