import java.util.List;
import java.util.ArrayList;

public class SimpleStack implements IStack<Character>{
    private final List<Character> array;
    private int top;

    public SimpleStack(){
        array = new ArrayList<>();
        top = -1;
    }
    @Override
    public boolean isEmpty() {
        return top==-1;
    }

    @Override
    public int size() {
        return top+1;
    }

    @Override
    public void push(Character item) {
        array.add(item);
        top++;
    }

    @Override
    public Character top() {
        if(this.size()==0){
            System.exit(-1);
        }
        else{
            return array.get(top);
        }
        return null;
    }

    @Override
    public Character pop() {
        if(this.size()==0){
            System.exit(-1);
        }
        else{
            return array.remove(top--);
        }
        return null;
    }
}
