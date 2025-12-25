//
// Created by pennywise on 12/21/25.
//

#include <gtest/gtest.h>
#include "../src/expand/glob.h"

TEST(PatternMatcherTest, MatchTest) {
    Glob glob;

    // ----- STAR (*) -----
    EXPECT_TRUE(glob.match("test", "*"));
    //EXPECT_TRUE(glob.match("", "*"));
    EXPECT_TRUE(glob.match("abcdef", "*"));

    // ----- QUESTION (?) -----
    EXPECT_TRUE(glob.match("a", "?"));
    EXPECT_FALSE(glob.match("", "?"));
    EXPECT_FALSE(glob.match("ab", "?"));

    // ----- LITERAL -----
    EXPECT_TRUE(glob.match("test", "test"));
    EXPECT_FALSE(glob.match("test", "tes"));
    EXPECT_FALSE(glob.match("test", "testx"));

    // ----- STAR + LITERAL -----
    EXPECT_TRUE(glob.match("test", "t*"));
    EXPECT_TRUE(glob.match("test", "*t"));
    EXPECT_TRUE(glob.match("test", "t*t"));
    EXPECT_FALSE(glob.match("test", "t*z"));

    // ----- BRACKET BASIC -----
    EXPECT_TRUE(glob.match("a", "[abc]"));
    EXPECT_FALSE(glob.match("d", "[abc]"));

    // ----- BRACKET RANGE -----
    EXPECT_TRUE(glob.match("g", "[a-z]"));
    EXPECT_FALSE(glob.match("G", "[a-z]"));

    // ----- BRACKET NEGATION -----
    EXPECT_TRUE(glob.match("x", "[^abc]"));
    EXPECT_FALSE(glob.match("a", "[^abc]"));

    // ----- COMPLEX PATTERNS -----
    EXPECT_TRUE(glob.match("file123.txt", "file*.txt"));
    EXPECT_TRUE(glob.match("file9.txt", "file?.txt"));
    EXPECT_FALSE(glob.match("file99.txt", "file?.txt"));
}

TEST(PatternMatcherTest, MatchEdgeCases) {
    Glob glob;

    // ----- EMPTY STRING CASES -----
    EXPECT_TRUE(glob.match("", "*"));          // * matches empty
    EXPECT_FALSE(glob.match("", "?"));         // ? needs exactly 1 char
    EXPECT_TRUE(glob.match("", ""));           // empty matches empty
    EXPECT_FALSE(glob.match("a", ""));         // empty pattern

    // ----- MULTIPLE STARS -----
    EXPECT_TRUE(glob.match("abc", "**"));
    EXPECT_TRUE(glob.match("abc", "***"));
    EXPECT_TRUE(glob.match("abc", "*a*b*c*"));
    EXPECT_FALSE(glob.match("abc", "*a*b*d*"));

    // ----- STAR GREEDY EDGE -----
    EXPECT_TRUE(glob.match("abc", "*c"));
    EXPECT_TRUE(glob.match("abc", "*bc"));
    EXPECT_FALSE(glob.match("abc", "*bd"));

    // ----- QUESTION + STAR MIX -----
    EXPECT_TRUE(glob.match("abc", "?*?"));
    EXPECT_TRUE(glob.match("abc", "??*"));
    EXPECT_FALSE(glob.match("a", "?*?"));

    // ----- BRACKET SINGLE CHAR -----
    EXPECT_TRUE(glob.match("a", "[a]"));
    EXPECT_FALSE(glob.match("b", "[a]"));

    // ----- BRACKET RANGE EDGES -----
    EXPECT_TRUE(glob.match("a", "[a-z]"));
    EXPECT_TRUE(glob.match("z", "[a-z]"));
    EXPECT_FALSE(glob.match("A", "[a-z]"));

    // ----- BRACKET WITH DASH LITERAL -----
    EXPECT_TRUE(glob.match("-", "[-]"));
    EXPECT_TRUE(glob.match("-", "[a-z-]"));
    EXPECT_FALSE(glob.match("_", "[a-z-]"));

    // ----- COMPLEX NEGATED BRACKET -----
    EXPECT_FALSE(glob.match("x", "[^a-z]"));
    EXPECT_FALSE(glob.match("m", "[^a-z]"));

    // ----- OVERLAPPING RANGES -----
    EXPECT_TRUE(glob.match("5", "[0-9-5]"));
    EXPECT_TRUE(glob.match("-", "[0-9-5]"));
    EXPECT_FALSE(glob.match("A", "[0-9-5]"));

    // ----- MULTI BRACKET -----
    EXPECT_TRUE(glob.match("ab", "[a-z][a-z]"));
    EXPECT_FALSE(glob.match("a1", "[a-z][a-z]"));

    // ----- DOT HANDLING -----
    EXPECT_TRUE(glob.match(".txt", "*.txt"));
    EXPECT_TRUE(glob.match("a.txt", "*.txt"));
    EXPECT_FALSE(glob.match("txt", "*.txt"));

    // ----- FULL CONSUMPTION -----
    EXPECT_FALSE(glob.match("abcd", "abc"));
    EXPECT_FALSE(glob.match("abc", "abcd"));

    // ----- PATH-LIKE (NO / MATCH) -----
    EXPECT_TRUE(glob.match("a/b", "*"));       // if you forbid '/'
    EXPECT_TRUE(glob.match("/", "*"));
}

