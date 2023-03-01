public class StringChecker {
    public boolean isBalanced(String string) {
        SimpleStack simpleStack = new SimpleStack();

        // for a string to be considered balanced two conditions must be met:
        // - the number of opening brackets must be equal to the number of closing ones
        // - they must be closed in inverse order of opening

        for (char c: string.toCharArray()) {
            if (c=='{' || c=='(' || c=='<' || c=='[') {
                simpleStack.push(c);
            }

            // if the character is a closing bracket and the stack is empty there was no corresponding
            // opening bracket
            if (c=='}' || c==')' || c=='>' || c==']') {
                if (simpleStack.isEmpty()) {
                    return false;
                }
                char nextChar = ' ';
                switch (c) {
                    case '}' -> {
                        nextChar = simpleStack.pop();
                        if (nextChar == '(' || nextChar == '[' || nextChar == '<') {
                            return false;
                        }
                    }
                    case ')' -> {
                        nextChar = simpleStack.pop();
                        if (nextChar == '{' || nextChar == '[' || nextChar == '<') {
                            return false;
                        }
                    }
                    case ']' -> {
                        nextChar = simpleStack.pop();
                        if (nextChar == '{' || nextChar == '(' || nextChar == '<') {
                            return false;
                        }
                    }
                    case '>' -> {
                        nextChar = simpleStack.pop();
                        if (nextChar == '{' || nextChar == '(' || nextChar == '[') {
                            return false;
                        }
                    }
                    default -> {}
                }
            }
        }
        return simpleStack.isEmpty();
    }
}
