package ImageAbstraction;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class Image {
    public final int topRowIndex;
    public final int bottomRowIndex;

    private int width;
    private int height;

    //each Pixel[] inside Pixel[][] is a row, thus Pixel[x][y] is the pixel in the x'th row and the y'th column
    private Pixel[][] image;

    public Image(int width, int height, int topRowIndex, int bottomRowIndex) {
        this.width = width;
        this.height = height;
        this.topRowIndex = topRowIndex;
        this.bottomRowIndex = bottomRowIndex;
        initImage(width, height);
    }

    public Image(int[][] image, int topRowIndex, int bottomRowIndex) {
        initImage(image);
        this.height = this.image.length;
        this.width = this.image[0].length;
        this.topRowIndex = topRowIndex;
        this.bottomRowIndex = bottomRowIndex;
    }

    public Image(Pixel[][] image, int topRowIndex, int bottomRowIndex) {
        this.image = image;
        this.height = this.image.length;
        this.width = this.image[0].length;
        this.topRowIndex = topRowIndex;
        this.bottomRowIndex = bottomRowIndex;
    }

    /**
     * returns the row from including topRow to exclusive bottomRow
     */
    public Image getRows(int topRow, int bottomRow) {
        Image image = new Image(this.getWidth(), bottomRow - topRow, topRowIndex + topRow, topRowIndex + bottomRow);
        for (int i = 0; i < bottomRow - topRow; i++) {
            image.setRow(i, this.image[topRow + i]);
        }
        return image;
    }

    public void setRow(int row, Pixel[] pixels) {
        for (int column = 0; column < this.width; column++) {
            Pixel pixel = pixels[column];
            this.image[row][column] = new Pixel(new Point(row, column), pixel.getValue(), pixel.isActive());
        }
    }

    public Pixel[] getRow(int row) {
        return this.image[row];
    }

    public Pixel[] getTopRow() {
        return this.image[0];
    }

    public Pixel[] getBottomRow() {
        return this.image[this.image.length - 1];
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public Dimension getDimension() {
        return new Dimension(this.width, this.height);
    }

    public void setValue(int row, int column, int value) {
        this.image[row][column] = new Pixel(new Point(row, column), value, this.image[row][column].isActive());
    }

    public int getValue(int row, int column) {
        return this.getPixel(row, column).getValue();
    }

    public Pixel getPixel(int row, int column) {
        return this.image[row][column];
    }

    public List<Pixel> getNeighbours(Pixel pixel) {
        List<Pixel> neighbours = new ArrayList<>();
        Point position = pixel.getPosition();

        if (position.x > 0) {
            neighbours.add(image[position.x - 1][position.y]);
            if (position.y > 0) {
                neighbours.add(image[position.x - 1][position.y - 1]);
            }
        }

        if (position.y > 0) {
            neighbours.add(image[position.x][position.y - 1]);
            if (position.x < this.getHeight() - 1) {
                neighbours.add(image[position.x + 1][position.y - 1]);
            }
        }


        if (position.x < this.getHeight() - 1) {
            neighbours.add(image[position.x + 1][position.y]);
            if (position.y < this.getWidth() - 1) {
                neighbours.add(image[position.x + 1][position.y + 1]);
            }
        }

        if (position.y < this.getWidth() - 1) {
            neighbours.add(image[position.x][position.y + 1]);
            if (position.x > 0) {
                neighbours.add(image[position.x - 1][position.y + 1]);
            }
        }

        return neighbours;
    }

    public List<Image> splitIntoParts(int parts) {
        if (parts > this.height)
            parts = height;
        List<Image> result = new ArrayList<>();
        int rowsPerImage = this.height / parts;
        int index = 0;
        // Split into equal parts
        for (int i = 0; i < parts - 1; i++) {
            result.add(this.getRows(index, index + rowsPerImage));
            index += rowsPerImage;
        }
        // Last element can contain more rows, since previous rows might have gotten slightly less than avarage count of rows
        result.add(this.getRows(index, this.height));

        return result;
    }

    public void setRows(int start, Image image) {
        for (int i = 0; i < image.getHeight(); i++) {
            this.setRow(start + i, image.getRow(i));
        }
    }

    public void setPixel(Pixel pixel) {
        Point position = pixel.getPosition();
        this.image[position.x][position.y] = pixel;
    }

    @Override
    public boolean equals(Object other) {
        if (other instanceof Image) {
            Image oImage = (Image) other;
            for (int row = 0; row < getHeight(); row++) {
                for (int column = 0; column < getWidth(); column++) {
                    if (!image[row][column].equals(oImage.image[row][column])) {
                        return false;
                    }
                }
            }
            return true;
        }
        return false;
    }

    public int[][] asIntArray() {
        int[][] result = new int[this.height][this.width];
        for (int row = 0; row < this.height; row++) {
            for (int column = 0; column < this.width; column++) {
                result[row][column] = this.image[row][column].getValue();
            }
        }
        return result;
    }

    private void initImage(int width, int height) {
        this.image = new Pixel[height][width];
        for (int row = 0; row < this.image.length; row++) {
            for (int column = 0; column < this.image[row].length; column++) {
                this.image[row][column] = new Pixel(new Point(row, column),  0);
            }
        }
    }

    private void initImage(int[][] image) {
        this.image = new Pixel[image.length][image[0].length];
        for (int row = 0; row < this.image.length; row++) {
            for (int column = 0; column < this.image[row].length; column++) {
                this.image[row][column] = new Pixel(new Point(row, column),  image[row][column]);
            }
        }
    }
}
