package ImageIO;

import java.io.*;
import java.util.*;

public class ImageParser {
    private static int width;
    private static int height;

    public ImageParser(){
    }

    public int[][] parseImage(String path){
        InputStream f = ClassLoader.getSystemClassLoader().getResourceAsStream(path);
        BufferedReader br = new BufferedReader(new InputStreamReader(f));
        try {
            String magic = br.readLine(); // first line contains P2 or P5
            String line = br.readLine(); // second line contains width and height
            while (line.startsWith("#")){
                line = br.readLine();
            }
            Scanner s = new Scanner(line);
            setWidth(s.nextInt());
            setHeight(s.nextInt());
            line = br.readLine(); // third line contains maxVal

            int [][] image = new int[height][width];

            int count = 0;

            try {
                while(count < height * width){
                    line = br.readLine();
                    s = new Scanner(line);
                    while (s.hasNext()){
                        image[count / width][count % width] = s.nextInt();
                        count++;
                    }
                }
            }catch (Exception e){
                e.printStackTrace(System.err);
            }
            return image;
        } catch (java.io.IOException t){
            t.printStackTrace(System.err);
            return null;
        }
    }

    public void setWidth(int w) {
        this.width = w;
    }

    public void setHeight(int h) {
        this.height = h;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }
}
