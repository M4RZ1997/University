package ImageIO;

import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class ImageCreator {
    private static final int MAXVAL = 255;

    public ImageCreator(){

    }

    public void createFile(int[][] image, String filename)
    {
        try {
            PrintWriter pw = new PrintWriter(filename);

            int width = image[0].length;
            int height = image.length;

            // magic number, width, height, and maxval
            pw.println("P2");
            pw.println(width + " " + height);
            pw.println(MAXVAL);

            // print out the data, limiting the line lengths to 70 characters
            int lineLength = 0;
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    int value = image[i][j];

                    // if we are going over 70 characters on a line,
                    // start a new line
                    String stringValue = "" + value;
                    int currentLength = stringValue.length() + 1;
                    if (currentLength + lineLength > 70)
                    {
                        pw.println();
                        lineLength = 0;
                    }
                    lineLength += currentLength;
                    pw.print(value + " ");
                }
            }
            pw.close();
        }catch(java.io.FileNotFoundException e){
            e.printStackTrace(System.err);
        }
    }
}
