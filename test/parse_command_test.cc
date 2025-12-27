//
// Created by pennywise on 12/27/25.
//

#include <gtest/gtest.h>

#include "../src/parser/parseCommand.h"
#include "../src/util/stringUtil.h"

TEST(ParseCommandTest, Basic) {
    ParseCommand pc;
    Command cmd;

    Word w1;
    w1.lexeme = "echo";
    w1.context = Word::UN_QUOTE;
    cmd.wordStream.push_back(w1);

    Word w2;
    w2.lexeme = "java";
    w2.context = Word::UN_QUOTE;
    cmd.wordStream.push_back(w2);

    Command parsedCmd = pc.parse(cmd);
    ASSERT_EQ(parsedCmd.argv.size(), 2);
    ASSERT_EQ(parsedCmd.argv[0], "echo");
    ASSERT_EQ(parsedCmd.argv[1], "java");
    ASSERT_EQ(parsedCmd.redirectStandartInput, "");
    ASSERT_EQ(parsedCmd.redirectStandartOutput, "");
    ASSERT_EQ(parsedCmd.identifier, "");
    ASSERT_EQ(parsedCmd.value, "");
}

TEST(ParseCommandTest, CdBasic) {
    ParseCommand pc;
    Command cmd;

    Word w1{"cd", Word::UN_QUOTE};
    Word w2{"/tmp", Word::UN_QUOTE};

    cmd.wordStream = {w1, w2};

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.argv.size(), 2); // cd, /tmp
    ASSERT_EQ(parsed.argv[0], "cd");
    ASSERT_EQ(parsed.argv[1], "/tmp");

    ASSERT_EQ(parsed.redirectStandartInput, "");
    ASSERT_EQ(parsed.redirectStandartOutput, "");
}

TEST(ParseCommandTest, CdNoArgument) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cd", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.argv.size(), 1);
    ASSERT_EQ(parsed.argv[0], "cd");
}

TEST(ParseCommandTest, CdTooManyArgs) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cd", Word::UN_QUOTE},
            {"a", Word::UN_QUOTE},
            {"b", Word::UN_QUOTE}
    };

    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandTest, ExternalEchoStdoutRedirection) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"echo", Word::UN_QUOTE},
            {"hi", Word::UN_QUOTE},
            {">", Word::UN_QUOTE},
            {"out.txt", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);

    ASSERT_EQ(parsed.argv.size(), 2);
    ASSERT_EQ(parsed.argv[0], "echo");
    ASSERT_EQ(parsed.argv[1], "hi");

    ASSERT_EQ(parsed.redirectStandartOutput, "out.txt");
}

TEST(ParseCommandTest, ExternalEchoBasic) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"echo", Word::UN_QUOTE},
            {"java", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);

    ASSERT_EQ(parsed.argv.size(), 2); // echo, java
    ASSERT_EQ(parsed.argv[0], "echo");
    ASSERT_EQ(parsed.argv[1], "java");

    ASSERT_EQ(parsed.redirectStandartInput, "");
    ASSERT_EQ(parsed.redirectStandartOutput, "");
}

TEST(ParseCommandTest, ExternalLastStdoutWins) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"echo", Word::UN_QUOTE},
            {"hi", Word::UN_QUOTE},
            {">", Word::UN_QUOTE},
            {"a.txt", Word::UN_QUOTE},
            {">", Word::UN_QUOTE},
            {"b.txt", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);
    ASSERT_EQ(parsed.redirectStandartOutput, "b.txt");
}

TEST(ParseCommandTest, BuiltinCdBasic) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cd", Word::UN_QUOTE},
            {"/tmp", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::CD);

    ASSERT_EQ(parsed.argv.size(), 2);
    ASSERT_EQ(parsed.argv[0], "cd");
    ASSERT_EQ(parsed.argv[1], "/tmp");

    ASSERT_EQ(parsed.redirectStandartInput, "");
    ASSERT_EQ(parsed.redirectStandartOutput, "");
}

