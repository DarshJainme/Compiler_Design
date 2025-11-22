
int main() {
    int counter = 0;

    // The 'until' loop executes the body as long as the condition is false.
    // This is equivalent to 'while (!(counter >= 10))' or 'while (counter < 10)'.
    // This loop will run 10 times.
    until (counter >= 10) {
        counter = counter + 1;
    }

    // At this point, counter should be 10.

    int another_counter = 5;
    
    // This loop should not execute at all.
    // The condition (5 > 3) is true, so the loop body is skipped.
    until (another_counter > 3) {
        another_counter = 0; // This line should not be executed.
    }

    // int another_counter = 2;
    
    // This loop should execute
    // The condition (5 > 3) is true, so the loop body is skipped.
    until (another_counter > 3) {
        another_counter = 5; // This line should not be executed.
    }
    // The final return value should be 10 + 5 = 15.
    // This verifies the first loop ran correctly and the second was skipped.
    return counter + another_counter;
}