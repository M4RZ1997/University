package Executors;

import Broadcast.SubscribtionNotPossibleException;
import Runnables.AbstractImageManipulatorRunnable;
import Runnables.Filter.EmbossImageManipulatorRunnable;

public class EmbossExecutor extends AbstractExecutor {
    public EmbossExecutor() throws SubscribtionNotPossibleException {
        super();
    }

    public static void main(String[] args) {
        for (int i = 0; i < IMAGES.length; i++) {
            try {
                AbstractExecutor executor = new EmbossExecutor();
                executor.execute(i);
            } catch (SubscribtionNotPossibleException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected String getGeneratedFolderPath() {
        return "src/Executors/Images/embossGenerated/";
    }

    @Override
    protected AbstractImageManipulatorRunnable getManipulator(int i) throws SubscribtionNotPossibleException {
        return new EmbossImageManipulatorRunnable(imageParts.get(i),this, i);
    }
}
