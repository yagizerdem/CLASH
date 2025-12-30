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
