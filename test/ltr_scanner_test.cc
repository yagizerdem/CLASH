//
// Created by pennywise on 28/21/25.
//

#include <gtest/gtest.h>
#include "../src/expand/ltr_scanner.h"

#include "../src/lexer/wordSplitter.h"
#include "../src/parser/parseCommand.h"
#include "../src/util/env.h"
#include "../src/util/model/command.h"

void setDefaultEnv(char* envp[]) {
    Env* env = Env::getInstance();
    env->loadEnvp(envp);
    std::string HOME = env->getEnv("HOME").value;
    env->setCwd(HOME);
}


int main(int argc, char* argv[], char* envp[]) {
    testing::InitGoogleTest(&argc, argv);
    setDefaultEnv(envp);
    return RUN_ALL_TESTS();
}


TEST(LTRSCANNER, Basic) {
    Env* env = Env::getInstance();
    env->setEnv("FULL_NAME", "yagiz erdem");
    env->setEnv("ADDRESS", "izmir");


    LTR_scanner scanner;
    std::string cmd = "echo 'yagiz erdem' $ADDRESS `echo java programmer` \"$FULL_NAME\"";

    std::string expand = scanner.singlePass(cmd);

    ASSERT_EQ(expand, "echo 'yagiz erdem' izmir java programmer \"yagiz erdem\"");

    std::cout << expand << std::endl;

}

TEST(LTRSCANNER, VariableEdgeCases) {
    Env* env = Env::getInstance();
    env->setEnv("A", "1");
    env->setEnv("AB", "12");
    env->setEnv("EMPTY", "");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo $A$AB"),
        "echo 112"
    );

    ASSERT_EQ(
        scanner.singlePass("echo ${A}${AB}"),
        "echo 112"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $UNDEFINED"),
        "echo "
    );

    ASSERT_EQ(
        scanner.singlePass("echo ${EMPTY}X"),
        "echo X"
    );
}

TEST(LTRSCANNER, QuoteAndVariableInteraction) {
    Env* env = Env::getInstance();
    env->setEnv("NAME", "yagiz");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo \"$NAME\""),
        "echo \"yagiz\""
    );

    ASSERT_EQ(
        scanner.singlePass("echo '$NAME'"),
        "echo '$NAME'"
    );

    ASSERT_EQ(
        scanner.singlePass("echo \"hi $NAME\""),
        "echo \"hi yagiz\""
    );
}

TEST(LTRSCANNER, EscapedDollar) {
    Env* env = Env::getInstance();
    env->setEnv("X", "42");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo \\$X"),
        "echo \\$X"
    );

    ASSERT_EQ(
        scanner.singlePass("echo \"\\$X\""),
        "echo \"\\$X\""
    );
}


TEST(LTRSCANNER, CommandSubstitutionTrimNewline) {
    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo `echo test`"),
        "echo test"
    );

    ASSERT_EQ(
        scanner.singlePass("echo `printf \"a\nb\n\"`"),
        "echo a\nb"
    );
}

TEST(LTRSCANNER, MixedExpansionStress) {
    Env* env = Env::getInstance();
    env->setEnv("USER", "pennywise");
    env->setEnv("CITY", "izmir");

    LTR_scanner scanner;

    std::string cmd =
        "echo '$USER' \"$USER lives in $CITY\" `echo ok`";

    ASSERT_EQ(
        scanner.singlePass(cmd),
        "echo '$USER' \"pennywise lives in izmir\" ok"
    );
}

TEST(LTRSCANNER, SyntaxErrors) {
    LTR_scanner scanner;

    ASSERT_THROW(
        scanner.singlePass("echo ${UNFINISHED"),
        std::invalid_argument
    );

    ASSERT_THROW(
        scanner.singlePass("echo `unterminated"),
        std::invalid_argument
    );
}


TEST(LTRSCANNER, NestedCommandSubstitution) {
    Env* env = Env::getInstance();
    env->setEnv("X", "outer");

    LTR_scanner scanner;

    // Nested command substitution
    ASSERT_EQ(
        scanner.singlePass("echo `echo $X`"),
        "echo outer"
    );

    // Multiple command substitutions
    ASSERT_EQ(
        scanner.singlePass("echo `echo a` `echo b`"),
        "echo a b"
    );

    // Quote inside command substitution
    ASSERT_EQ(
        scanner.singlePass("echo `echo \"hello world\"`"),
        "echo hello world"
    );
}

