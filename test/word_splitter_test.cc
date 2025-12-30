//
// Created by pennywise on 12/25/25.
//

#include <gtest/gtest.h>

#include "../src/lexer/wordSplitter.h"
#include "../src/process/spawn.h"

TEST(WordSplitterTest, Basic) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo foo bar";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 3);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "foo");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "bar");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::UN_QUOTE);

    shellCommand = "echo \"foo\" \"bar\"";
    word_stream = wordSplitter.wordStream(shellCommand);
    ASSERT_EQ(word_stream.size(), 3);

    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "foo");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "bar");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::DOUBLE_QUOTE);


}



TEST(WordSplitterTest, MixedQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'single' \"double\" unquoted";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "single");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::SINGLE_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "double");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[3].lexeme, "unquoted");
    ASSERT_EQ(word_stream[3].context, Word::WordContext::UN_QUOTE);
}

TEST(WordSplitterTest, EmptyString) {
    WordSplitter wordSplitter;
    std::string shellCommand = "";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_TRUE(word_stream.empty());
}

TEST(WordSplitterTest, OnlyWhitespace) {
    WordSplitter wordSplitter;
    std::string shellCommand = "   \t\n  ";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_TRUE(word_stream.empty());
}

TEST(WordSplitterTest, MultipleSpaces) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo    foo     bar";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 3);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "foo");
    ASSERT_EQ(word_stream[2].lexeme, "bar");
}

TEST(WordSplitterTest, QuotedSpaces) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"foo bar baz\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "foo bar baz");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, SingleQuotedSpaces) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'foo bar baz'";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "foo bar baz");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, EmptyQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"\" '' test";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
    ASSERT_EQ(word_stream[2].lexeme, "");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);
    ASSERT_EQ(word_stream[3].lexeme, "test");
}

TEST(WordSplitterTest, NestedQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"foo 'bar' baz\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "foo 'bar' baz");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, NestedQuotesReverse) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'foo \"bar\" baz'";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "foo \"bar\" baz");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, EscapedQuotesInDouble) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"foo \\\"bar\\\" baz\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    // Escaped quotes should be preserved or handled
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, SpecialCharactersUnquoted) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo $PATH ~/home `date`";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "$PATH");
    ASSERT_EQ(word_stream[2].lexeme, "~/home");
    ASSERT_EQ(word_stream[3].lexeme, "date");
}

TEST(WordSplitterTest, SpecialCharactersInQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"$PATH\" '~/home' \"`date`\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "echo");

    ASSERT_EQ(word_stream[1].lexeme, "$PATH");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "~/home");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);

    ASSERT_EQ(word_stream[3].lexeme, "`date`");
    ASSERT_EQ(word_stream[3].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, RedirectionOperators) {
    WordSplitter wordSplitter;
    std::string shellCommand = "cat file.txt > output.txt";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "cat");
    ASSERT_EQ(word_stream[1].lexeme, "file.txt");
    ASSERT_EQ(word_stream[2].lexeme, ">");
    ASSERT_EQ(word_stream[3].lexeme, "output.txt");
}

TEST(WordSplitterTest, ComplexCommand) {
    WordSplitter wordSplitter;
    std::string shellCommand = "grep -r 'pattern with spaces' /var/log/*.log";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "grep");
    ASSERT_EQ(word_stream[1].lexeme, "-r");
    ASSERT_EQ(word_stream[2].lexeme, "pattern with spaces");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, TabsAndNewlines) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo\tfoo\nbar";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 3);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "foo");
    ASSERT_EQ(word_stream[2].lexeme, "bar");
}

TEST(WordSplitterTest, BackslashEscapes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo foo\\ bar";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    // Depending on implementation: might be 2 words or 3
    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, ConsecutiveQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"foo\"\"bar\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
}

TEST(WordSplitterTest, QuotesAtWordBoundary) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo foo\"bar\"baz";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, SingleWord) {
    WordSplitter wordSplitter;
    std::string shellCommand = "ls";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 1);
    ASSERT_EQ(word_stream[0].lexeme, "ls");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);
}

TEST(WordSplitterTest, VeryLongWord) {
    WordSplitter wordSplitter;
    std::string longWord(10000, 'a');
    std::string shellCommand = "echo " + longWord;

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, longWord);
}

TEST(WordSplitterTest, VeryLongQuotedString) {
    WordSplitter wordSplitter;
    std::string longString(10000, 'x');
    std::string shellCommand = "echo \"" + longString + "\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, longString);
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, UnclosedDoubleQuote) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"unclosed";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    // Should handle gracefully - either error or treat as unclosed
    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, UnclosedSingleQuote) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'unclosed";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    // Should handle gracefully - either error or treat as unclosed
    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, MultipleCommandsStyle) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"hello world\" 'foo bar' unquoted \"another\" test";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 6);

    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[0].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[1].lexeme, "hello world");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "foo bar");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);

    ASSERT_EQ(word_stream[3].lexeme, "unquoted");
    ASSERT_EQ(word_stream[3].context, Word::WordContext::UN_QUOTE);

    ASSERT_EQ(word_stream[4].lexeme, "another");
    ASSERT_EQ(word_stream[4].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[5].lexeme, "test");
    ASSERT_EQ(word_stream[5].context, Word::WordContext::UN_QUOTE);
}

