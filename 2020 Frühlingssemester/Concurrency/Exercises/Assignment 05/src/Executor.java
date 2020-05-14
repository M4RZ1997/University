import ImageIO.*;

public class Executor {
    private static final int D = 3;
    private int threadCount = 8;
    private int[][] image;

    public Executor(){}

    private void readFile(String pathname){
        ImageParser iParser = new ImageParser();
        image = iParser.parseImage(pathname);
    }

    private void writeFile(String filename){
        ImageCreator iCreator = new ImageCreator();
        iCreator.createFile(image, filename);
    }

    public static void main(String[] args){
        Executor executor = new Executor();
        executor.readFile("Images/first.pgm");
        //TODO: Do something
        String filename = "src/Images/newFirst_D" + D + ".pgm";
        executor.writeFile(filename);
    }
}
