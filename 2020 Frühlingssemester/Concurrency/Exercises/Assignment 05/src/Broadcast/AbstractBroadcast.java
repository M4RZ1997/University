package Broadcast;

import java.util.ArrayList;

public abstract class AbstractBroadcast implements IBroadcast{
    protected final ArrayList<IReceiver> receivers;
    protected boolean working;

    public AbstractBroadcast() {
        this.receivers = new ArrayList<>();
        this.working = false;
    }

    @Override
    public void broadcast(Object sender, Object data) {
        if (getBroadcastCondition(sender, data)) {
            if (!this.working) {
                this.working = true;
            }
            for (IReceiver receiver : this.receivers) {
                receiver.onReceive(getSenderId(sender), data);
            }
        }
    }

    @Override
    public synchronized void subscribe(IReceiver receiver) throws SubscribtionNotPossibleException {
        if (this.working) {
            throw new SubscribtionNotPossibleException("Can't subscribe to this broadcast, since broadcast might already have sent messages");
        }
        this.receivers.add(receiver);
    }

    @Override
    public int getReceiverCount() {
        return this.receivers.size();
    }

    protected abstract int getSenderId(Object sender);

    protected abstract boolean getBroadcastCondition(Object sender, Object data);

}