TEST(LTRSCANNER, ComplexVariableNames) {
    Env* env = Env::getInstance();
    env->setEnv("VAR_123", "test");
    env->setEnv("_UNDERSCORE", "value");
    env->setEnv("A1B2C3", "mixed");
    env->setEnv("123", "numeric");  // May be invalid, but for testing

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo $VAR_123"),
        "echo test"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $_UNDERSCORE"),
        "echo value"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $A1B2C3"),
        "echo mixed"
    );

    // Variable starting with a number
    ASSERT_EQ(
        scanner.singlePass("echo $123ABC"),
        "echo 23ABC"  // $1 expands, 23ABC remains literal or whole thing fails
    );
}

TEST(LTRSCANNER, VariableBoundaries) {
    Env* env = Env::getInstance();
    env->setEnv("NAME", "john");
    env->setEnv("NAMETEST", "should_not_match");

    LTR_scanner scanner;

    // Variable name boundaries
    ASSERT_EQ(
        scanner.singlePass("echo $NAME.txt"),
        "echo john.txt"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $NAME-file"),
        "echo john-file"
    );

    ASSERT_EQ(
        scanner.singlePass("echo ${NAME}TEST"),
        "echo johnTEST"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $NAME/path"),
        "echo john/path"
    );

    ASSERT_EQ(
        scanner.singlePass("echo a$NAME"),
        "echo ajohn"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $NAME$NAME"),
        "echo johnjohn"
    );
}

TEST(LTRSCANNER, EmptyAndWhitespace) {
    Env* env = Env::getInstance();
    env->setEnv("SPACE", "a b");
    env->setEnv("TAB", "a\tb");
    env->setEnv("NEWLINE", "a\nb");

    LTR_scanner scanner;

    // Empty string
    ASSERT_EQ(scanner.singlePass(""), "");

    // Whitespace only
    ASSERT_EQ(scanner.singlePass("   "), "");
    ASSERT_EQ(scanner.singlePass("\t\t"), "");

    // Variables containing whitespace
    ASSERT_EQ(
        scanner.singlePass("echo $SPACE"),
        "echo a b"
    );

    ASSERT_EQ(
        scanner.singlePass("echo \"$SPACE\""),
        "echo \"a b\""
    );
}

TEST(LTRSCANNER, SpecialCharactersInVariables) {
    Env* env = Env::getInstance();
    env->setEnv("VAR", "a;b|c&d");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo $VAR"),
        "echo a;b|c&d"
    );

    ASSERT_EQ(
        scanner.singlePass("echo \"$VAR\""),
        "echo \"a;b|c&d\""
    );
}

TEST(LTRSCANNER, QuoteEdgeCases) {
    Env* env = Env::getInstance();
    env->setEnv("X", "value");

    LTR_scanner scanner;

    // Empty quotes
    ASSERT_EQ(
        scanner.singlePass("echo \"\""),
        "echo \"\""
    );

    ASSERT_EQ(
        scanner.singlePass("echo ''"),
        "echo ''"
    );

    // Adjacent quotes
    ASSERT_EQ(
        scanner.singlePass("echo \"\"\"\""),
        "echo \"\"\"\""
    );

    ASSERT_EQ(
        scanner.singlePass("echo ''''"),
        "echo ''''"
    );

    // Mixed quote types
    ASSERT_EQ(
        scanner.singlePass("echo \"'$X'\""),
        "echo \"'value'\""
    );

    ASSERT_EQ(
        scanner.singlePass("echo '\"$X\"'"),
        "echo '\"$X\"'"
    );

    // Backslash inside quotes
    ASSERT_EQ(
        scanner.singlePass("echo \"test\\\"quote\""),
        "echo \"test\\\"quote\""
    );
}

TEST(LTRSCANNER, DollarSignEdgeCases) {
    Env* env = Env::getInstance();
    env->setEnv("VAR", "test");
    env->setEnv("?", "128");


    LTR_scanner scanner;

    // $ at the end
    ASSERT_EQ(
        scanner.singlePass("echo"),
        "echo"
    );

    // Multiple $
    ASSERT_EQ(
        scanner.singlePass("echo "),
        "echo"  // $$ may have special meaning (process ID)
    );

    // $ and whitespace
    ASSERT_EQ(
        scanner.singlePass("echo $ VAR"),
        "echo  VAR"
    );

    // Invalid $ usage
    ASSERT_EQ(
        scanner.singlePass("echo $@#$"),
        "echo @#"  // Special variables or invalid
    );

    // $ and special characters
    ASSERT_EQ(
        scanner.singlePass("echo $!"),
        "echo !"  // $! may be a special variable
    );

    ASSERT_EQ(
        scanner.singlePass("echo $?"),
        "echo 128"  // $? exit status
    );
}

