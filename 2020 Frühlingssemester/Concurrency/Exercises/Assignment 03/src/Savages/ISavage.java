package Savages;

import java.util.Observer;

public interface ISavage extends Runnable, Observer {
    void eat();
    void orderRefill();
}