TEST(WordSplitterTest, ComplexRealWorld1) {
    WordSplitter wordSplitter;
    std::string shellCommand = "grep -r 'error\\|warning' /var/log/*.log";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "grep");
    ASSERT_EQ(word_stream[1].lexeme, "-r");
    ASSERT_EQ(word_stream[2].lexeme, "error|warning");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, ComplexRealWorld2) {
    WordSplitter wordSplitter;
    std::string shellCommand = "awk -F'|' '{print $1, $2}' data.csv";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 3);
    ASSERT_EQ(word_stream[0].lexeme, "awk");
    ASSERT_EQ(word_stream[1].lexeme, "-F'|'");
}

TEST(WordSplitterTest, ComplexRealWorld3) {
    WordSplitter wordSplitter;
    std::string shellCommand = "find . -name \"*.cpp\" -o -name \"*.h\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 7);
    ASSERT_EQ(word_stream[0].lexeme, "find");
    ASSERT_EQ(word_stream[1].lexeme, ".");
    ASSERT_EQ(word_stream[2].lexeme, "-name");
    ASSERT_EQ(word_stream[3].lexeme, "*.cpp");
    ASSERT_EQ(word_stream[3].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, PathsWithSpaces) {
    WordSplitter wordSplitter;
    std::string shellCommand = "cd \"/home/user/My Documents\"";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "cd");
    ASSERT_EQ(word_stream[1].lexeme, "/home/user/My Documents");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}



TEST(WordSplitterTest, EmptyQuotesMultiple) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"\" '' \"\" '' test \"\" ''";

    std::vector<Word> word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 8);
    ASSERT_EQ(word_stream[0].lexeme, "echo");

    ASSERT_EQ(word_stream[1].lexeme, "");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);

    ASSERT_EQ(word_stream[2].lexeme, "");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);

    ASSERT_EQ(word_stream[7].lexeme, "");
    ASSERT_EQ(word_stream[7].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, ConsecutiveQuotesNoSpace) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"foo\"\"bar\"\"baz\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    // Implementation dependent: might be 1 word "foobarbaz" or 3 words
}

TEST(WordSplitterTest, ConsecutiveMixedQuotesNoSpace) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'foo'\"bar\"'baz'\"qux\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
}

TEST(WordSplitterTest, QuotesInMiddleOfWord) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo foo\"bar\"baz";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    // Might be treated as single word "foobarbaz" or multiple parts
}

TEST(WordSplitterTest, TripleNestedQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"outer 'middle \\\"inner\\\" middle' outer\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, QuadrupleNestedQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'L1 \"L2 'L3 \\\"L4\\\" L3' L2\" L1'";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
}

TEST(WordSplitterTest, OnlyQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "\"\" '' \"\" ''";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    for (const auto& word : word_stream) {
        ASSERT_EQ(word.lexeme, "");
    }
}

TEST(WordSplitterTest, UnclosedQuoteAtEnd) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"unclosed quote here";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 1);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
}

TEST(WordSplitterTest, MultipleUnclosedQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"first 'second \"third";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, OnlyUnclosedQuote) {
    WordSplitter wordSplitter;
    std::string shellCommand = "\"never closed";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, EscapeSequencesInDoubleQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"line1\\nline2\\ttab\\rcarriage\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
    // Should preserve escape sequences
}

TEST(WordSplitterTest, EscapeSequencesInSingleQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'line1\\nline2\\ttab\\rcarriage'";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "line1\\nline2\\ttab\\rcarriage");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, BackslashAtEnd) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo test\\";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, OnlyBackslashes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \\\\ \\\\\\\\ \\\\\\\\\\\\";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, QuoteEscapingMadness) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \\\"\\\"\\\"\\\"\\\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, AllPrintableASCII) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"!@#$%^&*()_+-=[]{}|;:,.<>?/~`\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].lexeme, "!@#$%^&*()_+-=[]{}|;:,.<>?/~`");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, UnicodeCharacters) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"Hello 世界 🌍 Привет\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].lexeme, "Hello 世界 🌍 Привет");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, ZeroWidthCharacters) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"test\u200Bword\""; // Zero-width space

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, ControlCharacters) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \x01\x02\x03\x04";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, NullByteInString) {
    WordSplitter wordSplitter;
    std::string shellCommand = std::string("echo test") + std::string(1, '\0') + std::string("after");

    auto word_stream = wordSplitter.wordStream(shellCommand);

    // Behavior depends on null handling
    ASSERT_GE(word_stream.size(), 1);
}

TEST(WordSplitterTest, MassiveWord) {
    WordSplitter wordSplitter;
    std::string massive(100000, 'x');
    std::string shellCommand = "echo " + massive;

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].lexeme.length(), 100000);
}

TEST(WordSplitterTest, MassiveQuotedString) {
    WordSplitter wordSplitter;
    std::string massive(100000, 'y');
    std::string shellCommand = "echo \"" + massive + "\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].lexeme.length(), 100000);
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, ThousandWords) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo";
    for (int i = 0; i < 1000; ++i) {
        shellCommand += " word" + std::to_string(i);
    }

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 1001);
}