TEST(ParseCommandTest, BuiltinCdNoArgument) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cd", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::CD);
    ASSERT_EQ(parsed.argv.size(), 1);
    ASSERT_EQ(parsed.argv[0], "cd");
}

TEST(ParseCommandTest, BuiltinCdTooManyArgs) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cd", Word::UN_QUOTE},
            {"a", Word::UN_QUOTE},
            {"b", Word::UN_QUOTE}
    };

    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandTest, BuiltinExitWithStatus) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"exit", Word::UN_QUOTE},
            {"1", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::EXIT);
    ASSERT_EQ(parsed.argv.size(), 2);
    ASSERT_EQ(parsed.argv[1], "1");
}

TEST(ParseCommandTest, AssignmentBasic) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"x=5", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);

    ASSERT_EQ(parsed.argv.size(), 1);
    ASSERT_EQ(parsed.argv[0], "x=5");

    ASSERT_EQ(parsed.identifier, "x");
    ASSERT_EQ(parsed.value, "5");
}

TEST(ParseCommandTest, AssignmentWithStdoutRedirection) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"x=5", Word::UN_QUOTE},
            {">", Word::UN_QUOTE},
            {"out.txt", Word::UN_QUOTE}
    };

    Command parsed = pc.parse(cmd);

    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);
    ASSERT_EQ(parsed.identifier, "x");
    ASSERT_EQ(parsed.value, "5");
    ASSERT_EQ(parsed.redirectStandartOutput, "out.txt");

    ASSERT_EQ(parsed.argv.size(), 1); // still only x=5
}

TEST(ParseCommandTest, AssignmentInvalidIdentifier) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"1x=5", Word::UN_QUOTE}
    };

    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandTest, ExternalSyntaxErrorMissingStdinTarget) {
    ParseCommand pc;
    Command cmd;

    cmd.wordStream = {
            {"cat", Word::UN_QUOTE},
            {"<", Word::UN_QUOTE}
    };

    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, EmptyWordStream) {
    ParseCommand pc;
    Command cmd;
    // Empty wordStream should return EMPTY type
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EMPTY);
}

TEST(ParseCommandEdgeCaseTest, SingleQuotedCd) {
    ParseCommand pc;
    Command cmd;
    // 'cd' in quotes should be treated as executable, not builtin
    cmd.wordStream = {{"cd", Word::SINGLE_QUOTE}, {"/tmp", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::CD);
}

TEST(ParseCommandEdgeCaseTest, DoubleQuotedExit) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"exit", Word::DOUBLE_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EXIT);
}

TEST(ParseCommandEdgeCaseTest, QuotedAssignmentNotRecognized) {
    ParseCommand pc;
    Command cmd;
    // x=5 in quotes should be treated as argument, not assignment
    cmd.wordStream = {{"x=5", Word::SINGLE_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);
}

TEST(ParseCommandEdgeCaseTest, AssignmentStartingWithNumber) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"1x=value", Word::UN_QUOTE}};
    // isValidVarName checks: first char must be alpha
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithUnderscore) {
    ParseCommand pc;
    Command cmd;
    // Underscore is NOT alpha, so should fail
    cmd.wordStream = {{"_var=5", Word::UN_QUOTE}};
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithHyphen) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"my-var=5", Word::UN_QUOTE}};
    // Hyphen is not alphanumeric
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithNumberInMiddle) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"var123=value", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);
    ASSERT_EQ(parsed.identifier, "var123");
    ASSERT_EQ(parsed.value, "value");
}

TEST(ParseCommandEdgeCaseTest, AssignmentEmptyIdentifier) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"=value", Word::UN_QUOTE}};
    // Empty identifier fails isValidVarName
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, AssignmentNoEquals) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"justtext", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    // No '=' found, should be executable
    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);
}

TEST(ParseCommandEdgeCaseTest, AssignmentMultipleEquals) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"PATH=/usr/bin:/home/user", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);
    ASSERT_EQ(parsed.identifier, "PATH");
    ASSERT_EQ(parsed.value, "/usr/bin:/home/user");
}