TEST(LTRSCANNER, CommandSubstitutionEdgeCases) {
    LTR_scanner scanner;

    // Empty command substitution
    ASSERT_EQ(
        scanner.singlePass("echo ``"),
        "echo "
    );

    // Whitespace only
    ASSERT_EQ(
        scanner.singlePass("echo `  `"),
        "echo "
    );

    // Multiple newlines
    ASSERT_EQ(
        scanner.singlePass("echo `printf \"a\\n\\n\\n\"`"),
        "echo a"  // Trailing newline is trimmed
    );

    // Backtick inside command (escape required)
    ASSERT_THROW(
        scanner.singlePass("echo `echo \\``"),
        std::invalid_argument
    );
}

TEST(LTRSCANNER, BraceExpansionEdgeCases) {
    Env* env = Env::getInstance();
    env->setEnv("VAR", "test");
    env->setEnv("A_B", "hyphen");

    LTR_scanner scanner;

    // Empty brace
    ASSERT_EQ(
        scanner.singlePass("echo ${}"),
        "echo "
    );

    // Whitespace inside brace
    ASSERT_EQ(
        scanner.singlePass("echo ${ VAR }"),
        "echo "
    );

    // Nested brace (invalid)
    ASSERT_EQ(
        scanner.singlePass("echo ${${VAR}}"),
        "echo }"
    );

    // Special characters inside brace
    ASSERT_EQ(
        scanner.singlePass("echo ${A_B}"),
        "echo hyphen"
    );
}

TEST(LTRSCANNER, MixedQuotesAndExpansion) {
    Env* env = Env::getInstance();
    env->setEnv("A", "1");
    env->setEnv("B", "2");

    LTR_scanner scanner;

    // Complex combinations
    ASSERT_EQ(
        scanner.singlePass("echo \"$A\"'$B'$A"),
        "echo \"1\"'$B'1"
    );

    ASSERT_EQ(
        scanner.singlePass("echo '$A'\"$B\"'$A'"),
        "echo '$A'\"2\"'$A'"
    );

    ASSERT_EQ(
        scanner.singlePass("echo `echo \"$A\"` '$B'"),
        "echo 1 '$B'"
    );

    // Command substitution inside quotes
    ASSERT_EQ(
        scanner.singlePass("echo \"`echo test`\""),
        "echo \"test\""
    );

    // Quote and variable inside command substitution
    ASSERT_EQ(
        scanner.singlePass("echo `echo \"$A\"`"),
        "echo 1"
    );
}

TEST(LTRSCANNER, EscapeSequences) {
    Env* env = Env::getInstance();
    env->setEnv("VAR", "test");

    LTR_scanner scanner;

    // Backslash combinations
    ASSERT_EQ(
        scanner.singlePass("echo \\\\$VAR"),
        "echo \\\\test"  // or "echo \\test" - implementation dependent
    );

    ASSERT_EQ(
        scanner.singlePass("echo \\`echo test\\`"),
        "echo \\`echo test\\`"
    );

    // Escape inside double quotes
    ASSERT_EQ(
        scanner.singlePass("echo \"\\n\\t\""),
        "echo \"\\n\\t\""
    );

    // Backslash inside single quotes (literal)
    ASSERT_EQ(
        scanner.singlePass("echo '\\$VAR'"),
        "echo '\\$VAR'"
    );
}

TEST(LTRSCANNER, LongAndComplexCommands) {
    Env* env = Env::getInstance();
    env->setEnv("USER", "admin");
    env->setEnv("PATH", "/usr/bin");
    env->setEnv("HOME", "/home/admin");

    LTR_scanner scanner;

    // Very long command
    std::string longCmd = "echo $USER $PATH \"$HOME\" `echo $USER` '$PATH' $HOME";
    std::string a =    scanner.singlePass(longCmd);
    ASSERT_EQ(
        scanner.singlePass(longCmd),
        "echo admin /usr/bin \"/home/admin\" admin '$PATH' /home/admin"
    );

    // Many expansions
    ASSERT_EQ(
        scanner.singlePass("echo $USER$USER$USER"),
        "echo adminadminadmin"
    );
}

