package Broadcast;

import Executors.AbstractExecutor;

public class TerminationBroadcast extends AbstractBroadcast{
    public TerminationBroadcast() {
        super();
    }

    @Override
    protected int getSenderId(Object sender) {
        return ((AbstractExecutor)sender).identifier;
    }

    @Override
    protected boolean getBroadcastCondition(Object sender, Object data) {
        return sender instanceof AbstractExecutor;
    }
}
