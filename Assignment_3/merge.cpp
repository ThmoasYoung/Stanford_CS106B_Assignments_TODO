/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;

    while(!a.isEmpty() && !b.isEmpty()) {
        if(a.peek() <= b.peek()) {
            int pre = a.dequeue();
            if(a.isEmpty() || pre <= a.peek()) {
                result.enqueue(pre);
            } else {
                error("Input is not increasingly sorted!");
            }
        } else {
            int pre = b.dequeue();
            if(b.isEmpty() || pre <= b.peek()) {
                result.enqueue(pre);
            } else {
                error("Input is not increasingly sorted!");
            }
        }
    }

    if(a.isEmpty()) {
        while(!b.isEmpty()) {
            int pre = b.dequeue();
            if(b.isEmpty() || pre <= b.peek()) {
                result.enqueue(pre);
            } else {
                error("Input is not increasingly sorted!");
            }
        }
    }

    if(b.isEmpty()) {
        while(!a.isEmpty()) {
            int pre = a.dequeue();
            if(a.isEmpty() || pre <= a.peek()) {
                result.enqueue(pre);
            } else {
                error("Input is not increasingly sorted!");
            }
        }
    }

    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    if(all.isEmpty()) {
        return {};
    } else if(all.size() == 1) {
        return all[0];
    } else {
        Queue<int> result;
        Vector<Queue<int>> left = all.subList(0,all.size()/2);
        Vector<Queue<int>> right = all.subList(all.size()/2,all.size() - all.size()/2);
        Queue<int> leftResult = recMultiMerge(left);
        Queue<int> rightResult = recMultiMerge(right);
        result = binaryMerge(leftResult, rightResult);
        return result;
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

STUDENT_TEST("binaryMerge, two sequences with duplicate value") {
    Queue<int> a = {1, 1, 2, 4, 5, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 1, 1, 2, 3, 3, 4, 5, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, two unsorted sequences") {
    Queue<int> a = {1, 3, 2, 6, 5};
    Queue<int> b = {1, 7, 4};
    EXPECT_ERROR(binaryMerge(a, b));
}


STUDENT_TEST("binaryMerge, two sequences with negative numbers") {
    Queue<int> a = {-3, -1, 2, 6, 8};
    Queue<int> b = {1, 4, 7};
    Queue<int> expected = {-3, -1, 1, 2, 4, 6, 7, 8};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, long sequence with empty sequence") {
    Queue<int> a = {1,3,4,6,9,10,13,15,18,29,30,45,57,88,99,101,109,234,567,987};
    Queue<int> b = {};
    Queue<int> expected = {1,3,4,6,9,10,13,15,18,29,30,45,57,88,99,101,109,234,567,987};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("Time binaryMerge operation 1") {
    int n = 500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

STUDENT_TEST("Time binaryMerge operation 2") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

STUDENT_TEST("Time binaryMerge operation 3") {
    int n = 2000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

STUDENT_TEST("Time binaryMerge operation 4") {
    int n = 4000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

STUDENT_TEST("naiveMultiMerge, 0 sequences") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, small collection with many empty sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {},
                             {},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 3, 6, 9, 9, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("Time naiveMultiMerge operation 1") {
    int n = 10000;
    int k = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

STUDENT_TEST("Time naiveMultiMerge operation 2") {
    int n = 20000;
    int k = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

STUDENT_TEST("Time naiveMultiMerge operation 2") {
    int n = 20000;
    int k = 2000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge, length equals 200") {
    int n = 200;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge,length equals 2000") {
    int n = 2000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge,length equals 10000") {
    int n = 10000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("Time naiveMultiMerge operation") {
    int n = 10000;
    int k = 1000;

    for(int i = 0; i < 9; i++) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
        n += 10000;
    }

    k = 1;
    for(int i = 0; i < 5; i++) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
        k *= 10;
    }

}