TEST(PatternMatcherTest, ExtremeEdgeCases) {
    Glob glob;

    // ----- NESTED BRACKET PATTERNS -----
    EXPECT_TRUE(glob.match("a]", "[[a]]"));           // if [ is literal inside
    EXPECT_TRUE(glob.match("[", "[[]"));             // literal [


    // ----- EMPTY BRACKETS -----
    EXPECT_FALSE(glob.match("", "[]"));              // empty bracket set
    EXPECT_FALSE(glob.match("a", "[]"));             // matches nothing
    EXPECT_FALSE(glob.match("", "[^]"));             // negated empty

    // ----- INVERTED RANGES -----
    EXPECT_FALSE(glob.match("a", "[z-a]"));          // z-a is invalid range
    EXPECT_FALSE(glob.match("m", "[z-a]"));
    EXPECT_TRUE(glob.match("z", "[z-a]"));           // or maybe treats as literals


    // ----- MULTIPLE CONSECUTIVE QUESTION MARKS -----
    EXPECT_TRUE(glob.match("abcde", "?????"));
    EXPECT_FALSE(glob.match("abcd", "?????"));
    EXPECT_FALSE(glob.match("abcdef", "?????"));

    // ----- STAR AT BEGINNING/MIDDLE/END -----
    EXPECT_TRUE(glob.match("xyzabc", "*abc"));
    EXPECT_TRUE(glob.match("abcxyz", "abc*"));
    EXPECT_TRUE(glob.match("xabcy", "*abc*"));
    EXPECT_FALSE(glob.match("xaybzc", "*abc*"));     // abc not consecutive

    // ----- OVERLAPPING STAR PATTERNS -----
    EXPECT_TRUE(glob.match("aaaa", "*a*a*"));
    EXPECT_TRUE(glob.match("ababa", "*a*a*"));
    EXPECT_FALSE(glob.match("bbb", "*a*a*"));
    EXPECT_FALSE(glob.match("aa", "*a*a*a*a*"));      // greedy matching

    // ----- BRACKET WITH SPECIAL CHARS -----
    EXPECT_TRUE(glob.match("*", "[*]"));             // literal *
    EXPECT_TRUE(glob.match("?", "[?]"));             // literal ?
    EXPECT_TRUE(glob.match("!", "[!]"));
    EXPECT_FALSE(glob.match("^", "[^x^]"));           // ^ not at start

    // ----- NEGATED BRACKET EDGE CASES -----
    EXPECT_TRUE(glob.match("!", "[^a-z]"));
    EXPECT_TRUE(glob.match("5", "[^a-z]"));
    EXPECT_FALSE(glob.match("a", "[^a-z]"));
    EXPECT_TRUE(glob.match("^", "[^a]"));            // ^ is literal here

    // ----- MULTIPLE RANGES IN BRACKET -----
    EXPECT_TRUE(glob.match("5", "[0-9a-z]"));
    EXPECT_TRUE(glob.match("m", "[0-9a-z]"));
    EXPECT_FALSE(glob.match("M", "[0-9a-z]"));
    EXPECT_TRUE(glob.match("Z", "[A-Za-z0-9]"));

    // ----- DASH POSITION IN BRACKETS -----
    EXPECT_TRUE(glob.match("-", "[-a-z]"));          // dash at start
    EXPECT_TRUE(glob.match("-", "[a-z-]"));          // dash at end
    EXPECT_TRUE(glob.match("-", "[a-z-0-9]"));       // dash between ranges
    EXPECT_FALSE(glob.match("_", "[-a-z]"));

    // ----- WHITESPACE EDGE CASES -----
    EXPECT_TRUE(glob.match(" ", "*"));
    EXPECT_TRUE(glob.match("  ", "**"));
    EXPECT_TRUE(glob.match(" a ", "?a?"));
    EXPECT_TRUE(glob.match("a b", "a?b"));          // ? matches space
    EXPECT_TRUE(glob.match("a b", "a?b"));           // correction: should match

    // ----- VERY LONG STRINGS -----
    std::string longStr(10000, 'a');
    EXPECT_TRUE(glob.match(longStr, "*"));
    EXPECT_TRUE(glob.match(longStr, "*a"));
    EXPECT_FALSE(glob.match(longStr, "*b"));
    EXPECT_TRUE(glob.match(longStr, std::string(10000, '?')));

    // ----- PATTERN LONGER THAN STRING -----
    EXPECT_FALSE(glob.match("a", "abc"));
    EXPECT_FALSE(glob.match("ab", "abc"));
    EXPECT_FALSE(glob.match("", "a"));
    EXPECT_FALSE(glob.match("abc", "abcd"));

    // ----- ALTERNATING WILDCARDS -----
    EXPECT_TRUE(glob.match("abcd", "?*?*?*?"));
    EXPECT_TRUE(glob.match("abcd", "*?*?*?*"));
    EXPECT_FALSE(glob.match("abc", "?*?*?*?"));

    // ----- GREEDY VS NON-GREEDY STAR -----
    EXPECT_TRUE(glob.match("aaa", "*a"));            // greedy: ** matches "aa"
    EXPECT_TRUE(glob.match("aaaa", "*aa"));          // greedy: ** matches "aa"
    EXPECT_FALSE(glob.match("aab", "*aa"));          // can't match trailing b

    // ----- BRACKET AFTER STAR -----
    EXPECT_TRUE(glob.match("abc", "*[c]"));
    EXPECT_TRUE(glob.match("xyzc", "*[a-z]"));
    EXPECT_FALSE(glob.match("xyz9", "*[a-z]"));

    // ----- STAR AFTER BRACKET -----
    EXPECT_TRUE(glob.match("abc", "[a]*"));
    EXPECT_FALSE(glob.match("bcd", "[a]*"));
    EXPECT_TRUE(glob.match("a", "[a]*"));

    // ----- QUESTION MARK IN BRACKETS -----
    EXPECT_TRUE(glob.match("?", "[?]"));             // literal ?
    EXPECT_FALSE(glob.match("a", "[?]"));
    EXPECT_TRUE(glob.match("?", "[?*]"));            // ? or *

    // ----- BACKSLASH IN BRACKETS (IF SUPPORTED) -----
    EXPECT_TRUE(glob.match("\\", "[\\]"));           // literal backslash
    EXPECT_TRUE(glob.match("\\", "[\\\\]"));         // escaped backslash
    EXPECT_FALSE(glob.match("a", "[\\]"));

    // ----- CASE SENSITIVITY -----
    EXPECT_FALSE(glob.match("ABC", "abc"));          // exact match fails
    EXPECT_FALSE(glob.match("ABC", "[a-z]*"));
    EXPECT_TRUE(glob.match("ABC", "[A-Z]*"));
    EXPECT_TRUE(glob.match("AbC", "[A-Za-z]*"));

    // ----- PARTIAL STAR MATCH -----
    EXPECT_TRUE(glob.match("prefix_middle_suffix", "prefix*suffix"));
    EXPECT_TRUE(glob.match("prefix_suffix", "prefix*suffix"));
    EXPECT_FALSE(glob.match("prefixsuffi", "prefix*suffix"));

    // ----- REDUNDANT PATTERNS -----
    EXPECT_TRUE(glob.match("a", "*?*"));
    EXPECT_TRUE(glob.match("ab", "*?*"));
    EXPECT_FALSE(glob.match("", "*?*"));             // needs at least 1 char

    // ----- SINGLE CHARACTER PATTERNS -----
    EXPECT_TRUE(glob.match("x", "?"));
    EXPECT_FALSE(glob.match("xy", "?"));
    EXPECT_TRUE(glob.match("x", "*"));
    EXPECT_TRUE(glob.match("x", "[x]"));

    // ----- NUMERIC RANGES -----
    EXPECT_TRUE(glob.match("5", "[0-9]"));
    EXPECT_TRUE(glob.match("0", "[0-9]"));
    EXPECT_TRUE(glob.match("9", "[0-9]"));
    EXPECT_FALSE(glob.match("a", "[0-9]"));

    // ----- MIXED ALPHANUMERIC -----
    EXPECT_TRUE(glob.match("a1b2", "[a-z0-9]*"));
    EXPECT_TRUE(glob.match("a1B2", "[a-z0-9]*"));   // capital B
    EXPECT_TRUE(glob.match("test123", "test[0-9]*"));

    // ----- DOT AS SPECIAL VS LITERAL -----
    EXPECT_TRUE(glob.match(".", "."));               // exact match
    EXPECT_TRUE(glob.match(".", "*"));               // star matches
    EXPECT_TRUE(glob.match(".", "?"));               // ? matches
    EXPECT_TRUE(glob.match(".", "[.]"));             // bracket literal

    // ----- CONSECUTIVE BRACKETS -----
    EXPECT_TRUE(glob.match("ab", "[a][b]"));
    EXPECT_FALSE(glob.match("aa", "[a][b]"));
    EXPECT_TRUE(glob.match("xy", "[x-z][x-z]"));

    // ----- EMPTY PATTERN PARTS -----
    EXPECT_FALSE(glob.match("abc", "ab**cd"));       // no chars between ** and cd
    EXPECT_TRUE(glob.match("abc", "a**c"));          // ** collapses to *

    // ----- BOUNDARY TESTS -----
    EXPECT_TRUE(glob.match(std::string(1, '\0'), "?"));      // null char
    EXPECT_TRUE(glob.match(std::string(1, '\xFF'), "?"));    // high byte
    EXPECT_TRUE(glob.match("\n", "?"));                      // newline
    EXPECT_TRUE(glob.match("\t", "?"));                      // tab

}

TEST(GlobResolveTest, BasicGlobResolver) {
    Glob glob;
    std::string cwd = "/home/pennywise/Desktop/globtest";
    std::string wildcard = "ne?*d[1-9]/j*?a.txt";
    wildcard = "/*/pennywise/Desktop/*/ne?*d[1-9]/j*?a.txt";

    std::vector<std::string> resoledAbsPaths =  glob.resolveGlob(wildcard, cwd);

    ;

}