TEST(ParseCommandEdgeCaseTest, AssignmentEqualsAtEnd) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"VAR=", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);
    ASSERT_EQ(parsed.identifier, "VAR");
    ASSERT_EQ(parsed.value, "");
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithSpecialCharsInValue) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"URL=https://example.com?key=value&foo=bar", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.identifier, "URL");
    ASSERT_EQ(parsed.value, "https://example.com?key=value&foo=bar");
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithRedirection) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"x=5", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::ASSIGNMENT);
    ASSERT_EQ(parsed.identifier, "x");
    ASSERT_EQ(parsed.value, "5");
    ASSERT_EQ(parsed.redirectStandartOutput, "out.txt");
    ASSERT_EQ(parsed.argv.size(), 1); // Only x=5
}

TEST(ParseCommandEdgeCaseTest, AssignmentMustHaveExactlyOneArgv) {
    ParseCommand pc;
    Command cmd;
    // If argv.size() != 1 for assignment, logic_error thrown
    // This tests internal consistency check
    cmd.wordStream = {{"VAR=value", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.argv.size(), 1);
}

// ============================================================================
// CD EDGE CASES
// ============================================================================

TEST(ParseCommandEdgeCaseTest, CdTooManyArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cd", Word::UN_QUOTE},
        {"/tmp", Word::UN_QUOTE},
        {"/home", Word::UN_QUOTE}
    };
    // cd with argv.size() > 2 throws
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, CdWithRedirectionStillChecksArgCount) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cd", Word::UN_QUOTE},
        {"/tmp", Word::UN_QUOTE},
        {"/home", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    // Redirections are removed from argv, so argv = [cd, /tmp, /home]
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, CdWithOnlyRedirection) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cd", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // argv = [cd], size = 1, valid
    ASSERT_EQ(parsed.argv.size(), 1);
    ASSERT_EQ(parsed.redirectStandartOutput, "out.txt");
}

TEST(ParseCommandEdgeCaseTest, CdNoArgument) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"cd", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::CD);
    ASSERT_EQ(parsed.argv.size(), 1);
}

// ============================================================================
// EXIT EDGE CASES
// ============================================================================

TEST(ParseCommandEdgeCaseTest, ExitTooManyArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"exit", Word::UN_QUOTE},
        {"1", Word::UN_QUOTE},
        {"2", Word::UN_QUOTE}
    };
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, ExitWithRedirection) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"exit", Word::UN_QUOTE},
        {"1", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // Redirections removed from argv, so argv = [exit, 1], size = 2, valid
    ASSERT_EQ(parsed.argv.size(), 2);
    ASSERT_EQ(parsed.redirectStandartOutput, "out.txt");
}

TEST(ParseCommandEdgeCaseTest, ExitWithMultipleRedirections) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"exit", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"a.txt", Word::UN_QUOTE},
        {"0", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"b.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // argv = [exit, 0], size = 2
    ASSERT_EQ(parsed.argv.size(), 2);
    ASSERT_EQ(parsed.redirectStandartOutput, "b.txt");
}

// ============================================================================
// EXPORT EDGE CASES
// ============================================================================

TEST(ParseCommandEdgeCaseTest, ExportValidIdentifiers) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"export", Word::UN_QUOTE},
        {"PATH", Word::UN_QUOTE},
        {"HOME", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EXPORT);
}

TEST(ParseCommandEdgeCaseTest, ExportInvalidIdentifier) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"export", Word::UN_QUOTE},
        {"1VAR", Word::UN_QUOTE}
    };
    // isValidVarName fails for 1VAR
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, ExportWithUnderscore) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"export", Word::UN_QUOTE},
        {"_VAR", Word::UN_QUOTE}
    };
    // Underscore not alpha, fails
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, ExportWithHyphen) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"export", Word::UN_QUOTE},
        {"MY-VAR", Word::UN_QUOTE}
    };
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, ExportNoArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"export", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    // No identifiers to check, should pass
    ASSERT_EQ(parsed.commandType, Command::EXPORT);
}

