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