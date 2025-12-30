//
// Created by pennywise on 12/30/25.
//

#include <gtest/gtest.h>
#include "../src/lexer/wordSplitter.h"

TEST(GLOBEXPANDTEST, SingleQuotedGlobChar) {
    WordSplitter splitter;
    std::string rawCommand = "echo '*.txt'";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, true);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, EscapedGlobChar) {
    WordSplitter splitter;
    std::string rawCommand = "echo \\*.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, false);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, true);
}


TEST(GLOBEXPANDTEST, MixedQuotedAndUnquotedGlob) {
    WordSplitter splitter;
    std::string rawCommand = "echo afa\"*\"afa*.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, true);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, QuotedButNoGlobChar) {
    WordSplitter splitter;
    std::string rawCommand = "echo \"abcdef\"";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, false);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}

TEST(GLOBEXPANDTEST, NonGlobEscapeDoesNotDisableGlob) {
    WordSplitter splitter;
    std::string rawCommand = "echo \\a*.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, false);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, MultipleGlobOneQuoted) {
    WordSplitter splitter;
    std::string rawCommand = "echo *\"*\"*.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, true);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, PureUnquotedGlob) {
    WordSplitter splitter;
    std::string rawCommand = "echo *.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, false);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, IdentifyWordHasEscapedOrQuotedGlobChar) {
    WordSplitter splitter;
    std::string rawCommand = "ehco *yagiz'*'erem.txt";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 2);

    ASSERT_EQ(stream[1].hasQuotedGlobChar, true);
    ASSERT_EQ(stream[1].hasEscapedGlobChar, false);

    ASSERT_EQ(stream[0].hasQuotedGlobChar, false);
    ASSERT_EQ(stream[0].hasEscapedGlobChar, false);
}


TEST(GLOBEXPANDTEST, LongInputQuotedGlobInMiddle) {
    WordSplitter splitter;
    std::string rawCommand =
        "echo before *mid\"*\"dle*.txt after end";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 5);

    // echo
    ASSERT_FALSE(stream[0].hasQuotedGlobChar);
    ASSERT_FALSE(stream[0].hasEscapedGlobChar);

    // before
    ASSERT_FALSE(stream[1].hasQuotedGlobChar);
    ASSERT_FALSE(stream[1].hasEscapedGlobChar);

    // *mid"* "dle*.txt  -> quoted glob var
    ASSERT_TRUE(stream[2].hasQuotedGlobChar);
    ASSERT_FALSE(stream[2].hasEscapedGlobChar);

    // after
    ASSERT_FALSE(stream[3].hasQuotedGlobChar);
    ASSERT_FALSE(stream[3].hasEscapedGlobChar);

    // end
    ASSERT_FALSE(stream[4].hasQuotedGlobChar);
    ASSERT_FALSE(stream[4].hasEscapedGlobChar);
}


TEST(GLOBEXPANDTEST, LongInputEscapedAndNormalGlob) {
    WordSplitter splitter;
    std::string rawCommand =
        "ls start \\*.txt mid *.cpp finish";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 6);

    // ls
    ASSERT_FALSE(stream[0].hasQuotedGlobChar);
    ASSERT_FALSE(stream[0].hasEscapedGlobChar);

    // start
    ASSERT_FALSE(stream[1].hasQuotedGlobChar);
    ASSERT_FALSE(stream[1].hasEscapedGlobChar);

    // \*.txt -> escaped glob
    ASSERT_FALSE(stream[2].hasQuotedGlobChar);
    ASSERT_TRUE(stream[2].hasEscapedGlobChar);

    // *.cpp -> normal glob
    ASSERT_FALSE(stream[3].hasQuotedGlobChar);
    ASSERT_FALSE(stream[3].hasEscapedGlobChar);

    // finish
    ASSERT_FALSE(stream[4].hasQuotedGlobChar);
    ASSERT_FALSE(stream[4].hasEscapedGlobChar);
}


TEST(GLOBEXPANDTEST, VeryLongMixedComplexInput) {
    WordSplitter splitter;
    std::string rawCommand =
        "cmd a b* c\"*\"d*.txt e \\f*.md g h\"abc\"i*.c j";
    std::vector<Word> stream = splitter.wordStream(rawCommand);

    ASSERT_EQ(stream.size(), 9);

    // cmd
    ASSERT_FALSE(stream[0].hasQuotedGlobChar);
    ASSERT_FALSE(stream[0].hasEscapedGlobChar);

    // a
    ASSERT_FALSE(stream[1].hasQuotedGlobChar);
    ASSERT_FALSE(stream[1].hasEscapedGlobChar);

    // b*
    ASSERT_FALSE(stream[2].hasQuotedGlobChar);
    ASSERT_FALSE(stream[2].hasEscapedGlobChar);

    // c"* "d*.txt -> quoted glob var
    ASSERT_TRUE(stream[3].hasQuotedGlobChar);
    ASSERT_FALSE(stream[3].hasEscapedGlobChar);

    // e
    ASSERT_FALSE(stream[4].hasQuotedGlobChar);
    ASSERT_FALSE(stream[4].hasEscapedGlobChar);

    // \f*.md -> \f normal escape, * unescaped
    ASSERT_FALSE(stream[5].hasQuotedGlobChar);
    ASSERT_FALSE(stream[5].hasEscapedGlobChar);

    // g
    ASSERT_FALSE(stream[6].hasQuotedGlobChar);
    ASSERT_FALSE(stream[6].hasEscapedGlobChar);

    // h"abc"i*.c -> quoted var ama glob quoted değil
    ASSERT_FALSE(stream[7].hasQuotedGlobChar);
    ASSERT_FALSE(stream[7].hasEscapedGlobChar);

    // j
    ASSERT_FALSE(stream[8].hasQuotedGlobChar);
    ASSERT_FALSE(stream[8].hasEscapedGlobChar);
}

