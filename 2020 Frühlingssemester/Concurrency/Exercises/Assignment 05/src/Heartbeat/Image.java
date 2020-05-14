package Heartbeat;

import java.awt.*;
import java.util.ArrayList;

public class Image {
    private int width;
    private int height;
    private int[][] values;
    private Pixel[][] pixels;

    public Image(int[][] v){
        this.values = v;
        this.width = values.length;
        this.height = values[0].length;
        this.pixels = new Pixel[width][height];
        for (int w = 0; w < this.width; w++){
            for (int h = 0; h < this.height; h++){
                this.pixels[w][h] = new Pixel(new Point(w, h), values[w][h]);
            }
        }
    }

    public Image(Pixel[][] p){
        this.pixels = p;
        this.width = pixels.length;
        this.height = pixels[0].length;
        this.values = new int[width][height];
        for (int w = 0; w < this.width; w++){
            for (int h = 0; h < this.height; h++){
                this.values[w][h] = pixels[w][h].getValue();
            }
        }
    }

    public ArrayList<Pixel> getNeighbours(int x, int y){
        ArrayList<Pixel> neighbours = new ArrayList<Pixel>();
        if (x != 0){
            neighbours.add(pixels[x-1][y]);
        }
        if (x != width-1){
            neighbours.add(pixels[x+1][y]);
        }
        if (y != 0){
            neighbours.add(pixels[x][y-1]);
        }
        if (y != height-1){
            neighbours.add(pixels[x][y+1]);
        }
        if (x != 0 && y != 0){
            neighbours.add(pixels[x-1][y-1]);
        }
        if (x != 0 && y != height-1){
            neighbours.add(pixels[x-1][y+1]);
        }
        if (x != width-1 && y != 0){
            neighbours.add(pixels[x+1][y-1]);
        }
        if (x != width-1 && y != height-1){
            neighbours.add(pixels[x+1][y+1]);
        }
        return neighbours;
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

    public Pixel[][] getPixelArray(){
        return pixels;
    }
}
