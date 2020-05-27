package Broadcast;

import Runnables.AbstractImageManipulatorRunnable;

public class ImageDataBroadcast extends AbstractBroadcast{
    public ImageDataBroadcast() {
        super();
    }

    @Override
    protected int getSenderId(Object sender) {
        return ((AbstractImageManipulatorRunnable)sender).identifier;
    }

    @Override
    protected boolean getBroadcastCondition(Object sender, Object data) {
        return sender instanceof AbstractImageManipulatorRunnable;
    }
}