TEST(LTRSCANNER, UnclosedQuotesAndBraces) {
    LTR_scanner scanner;


    // Unclosed brace
    ASSERT_THROW(
        scanner.singlePass("echo ${VAR"),
        std::invalid_argument
    );

    // Unclosed command substitution
    ASSERT_THROW(
        scanner.singlePass("echo `test"),
        std::invalid_argument
    );


}

TEST(LTRSCANNER, SpecialShellVariables) {
    Env* env = Env::getInstance();
    env->setEnv("0", "script_name");
    env->setEnv("1", "arg1");
    env->setEnv("?", "0");
    env->setEnv("#", "5");

    LTR_scanner scanner;

    // Positional parameters
    ASSERT_EQ(
        scanner.singlePass("echo $0 $1"),
        "echo script_name arg1"
    );

    // Special variables
    ASSERT_EQ(
        scanner.singlePass("echo $? $#"),
        "echo 0 5"
    );
}

TEST(LTRSCANNER, UnicodeAndSpecialChars) {
    Env* env = Env::getInstance();
    env->setEnv("CITY", "İzmir");
    env->setEnv("EMOJI", "🚀");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo $CITY"),
        "echo İzmir"
    );

    ASSERT_EQ(
        scanner.singlePass("echo $EMOJI"),
        "echo 🚀"
    );

    ASSERT_EQ(
        scanner.singlePass("echo \"$CITY $EMOJI\""),
        "echo \"İzmir 🚀\""
    );
}

TEST(LTRSCANNER, CommandSubstitutionWithComplexCommands) {
    LTR_scanner scanner;

    // Contains pipe
    ASSERT_EQ(
        scanner.singlePass("echo `echo test | cat`"),
        "echo test"
    );

    // Contains redirection
    ASSERT_EQ(
        scanner.singlePass("echo `cat < /dev/null`"),
        "echo "
    );

    // Multiple commands
    ASSERT_EQ(
        scanner.singlePass("echo `echo a; echo b`"),
        "echo ab"
    );
}

TEST(LTRSCANNER, RecursiveExpansion) {
    Env* env = Env::getInstance();
    env->setEnv("A", "$B");
    env->setEnv("B", "value");

    LTR_scanner scanner;

    // Recursive expansion (should not happen in single pass)
    ASSERT_EQ(
        scanner.singlePass("echo $A"),
        "echo $B"  // Second $ should not be expanded in single pass
    );
}

TEST(LTRSCANNER, EdgeCasesCombined) {
    Env* env = Env::getInstance();
    env->setEnv("X", "");
    env->setEnv("Y", " ");
    env->setEnv("Z", "  test  ");

    LTR_scanner scanner;

    // Empty variables
    ASSERT_EQ(
        scanner.singlePass("echo a${X}b"),
        "echo ab"
    );

    std::string a  = scanner.singlePass("echo a$Yb");

    ASSERT_EQ(
        scanner.singlePass("echo a${Y}b"),
        "echo a b"
    );

    // Untrimmed values
    ASSERT_EQ(
        scanner.singlePass("echo \"$Z\""),
        "echo \"  test  \""
    );

    // Multiple empty expansions
    ASSERT_EQ(
        scanner.singlePass("echo $X$X$X"),
        "echo "
    );
}

TEST(LTRSCANNER, BacktickAdjacentWordBasic) {
    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo yagiz`echo erdem`"),
        "echo yagizerdem"
    );

    ASSERT_EQ(
        scanner.singlePass("echo `echo yagiz`erdem"),
        "echo yagizerdem"
    );

    ASSERT_EQ(
        scanner.singlePass("echo yagiz`echo erdem`soyad"),
        "echo yagizerdemsoyad"
    );
}
TEST(LTRSCANNER, MultipleBackticksInOneWord) {
    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo a`echo b`c`echo d`e"),
        "echo abcde"
    );
}

TEST(LTRSCANNER, BacktickAdjacentToQuotes) {
    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo \"name:`echo yagiz`\""),
        "echo \"name:yagiz\""
    );

    ASSERT_EQ(
        scanner.singlePass("echo 'name:`echo yagiz`'"),
        "echo 'name:`echo yagiz`'"
    );
}

TEST(LTRSCANNER, BacktickAdjacentSyntaxErrors) {
    LTR_scanner scanner;

    // Unclosed backtick inside word
    ASSERT_THROW(
        scanner.singlePass("echo yagiz`echo erdem"),
        std::invalid_argument
    );

    // Escaped backtick should stay literal
    ASSERT_EQ(
        scanner.singlePass("echo yagiz\\`echo erdem\\`"),
        "echo yagiz\\`echo erdem\\`"
    );
}