// ============================================================================
// UNSET EDGE CASES
// ============================================================================

TEST(ParseCommandEdgeCaseTest, UnsetValidIdentifiers) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"unset", Word::UN_QUOTE},
        {"VAR1", Word::UN_QUOTE},
        {"VAR2", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::UNSET);
}

TEST(ParseCommandEdgeCaseTest, UnsetInvalidIdentifier) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"unset", Word::UN_QUOTE},
        {"9VAR", Word::UN_QUOTE}
    };
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, UnsetMixedValidInvalid) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"unset", Word::UN_QUOTE},
        {"VALID", Word::UN_QUOTE},
        {"_invalid", Word::UN_QUOTE}
    };
    // Second identifier fails
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, UnsetNoArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"unset", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::UNSET);
}

// ============================================================================
// REDIRECTION EDGE CASES (classifyStdInput/Output logic)
// ============================================================================

TEST(ParseCommandEdgeCaseTest, InputRedirectionMissingTarget) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cat", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE}
    };
    // i + 1 >= size, throws syntax error
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, OutputRedirectionMissingTarget) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"echo", Word::UN_QUOTE},
        {">", Word::UN_QUOTE}
    };
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, RedirectionFollowedByRedirection) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cat", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {">", Word::UN_QUOTE}
    };
    // next.lexeme == ">" triggers error
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, RedirectionFollowedByPipe) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cat", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"|", Word::UN_QUOTE}
    };
    // Pipe after redirection is syntax error
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, RedirectionFollowedBySemicolon) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"echo", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {";", Word::UN_QUOTE}
    };
    ASSERT_THROW(pc.parse(cmd), std::invalid_argument);
}

TEST(ParseCommandEdgeCaseTest, MultipleInputRedirectionsLastWins) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cat", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {"file1.txt", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {"file2.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // Last assignment wins in classifyStdInput
    ASSERT_EQ(parsed.redirectStandartInput, "file2.txt");
}

TEST(ParseCommandEdgeCaseTest, MultipleOutputRedirectionsLastWins) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"echo", Word::UN_QUOTE},
        {"test", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out1.txt", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out2.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.redirectStandartOutput, "out2.txt");
}

TEST(ParseCommandEdgeCaseTest, QuotedRedirectionNotRecognized) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"echo", Word::UN_QUOTE},
        {"test", Word::UN_QUOTE},
        {">", Word::SINGLE_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // context != UN_QUOTE, so not recognized as redirection
    ASSERT_EQ(parsed.redirectStandartOutput, "");
    ASSERT_EQ(parsed.argv.size(), 4); // echo, test, >, out.txt
}

TEST(ParseCommandEdgeCaseTest, MixedInputOutputRedirections) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"sort", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {"input.txt", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"output.txt", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.redirectStandartInput, "input.txt");
    ASSERT_EQ(parsed.redirectStandartOutput, "output.txt");
    ASSERT_EQ(parsed.argv.size(), 1); // sort only
}

TEST(ParseCommandEdgeCaseTest, RedirectionInMiddleOfArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"echo", Word::UN_QUOTE},
        {"arg1", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE},
        {"arg2", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // Redirections removed, argv = echo, arg1, arg2
    ASSERT_EQ(parsed.argv.size(), 3);
    ASSERT_EQ(parsed.argv[1], "arg1");
    ASSERT_EQ(parsed.argv[2], "arg2");
}

// ============================================================================
// ARGV CLASSIFICATION EDGE CASES
// ============================================================================

TEST(ParseCommandEdgeCaseTest, ExecutableCommandNoNullTerminator) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"ls", Word::UN_QUOTE},
        {"-la", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);
    ASSERT_EQ(parsed.argv.size(), 2); // ls, -la (no nullptr)
}

TEST(ParseCommandEdgeCaseTest, BuiltinCommandNoNullTerminator) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cd", Word::UN_QUOTE},
        {"/tmp", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::CD);
    ASSERT_EQ(parsed.argv.size(), 2); // cd, /tmp (no nullptr)
}

