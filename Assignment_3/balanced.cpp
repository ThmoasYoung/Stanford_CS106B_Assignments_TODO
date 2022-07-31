/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * The operatorsFrom Function takes in a string and returns a string consists only of
 * bracket characters from the string.
 */
string operatorsFrom(string str) {
    if(str.empty()) {
        // Base case
        return "";
    } else {
        // Recursive case
        string result = "";
        if(str[0] == '{' || str[0] == '[' || str[0] == '(' || \
                str[0] == '}' || str[0] == ']' || str[0] == ')' ) {
            result += str[0];
            result += operatorsFrom(str.substr(1));
        } else {
            result += operatorsFrom(str.substr(1));
        }
        return result;
    }
}

/*
 * The isMatchHelper function takes in two characters and detect whether
 * they are a pair of matched parenteses
 */
bool isMatchHelper(char left, char right) {
    if(left == '{' && right == '}') {
        return true;
    } else if(left == '[' && right == ']') {
        return true;
    } else if(left == '(' && right == ')') {
        return true;
    } else {
        return false;
    }
}

/*
 * The operatorAreMatchedHelper function use a stack help check whether
 * the operator string is balanced or not.
 */
bool operatorsAreMatchedHelper(string ops, Stack<char>& s) {
    if(ops.empty()) {
        // Base case
        if(s.isEmpty()) {
            return true;
        } else {
            cout<<"The line is missing a closing parenthesis."<<endl;
            return false;
        }
    } else {
        // recurive case
        if(ops[0] == '}' || ops[0] == ']' || ops[0] == ')') {
            // if the first character is closing parenthesis, check if the
            // corresponding opening parenthesis is in the stack

            if(s.isEmpty()) {
                // if stack is empty, there's no matched opening parenthesis before
                // the closing parenthesis

                cout<<"The closing parenthesis comes before the opening parenthesis."<<endl;
                return false;
            } else if(isMatchHelper(s.peek(), ops[0])) {
                // if stack is not empty and the peek macthes the closing parenthesis
                // remove the peek of the stack, and check the rest of the operators are balanced
                // or not

                s.pop();
                return operatorsAreMatchedHelper(ops.substr(1),s);
            } else {
                // if stack is not empty and the peek doesn't match the closing parenthesis
                // it means parentheses are improperly nested

                cout<<"The parentheses and braces are improperly nested."<<endl;
                return false;
            }
        } else {
            // if the first character is opening parenthesis, push it into the
            // stack and check the rest of the operators

            s.push(ops[0]);
            return operatorsAreMatchedHelper(ops.substr(1),s);
        }
    }
}

/*
 * The operatorsAreMatched takes in a string consists only of
 * brackets to check whether is balanced or not. It defines a
 * stack and call operatorAreMatchedHelper to help
 */
bool operatorsAreMatched(string ops) {
    Stack<char> s;
    return operatorsAreMatchedHelper(ops,s);


}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}


STUDENT_TEST("operatorsFrom on real code") {
    EXPECT_EQUAL(operatorsFrom(" int main() { int x = 2 * (vec[2] + 3);  \
                                 x = (1 + random()); }"), "(){([])(())}");
}

STUDENT_TEST("operatorsFrom on a unbalanced example") {
    EXPECT_EQUAL(operatorsFrom("vec[3])("), "[])(");
}

