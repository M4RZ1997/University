package Heartbeat;

import java.awt.*;

public class Image {
    private int width;
    private int height;
    private int[][] values;
    private Pixel[][] image;

    public Image(int[][] v){
        this.values = v;
        this.width = values.length;
        this.height = values[0].length;
        for (int i = 0; i < this.width; i++){
            for (int h = 0; h < this.height; h++){
                this.image[i][h] = new Pixel(new Point(i, h), values[i][h]);
            }
        }
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public void setWidth(int width) {
        this.width = width;
    }

    public void setValues(int[][] values) {
        this.values = values;
    }

    public int getHeight() {
        return height;
    }

    public int getWidth() {
        return width;
    }

    public int[][] getValues() {
        return values;
    }

    public Pixel getPixel(int x, int y){
        return this.image[x][y];
    }
}
