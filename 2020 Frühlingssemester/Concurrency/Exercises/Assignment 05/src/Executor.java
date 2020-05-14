import ImageIO.*;

public class Executor {
    private static final int D = 3;
    private int threadCount = 8;
    private static int[][] image;

    public Executor(){}

    private void readFile(String pathname){
        ImageParser iParser = new ImageParser();
        image = iParser.parseImage(pathname);
//        for (int w = 0; w < iParser.getWidth(); w++){
//            for (int h = 0; h < iParser.getHeight(); h++){
//                System.out.print(image[w][h] + " ");
//            }
//            System.out.print("\n");
//        }
//        System.out.println("");
//        for (int i = 0; i < iParser.getWidth(); i++){
//            System.out.print(image[0][i] + " ");
//        }
//        System.out.print("\n");
//        System.out.println("");
//        for (int i = 0; i < iParser.getWidth(); i++){
//            System.out.print(image[image[0].length - 1][i] + " ");
//        }
//        System.out.print("\n");
    }

    private void writeFile(String filename){
        ImageCreator iCreator = new ImageCreator();
        iCreator.createFile(image, filename);
    }

    public static void main(String[] args){
        Executor executor = new Executor();
        executor.readFile("Images/testImage_10x10.pgm");
        //TODO: Do something
        String filename = "src/Images/testImage_10x10_D" + D + ".pgm";
        executor.writeFile(filename);
    }
}
