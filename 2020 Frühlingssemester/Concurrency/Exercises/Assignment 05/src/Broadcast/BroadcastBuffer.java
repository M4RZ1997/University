package Broadcast;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

public class BroadcastBuffer {

    private List<Map<Integer, BroadcastData>> bufferedData;
    private volatile int receiverCount;

    public BroadcastBuffer(int receiverCount) {
        this.receiverCount = receiverCount;
        this.bufferedData = new CopyOnWriteArrayList<>();
    }

    /**
     * Indicates whether the next round of messages to be handled is complete
     */
    public boolean isReady() {
        if (!bufferedData.isEmpty()) {
            return bufferedData.get(0).size() == receiverCount;
        }

        return false;
    }

    /**
     * Add received data to first element, not having received data from given sender
     */
    public synchronized void add(int senderId, BroadcastData data) {
        for (int i = 0; i < bufferedData.size(); i++) {
            Map<Integer, BroadcastData> map = this.bufferedData.get(i);
            if (!map.containsKey(senderId)) {
                map.put(senderId, data);
                return;
            }
        }
        bufferedData.add(new ConcurrentHashMap<>());
        Map<Integer, BroadcastData> map = bufferedData.get(bufferedData.size() - 1);
        map.put(senderId, data);
    }

    public Map<Integer, BroadcastData> receiveNext() throws NotAllDataReceivedException {
        if (!isReady()) {
            throw new NotAllDataReceivedException("Thread did not receive all required information yet");
        }
        return this.bufferedData.remove(0);
    }
}
