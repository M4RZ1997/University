package Heartbeat;

import java.awt.*;
import java.util.concurrent.locks.ReentrantLock;

public class PixelManager {
    private Image image;
    private Point position;
    private ReentrantLock lock;

    public PixelManager(Image i){
        this.lock = new ReentrantLock();
        this.image = i;
    }

    public Pixel getNextPixel(){
        try{
            this.lock.lock();
        }finally{
            this.lock.unlock();
        }

        return pixel;
    }
}
