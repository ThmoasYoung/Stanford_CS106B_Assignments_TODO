// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function returns all valid moves from cur as a Set.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    if(cur.row < 0 || cur.row >= maze.numRows()
       || cur.col < 0 || cur.col >= maze.numCols() ||!maze[cur]) {
        return {};
    }
    Set<GridLocation> neighbors;
    GridLocation north,south,east,west;

    north.row = cur.row - 1;
    north.col = cur.col;
    if(north.row >= 0 && maze[north]) {
        neighbors.add(north);
    }
    south.row = cur.row + 1;
    south.col = cur.col;
    if(south.row < maze.numRows() && maze[south]) {
        neighbors.add(south);
    }

    west.row = cur.row;
    west.col = cur.col - 1;
    if(west.col >= 0 && maze[west]) {
        neighbors.add(west);
    }

    east.row = cur.row;
    east.col = cur.col + 1;
    if(east.col < maze.numCols() && maze[east]) {
        neighbors.add(east);
    }
    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    Set<GridLocation> uniqueSteps = {mazeExit};
    GridLocation mazeEntrance = {0,0};
    GridLocation cur = path.pop();

    while(!path.isEmpty()) {
        Set<GridLocation> neighbours = generateValidMoves(maze,cur);
        if(!neighbours.contains(path.peek())) {
            error("path is not a valid move away from the previous location");
        } else if(uniqueSteps.contains(path.peek())) {
            error("The path contains loops");
        } else if(path.size() == 1 ) {
            if(path.peek() != mazeEntrance) {
                error("Path does not start at maze entrance");
            } else {
                break;
            }
        } else {
            uniqueSteps.add(path.peek());
        }
        cur = path.pop();
    }

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    Queue<Stack<GridLocation>> possiblePaths;
    GridLocation mazeEntrance = {0, 0};
    GridLocation mazeExit = {maze.numRows() - 1, maze.numCols() - 1};
    Set<GridLocation> uniqueSteps;
    path.push(mazeEntrance);
    possiblePaths.enqueue(path);

    while(!possiblePaths.isEmpty()) {
        Stack<GridLocation> currentPath = possiblePaths.dequeue();
        uniqueSteps.add(currentPath.peek());
        MazeGraphics::highlightPath(currentPath, "blue", 100);
        if(currentPath.peek() == mazeExit) {
            path = currentPath;
            break;
        } else {
            Set<GridLocation> neighbours = generateValidMoves(maze,currentPath.peek());
            for(GridLocation nb : neighbours ) {
                if(!uniqueSteps.contains(nb)) {
                    Stack<GridLocation> newPath = currentPath;
                    newPath.push(nb);
                    possiblePaths.enqueue(newPath);
                }
            }
        }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on corner of 4x4 grid with walls") {
    Grid<bool> maze = {{true, false,false,true},
                       {true, true,false,false},
                       {false, true,true,false},
                       {true, false,true,false}};
    GridLocation corner = {1, 1};
    Set<GridLocation> expected = {{1, 0},{2,1}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on corner of 3x3 grid with round walls") {
    Grid<bool> maze = {{false, false,false},
                       {false, true,false},
                       {false, false,false}};
    GridLocation corner = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on corner of 1x1 grid") {
    Grid<bool> maze = {{true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on corner of 2x2 grid with all walls") {
    Grid<bool> maze = {{false, false},
                       {false, false}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("generateValidMoves on corner of 2x2 grid with invalid input") {
    Grid<bool> maze = {{true, true},
                       {true, true}};
    GridLocation corner = {10, 10};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("validatePath on solution for 1x1 maze") {
    Grid<bool> maze = {{true}};
    Stack<GridLocation> soln = {{0,0}};
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on correct solution loaded from file for 3x3 maze") {
    Grid<bool> maze;
    readMazeFile("res/3x3.maze", maze);
    Stack<GridLocation> soln;
    readSolutionFile("res/3x3.soln", soln);
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on invalid path should raise error for 3x3 maze") {
    Grid<bool> maze = {{true, false,false},
                       {true, true,true},
                       {false, false,true}};
    Stack<GridLocation> duplicate = { {0, 0}, {0 ,0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, duplicate));
}

STUDENT_TEST("validatePath on invalid path should raise error for 3x3 maze") {
    Grid<bool> maze = {{true, true,false},
                       {true, true,false},
                       {true, true,true}};
    Stack<GridLocation> loop = { {0, 0}, {1 ,0}, {1, 1}, {1, 0},{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};
    EXPECT_ERROR(validatePath(maze, loop));
}

STUDENT_TEST("validatePath on invalid path should raise error loaded from file for 4x4 maze") {
    Grid<bool> maze;
    readMazeFile("res/4x4.maze", maze);
    Stack<GridLocation> loop;
    readSolutionFile("res/4x4.soln", loop);
    EXPECT_ERROR(validatePath(maze, loop));
}

STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
