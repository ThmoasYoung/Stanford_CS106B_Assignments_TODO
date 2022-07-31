/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "queue.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * The points function takes a string as input and calculate the points
 * the string can get. If the string's length less than 4, it can't get
 * any points otherwise, it can get points which equal string's length
 * reduce 3.
 */
int points(string str) {
    if(str.size() <= 3) {
        return 0;
    } else {
        return str.size() - 3;
    }
}


/*
 * The generateValidMoves function takes in the board(Grid of char) and
 * the current position at the board as input. It generates valid neighbours
 * of the current position as a set.
 */
Set<GridLocation> generateValidMoves(Grid<char>& board, GridLocation cur){
    Set<GridLocation> neighbours;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            GridLocation next(cur.row + i, cur.col + j);
            if((i == 0 && j == 0) || next.row < 0 || next.col < 0 \
                    || next.row >= board.numRows() || next.col >= board.numCols()) {
                continue;
            } else {
                neighbours.add(next);
            }
        }
    }
    return neighbours;
}

/*
 * The scoreBoardHelper function recursively explore all the valid paths
 * and check whether characters on these paths can combine a word or not
 */
void scoreBoardHelper(Grid<char>& board, Lexicon& lex, Set<string>& words, \
                      Vector<GridLocation>& path, Set<GridLocation>& mark) {

    string check;
    for(GridLocation step : path) {
        check += board[step];
    }
    if(check.length() > 3 && lex.contains(check)) {
        words.add(check);
    }
    if(!lex.containsPrefix(check)) {
        //Base case
        //Trim the rest search space
    } else { 
        //marked current position to aviod loops
        mark.add(path[path.size() - 1]);

        Set<GridLocation> neighbours = generateValidMoves(board, path[path.size() - 1]);
        for(GridLocation neighbour : neighbours) {
            if(isalpha(board[neighbour]) && !mark.contains(neighbour)){
                // choose a neighbour to search
                path.add(neighbour);
                // make a recursive call
                scoreBoardHelper(board, lex, words, path, mark);
                // unchoose the neighbour to explore the next
                path.remove(path.size() - 1);
            }
        }
        //unmarked current position to return back to the call fuction
        mark.remove(path[path.size() - 1]);
    }
}


/*
 * The scoreBoard function takes in a board consists of characters and a lexicon
 * as input. It start at different positions of the board and then call scoreBoardHelper
 * function to recursively explore all the words contained in the lexicon(length > 3)
 * and tallying up the points.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int totalPoints = 0;
    Set<string> words;
    Set<GridLocation> mark;

    for(int i = 0; i < board.numRows(); i++) {
        for(int j = 0; j < board.numCols(); j++) {
            GridLocation start(i,j);
            Vector<GridLocation> path;
            path.add(start);
            scoreBoardHelper(board,lex,words,path,mark);
        }
    }

    for(const string& word : words) {
        totalPoints += points(word);
    }
    cout << words << endl;
    return totalPoints;
}



/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Test generateValidMoves") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};
    GridLocation cur(0,0);
    Set<GridLocation> neighbours = {};
    GridLocation neighbour(0,1);
    neighbours.add(neighbour);
    neighbour.row = 1;
    neighbours.add(neighbour);
    neighbour.col = 0;
    neighbours.add(neighbour);
    EXPECT_EQUAL(generateValidMoves(board, cur), neighbours);
}

STUDENT_TEST("Test generateValidMoves") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};
    GridLocation cur(1,1);
    Set<GridLocation> neighbours = {};
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            GridLocation next(cur.row + i, cur.col + j);
            if(!(i == 0 && j == 0)) {
                neighbours.add(next);
            }
        }
    }
    EXPECT_EQUAL(generateValidMoves(board, cur), neighbours);
}

STUDENT_TEST("Stress Test scoreBoard, online Boggle Game 4x4") {
    Grid<char> board = {{'T','Y','E','O'},
                        {'O','C','S','S'},
                        {'F','U','T','R'},
                        {'N','W','H','N'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 84);
}

STUDENT_TEST("Stress Test scoreBoard, online Boggle Game 5x5") {
    Grid<char> board = {{'A','C','E','T','E'},
                        {'N','P','D','J','E'},
                        {'U','S','L','P','N'},
                        {'E','S','L','Y','I'},
                        {'N','R','D','I','N'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 122);
}
