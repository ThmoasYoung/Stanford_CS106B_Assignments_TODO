// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// cleanToken takes in a string of characters and returns a "cleaned" version
// of that token. It will trim all punctuation from the beginning and end and
// then convert the token to lowercase. If the token contains only non-letters,
// it returns empty string.
string cleanToken(string s)
{
    while(ispunct(s[0]) || ispunct(s[s.length() - 1])) {
        if(ispunct(s[0])) {
            s.erase(0,1);
        } else {
            s.erase(s.length() - 1);
        }
    }
    for(char ch : s) {
        if(isalpha(ch)) {
            s = toLowerCase(s);
            return s;
        }
    }
    return "";
}

// The fucntion takes in a string and returns a Set of unique cleaned
// tokens in that string. It first tokenizes the string and then clean
// each token to store it in a Set.
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> uncleanedTokens = stringSplit(text," ");
    for(string s : uncleanedTokens) {
        string token = cleanToken(s);
        if(token.length() > 0) {
            tokens.add(token);
        }
    }

    return tokens;
}

// This function reads the content from the database file and
// processes it into the form of an inverted index. It first read
// the contents  into a Vector of lines, then call gatherTokens
// to extract the set of unique tokens. For each token in the page,
// add the page's URL to a set to build the map index.
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if(!openFile(in,dbfile)) {
        error("Cannot open file named " + dbfile);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    int count = 0;
    for(int i = 0; i < lines.size(); i += 2) {
        Set<string> tokens = gatherTokens(lines[i + 1]);
        for(string token : tokens) {
            index[token] += lines[i];
        }
        count++;
    }
    return count;
}

// findQueryMatches takes in query and index and return corresponding
// web pages according to query. For a single search term, it returns
// all web pages  that contain that term. For A sequence of terms, it
// returns corresponding a combined result according to modifer
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;

    Vector<string> terms = stringSplit(query," ");

    if(terms.size() <= 0) {
        return {};
    } else if(terms.size() == 1) {
        result = index[cleanToken(terms[0])];
        return result;
    } else {
        result += index[cleanToken(terms[0])];
        for(int i = 1; i< terms.size(); i++) {
            string term = terms[i];
            if(term[0] == '+') {
                result *= index[cleanToken(term)];
            } else if(term[0] == '-') {
                result -= index[cleanToken(term)];
            } else {
                result += index[cleanToken(term)];
            }
        }
        return result;
    }


}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    Map<string,Set<string>> index;
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << buildIndex(dbfile,index) << " pages containing " << \
            index.size() << " unique terms" << endl;
    string input = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    while(input != "" ) {

        Set<string> matches = findQueryMatches(index,input);
        cout << "Found " << matches.size() << " matching pages "<< endl;
        cout << matches << endl;
        cout << '\n';
        input = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on non-word strings"){
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("         "), "");
    EXPECT_EQUAL(cleanToken("~!@#$%^^^&*!!!"), "");
}

STUDENT_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hElloHeLLohEllOHELLohelloHELLO"), "hellohellohellohellohellohello");
    EXPECT_EQUAL(cleanToken("wor ld"), "wor ld");
    EXPECT_EQUAL(cleanToken("C-S*1-0-6-B"), "c-s*1-0-6-b");
}

STUDENT_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("@#$%/hello/!@#$"), "hello");
    EXPECT_EQUAL(cleanToken("!@*&%$~woRLD!><:"), "world");
}

STUDENT_TEST("gatherTokens from long sentences") {
    Set<string> tokens = gatherTokens("The function first tokenizes the body text —\
                                      this means to separate the string into words \
                                      using the space character as delimiter.");
    EXPECT_EQUAL(tokens.size(), 18);
    EXPECT(tokens.contains("the"));
    EXPECT(!tokens.contains("The"));
}

STUDENT_TEST("gatherTokens from non-letter strings") {
    Set<string> tokens = gatherTokens("@#$@ (*)^%*^& &%#%$@! )(*&%^& 12354123 3*3=9");
    EXPECT_EQUAL(tokens.size(), 0);
}

STUDENT_TEST("gatherTokens from empty strings") {
    Set<string> tokens = gatherTokens("");
    EXPECT_EQUAL(tokens.size(), 0);
}

STUDENT_TEST("buildIndex from mid.txt, 16 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/mid.txt", index);
    EXPECT_EQUAL(nPages, 16);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

STUDENT_TEST("buildIndex from empty.txt, 0 pages, 0 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/empty.txt", index);
    EXPECT_EQUAL(nPages, 0);
    EXPECT_EQUAL(index.size(), 0);
}

STUDENT_TEST("Time trials of buildIndex from website.txt") {
    Map<string, Set<string>> index;
    TIME_OPERATION(32, buildIndex("res/website.txt", index));
}
STUDENT_TEST("findQueryMatches from empty.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/empty.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT(matchesRed.isEmpty());
}

STUDENT_TEST("findQueryMatches from tiny.txt, mixcase compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "Red fiSh");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "rEd +fIsh");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "reD -fisH");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
    Set<string> matchesFishOrFish = findQueryMatches(index, "FISH fisH");
    EXPECT_EQUAL(matchesFishOrFish.size(),3);
    Set<string> matchesFishAndFish = findQueryMatches(index, "FISH +fisH");
    EXPECT_EQUAL(matchesFishAndFish.size(),3);
    Set<string> matchesFishWithoutFish = findQueryMatches(index, "FISH -fisH");
    EXPECT(matchesFishWithoutFish.isEmpty());
}
