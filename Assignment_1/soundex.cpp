/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 *It didn't check the first character s[0];
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*This fuction takes one argument char ch and map it into a single
 *number string. First, turn char to a string and use toLowerCase to
 *lower its case and then use do the map below.
 * a e h i o u w y -> 0
 * b f p v -> 1
 * c g j k s x z -> 2
 * d t -> 3
 * l -> 4
 * m n -> 5
 * r -> 6
 * Use string result to store the mapped value and return it at last;
 */

string soundmap(char ch) {

    string temp = charToString(ch);
    temp = toLowerCase(temp);
    ch = stringToChar(temp);

    string result;
    if(ch == 'b' || ch == 'f' || ch == 'p' || ch == 'v') {
        result = "1";
    }
    else if(ch == 'c' || ch == 'g' || ch == 'j' || ch == 'k'
           || ch == 's' || ch == 'x' || ch == 'z') {
        result = "2";
    }
    else if(ch == 'd' || ch == 't') {
        result = "3";
    }
    else if(ch == 'l') {
        result = "4";
    }
    else if(ch == 'm' || ch == 'n') {
        result = "5";
    }
    else if(ch == 'r') {
        result ="6";
    }
    else {
        result = "0";
    }

    return result;
}

/* This function takes one argument string input and turn it into a more
 * concise three-character string by do the steps below;
 * First, ignore all 0 and if there are consecutive equal numbers,
 * keep only one, and delete the rest.
 * Then, if the result exceeds 3 digits, take the first 3 digits;
 * if the result is less than 3 digits, add 0 to the back.
 */

string tidyUpString(string input) {
    string result = "";
    char temp = '0';
    for(char elements:input) {
        if (elements != temp) {
            temp = elements;
            if(elements != '0') {
                result += elements;
            }
        }

        if(result.length() >= 3) {
            break;
        }
    }
    for(int i = result.length();i < 3 ;i++) {
        result += "0";
    }
    return result;
}

/* This function takes one argument string s and  change s into
 * a code made of 4 character. The first character of the code is
 * the uppercase of the string's inital while the other three characters
 * are numbers mapped from the else characters.
 * Use soundmap to do the map.
 * Use tidyUpString to transform the temporary code string to the result code;
 */

string soundex(string s) {
    s = removeNonLetters(s);
    string result = toUpperCase(charToString(s[0]));
    string tempCode = "";
    for (int i = 1; i < s.length(); i++)
    {
       tempCode +=  soundmap(s[i]);
    }

    while((stringToChar(soundmap(s[0]))!= '0' || tempCode[0] != '0') && tempCode[0] == stringToChar(soundmap(s[0])) ) {
        tempCode.erase(0,1);
        tempCode.append("0");
    }

    tempCode = tidyUpString(tempCode);
    result += tempCode;
    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    cout << endl;
    string input = getLine("Enter a surname (RETURN to quit): ");
    while (input != "RETURN") {
        string inputCode = soundex(input);
        Vector<string> matchedNames;
        cout << "Soundex code is " << inputCode << endl;
        for(int i = 0; i < databaseNames.size();i++ ) {
            if(inputCode == soundex(databaseNames[i])) {
                matchedNames.add(databaseNames[i]);
            }
        }
        cout << "Matches from database: " << matchedNames << endl;
        cout << endl;
        input = getLine("Enter a surname (RETURN to quit): ");
    }
    cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces") {
    string s = "0'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Hara");
    s = "22Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "18tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

STUDENT_TEST("Test whether soundmap fuction is correct") {
    EXPECT_EQUAL(soundmap('a'),"0");
    EXPECT_EQUAL(soundmap('B'),"1");
    EXPECT_EQUAL(soundmap('c'),"2");
    EXPECT_EQUAL(soundmap('d'),"3");
    EXPECT_EQUAL(soundmap('L'),"4");
    EXPECT_EQUAL(soundmap('m'),"5");
    EXPECT_EQUAL(soundmap('R'),"6");
}

STUDENT_TEST("Test whether tidyUpString fuction is correct") {
    EXPECT_EQUAL(tidyUpString("55002062"), "526");
    EXPECT_EQUAL(tidyUpString("006600"),"600");
    EXPECT_EQUAL(tidyUpString("1234"),"123");
    EXPECT_EQUAL(tidyUpString("635"),"635");
    EXPECT_EQUAL(tidyUpString("666"),"600");
    EXPECT_EQUAL(tidyUpString("000"),"000");
    EXPECT_EQUAL(tidyUpString("113423"),"134");
    EXPECT_EQUAL(tidyUpString("11111333344444"),"134");
}

STUDENT_TEST("Test whether Van Niekerk can be coded correctly") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

STUDENT_TEST("Test whether Schwarz can be coded correctly") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}
