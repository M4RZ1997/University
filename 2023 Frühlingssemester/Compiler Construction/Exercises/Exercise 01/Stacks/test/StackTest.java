import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeEach;
import static org.junit.jupiter.api.Assertions.*;

public class StackTest {
    private SimpleStack simpleStack;
    @BeforeEach
    public void setup() {
        simpleStack = new SimpleStack();
    }

    @Test
    public void dummyTest() {
        simpleStack.push('E');
        simpleStack.push('{');
        assertEquals(2, simpleStack.size());
        assertEquals('{', simpleStack.pop());
        assertEquals('E', simpleStack.pop());
        assertEquals(0, simpleStack.size());
        assertTrue(simpleStack.isEmpty());
    }
}
