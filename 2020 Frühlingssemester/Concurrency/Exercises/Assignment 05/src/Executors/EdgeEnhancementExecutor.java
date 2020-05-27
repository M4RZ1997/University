package Executors;

import Broadcast.SubscribtionNotPossibleException;
import Runnables.AbstractImageManipulatorRunnable;
import Runnables.Filter.EdgeEnhancementImageManipulatorRunnable;

public class EdgeEnhancementExecutor extends AbstractExecutor {
    public EdgeEnhancementExecutor() throws SubscribtionNotPossibleException {
        super();
    }

    public static void main(String[] args) {
        for (int i = 0; i < IMAGES.length; i++) {
            try {
                AbstractExecutor executor = new EdgeEnhancementExecutor();
                executor.execute(i);
            } catch (SubscribtionNotPossibleException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected String getGeneratedFolderPath() {
        return "src/Executors/Images/edgeEnhancementGenerated/";
    }

    @Override
    protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException {
        return new EdgeEnhancementImageManipulatorRunnable(imageParts.get(i),this, i);
    }
}
