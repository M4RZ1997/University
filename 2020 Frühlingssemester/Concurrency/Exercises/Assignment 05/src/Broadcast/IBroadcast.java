package Broadcast;

public interface IBroadcast {
    void broadcast(Object sender, Object data);
    void subscribe (IReceiver receiver) throws SubscribtionNotPossibleException;
    int getReceiverCount();
}