TEST(ParseCommandEdgeCaseTest, AllArgumentsAreRedirections) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"<", Word::UN_QUOTE},
        {"in.txt", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"out.txt", Word::UN_QUOTE}
    };
    // No command before redirections
    Command parsed = pc.parse(cmd);
    // All items are skipped in classifyArgv, argv is empty
    ASSERT_TRUE(parsed.argv.empty());
}

TEST(ParseCommandEdgeCaseTest, RedirectionSkipsNextWord) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"cat", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {"input.txt", Word::UN_QUOTE},
        {"extra", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    // After <, i++ skips input.txt in classifyArgv
    ASSERT_EQ(parsed.argv.size(), 2); // cat, extra
    ASSERT_EQ(parsed.argv[0], "cat");
    ASSERT_EQ(parsed.argv[1], "extra");
}

// ============================================================================
// COMPLEX REAL-WORLD SCENARIOS
// ============================================================================

TEST(ParseCommandEdgeCaseTest, ComplexAssignmentWithPath) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"PATH=/usr/local/bin:/usr/bin:/bin", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.identifier, "PATH");
    ASSERT_EQ(parsed.value, "/usr/local/bin:/usr/bin:/bin");
}

TEST(ParseCommandEdgeCaseTest, CommandWithManyArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream.push_back({"gcc", Word::UN_QUOTE});
    for (int i = 0; i < 100; i++) {
        cmd.wordStream.push_back({"file" + std::to_string(i) + ".c", Word::UN_QUOTE});
    }
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.argv.size(), 101); // gcc + 100 files (no nullptr)
}

TEST(ParseCommandEdgeCaseTest, InterleavedRedirectionsAndArguments) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {
        {"sort", Word::UN_QUOTE},
        {"<", Word::UN_QUOTE},
        {"data.txt", Word::UN_QUOTE},
        {"-n", Word::UN_QUOTE},
        {">", Word::UN_QUOTE},
        {"sorted.txt", Word::UN_QUOTE},
        {"-r", Word::UN_QUOTE}
    };
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.redirectStandartInput, "data.txt");
    ASSERT_EQ(parsed.redirectStandartOutput, "sorted.txt");
    // argv = sort, -n, -r
    ASSERT_EQ(parsed.argv.size(), 3);
}

TEST(ParseCommandEdgeCaseTest, CaseInsensitiveVariableNames) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"MyVar=value", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    // isalpha/isalnum work with both cases
    ASSERT_EQ(parsed.identifier, "MyVar");
}

TEST(ParseCommandEdgeCaseTest, AssignmentWithSpacesInQuotedValue) {
    ParseCommand pc;
    Command cmd;
    // Note: In your tokenizer this would be one word
    cmd.wordStream = {{"MSG=hello world", Word::DOUBLE_QUOTE}};
    Command parsed = pc.parse(cmd);
    // Quoted, so not recognized as assignment
    ASSERT_EQ(parsed.commandType, Command::EXECUTABLE_COMMAND);
}

TEST(ParseCommandEdgeCaseTest, ValidVarNameAllUppercase) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"HOME=/home/user", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.identifier, "HOME");
    ASSERT_EQ(parsed.value, "/home/user");
}

TEST(ParseCommandEdgeCaseTest, ValidVarNameMixedCase) {
    ParseCommand pc;
    Command cmd;
    cmd.wordStream = {{"myVar123=test", Word::UN_QUOTE}};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.identifier, "myVar123");
    ASSERT_EQ(parsed.value, "test");
}

TEST(ParseCommandEdgeCaseTest, EmptyCommandTypeWithContent) {
    ParseCommand pc;
    Command cmd;
    // Test what happens with truly empty
    cmd.rawShellCommand = "";
    cmd.wordStream = {};
    Command parsed = pc.parse(cmd);
    ASSERT_EQ(parsed.commandType, Command::EMPTY);
}