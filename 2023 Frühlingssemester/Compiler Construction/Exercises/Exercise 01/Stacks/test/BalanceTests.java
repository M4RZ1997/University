import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class BalanceTests {
    private StringChecker stringChecker;
    @BeforeEach
    public void setup() {
        stringChecker = new StringChecker();
    }

    @Test
    public void balancedStringsTest(){
        assertTrue(stringChecker.isBalanced("(hello)(world)"));
        assertTrue(stringChecker.isBalanced("(a[s{d<f>g}h]j)<>"));
        assertTrue(stringChecker.isBalanced("<>()"));
    }

    @Test
    public void notBalancedStringsTest(){
        assertFalse(stringChecker.isBalanced("static public void main(String args[]) {"));
        assertFalse(stringChecker.isBalanced("{[(>)]}"));
        assertFalse(stringChecker.isBalanced("({<]})"));
    }
}
