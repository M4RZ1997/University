package Heartbeat;

import java.awt.*;

public class Pixel {
    private Point position;
    private int value;

    public Pixel(Point point, int v){
        this.position = point;
        this.value = v;
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
