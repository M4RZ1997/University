package ImageAbstraction;

import java.awt.*;

public class Pixel {
    //position.x = row in image
    //position.y = column in image
    private Point position;
    private int value;
    private boolean live;

    public Pixel(Point position, int value, boolean live) {
        this.position = position;
        this.value = value;
        this.live = live;
    }

    public Pixel(Point position, int value) {
        this(position, value, true);
    }

    public Point getPosition() {
        return position;
    }

    public int getValue() {
        if (live)
            return value;
        return 0;
    }

    public String toString() {
        return Integer.toString(value) + live;
    }

    // TODO should value = 0 mean not live?
    public boolean isActive() {
        return live && value > 0;
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Pixel) {
            Pixel oPixel = (Pixel)other;
            return this.isActive() == oPixel.isActive() && this.getPosition().equals(oPixel.getPosition()) && this.getValue() == oPixel.getValue();
        }
        return false;
    }
}