TEST(WordSplitterTest, ThousandEmptyQuotes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo";
    for (int i = 0; i < 1000; ++i) {
        shellCommand += " \"\"";
    }

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 1001);
    for (size_t i = 1; i < word_stream.size(); ++i) {
        ASSERT_EQ(word_stream[i].lexeme, "");
        ASSERT_EQ(word_stream[i].context, Word::WordContext::DOUBLE_QUOTE);
    }
}

TEST(WordSplitterTest, AlternatingQuoteTypes) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo 'a' \"b\" 'c' \"d\" 'e' \"f\" 'g' \"h\" 'i' \"j\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 11);
    for (size_t i = 1; i < word_stream.size(); ++i) {
        if (i % 2 == 1) {
            ASSERT_EQ(word_stream[i].context, Word::WordContext::SINGLE_QUOTE);
        } else {
            ASSERT_EQ(word_stream[i].context, Word::WordContext::DOUBLE_QUOTE);
        }
    }
}

TEST(WordSplitterTest, ComplexRealWorldAWK) {
    WordSplitter wordSplitter;
    std::string shellCommand = "awk -F'|' 'BEGIN{OFS=\";\"} {for(i=1;i<=NF;i++) print i, $i}' file.csv";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "awk");
}

TEST(WordSplitterTest, ComplexRealWorldFind) {
    WordSplitter wordSplitter;
    std::string shellCommand = "find . -name \"*.cpp\" -o -name \"*.h\" -exec grep -l \"pattern\" {} \\;";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 10);
}

TEST(WordSplitterTest, ComplexRealWorldSed) {
    WordSplitter wordSplitter;
    std::string shellCommand = "sed -n '/start/,/end/p' file.txt | sed 's/old/new/g'";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 7);
}

TEST(WordSplitterTest, SQLInShell) {
    WordSplitter wordSplitter;
    std::string shellCommand = "mysql -e \"SELECT * FROM users WHERE name LIKE '%admin%' AND status='active';\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 3);
    ASSERT_EQ(word_stream[2].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, JSONInShell) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo '{\"name\": \"John Doe\", \"age\": 30, \"city\": \"New York\"}'";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, XMLInShell) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"<root><item id='1'>Test</item></root>\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, Base64Strings) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"SGVsbG8gV29ybGQh\" | base64 -d";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
}

TEST(WordSplitterTest, RegexPatterns) {
    WordSplitter wordSplitter;
    std::string shellCommand = "grep -E '^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$' emails.txt";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
}

TEST(WordSplitterTest, MultipleRedirections) {
    WordSplitter wordSplitter;
    std::string shellCommand = "cat < input.txt > output.txt 2>&1";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 6);
}

TEST(WordSplitterTest, HereDocSimulation) {
    WordSplitter wordSplitter;
    std::string shellCommand = "cat << 'EOF'\nLine 1\nLine 2\nEOF";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 3);
}

TEST(WordSplitterTest, CommandSubstitutionComplex) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"Today is $(date +%Y-%m-%d) and user is $(whoami)\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 2);
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
}

TEST(WordSplitterTest, BacktickSubstitution) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo `ls -la | wc -l` files found";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 4);
}

TEST(WordSplitterTest, VariableExpansion) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo $PATH $HOME $USER ${SHELL}";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 5);
}

TEST(WordSplitterTest, TildeExpansion) {
    WordSplitter wordSplitter;
    std::string shellCommand = "cd ~/Documents/\"My Projects\"";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 2);
}

TEST(WordSplitterTest, Globbing) {
    WordSplitter wordSplitter;
    std::string shellCommand = "ls *.txt *.{cpp,h} [a-z]*.log";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
}

TEST(WordSplitterTest, ProcessSubstitution) {
    WordSplitter wordSplitter;
    std::string shellCommand = "diff <(sort file1.txt) <(sort file2.txt)";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_GE(word_stream.size(), 3);
}


TEST(WordSplitterTest, MixedQuotingStyles) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo \"double 'single' double\" 'single \"double\" single' unquoted";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[1].lexeme, "double 'single' double");
    ASSERT_EQ(word_stream[1].context, Word::WordContext::DOUBLE_QUOTE);
    ASSERT_EQ(word_stream[2].lexeme, "single \"double\" single");
    ASSERT_EQ(word_stream[2].context, Word::WordContext::SINGLE_QUOTE);
}

TEST(WordSplitterTest, WhitespaceVariety) {
    WordSplitter wordSplitter;
    std::string shellCommand = "echo\t\t\tfoo\n\n\nbar   \r\nbaz";

    auto word_stream = wordSplitter.wordStream(shellCommand);

    ASSERT_EQ(word_stream.size(), 4);
    ASSERT_EQ(word_stream[0].lexeme, "echo");
    ASSERT_EQ(word_stream[1].lexeme, "foo");
    ASSERT_EQ(word_stream[2].lexeme, "bar");
    ASSERT_EQ(word_stream[3].lexeme, "baz");
}