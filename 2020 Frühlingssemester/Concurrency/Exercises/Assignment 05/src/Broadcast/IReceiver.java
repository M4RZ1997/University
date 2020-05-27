package Broadcast;

public interface IReceiver {
    void onReceive(int senderId, Object data);
}
