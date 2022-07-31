/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * The function drawSierpinskiTriangle takes the three corner points of the triangle
 * and the order as input and draws a Sierpinski triangle defined by those three corner
 * points and of the specific order. The return value is the total count of black
 * triangles drawn.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if(order == 0) {
        // Base case
        fillBlackTriangle(window, one, two, three);
        return 1;
    } else {
        // Recursive case
        GPoint mid1((one.x + two.x) /2 ,(one.y + two.y) /2);
        GPoint mid2((one.x + three.x) /2 ,(one.y + three.y) /2);
        GPoint mid3((two.x + three.x) /2 ,(two.y + three.y) /2);
        int count = 0;
        count += drawSierpinskiTriangle(window, one, mid1, mid2, order - 1);
        count += drawSierpinskiTriangle(window, two, mid1, mid3, order - 1);
        count += drawSierpinskiTriangle(window, three, mid2, mid3, order - 1);
        return count;
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

