package Heartbeat;

import java.awt.*;

public class Pixel {
    private Point position;
    private int value;
    private boolean alive;

    public Pixel(Point point, int v){
        this.position = point;
        this.value = v;
        this.alive = true;
    }

    public void killPixel(){
        this.alive = false;
        this.value = 0;
    }

    public boolean isAlive(){
        return alive;
    }

    public void setValue(int v){
        this.value = v;
    }

    public void setPosition(Point point){
        this.position = point;
    }

    public int getValue() {
        return value;
    }

    public Point getPosition() {
        return position;
    }
}
