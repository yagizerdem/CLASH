//
// Created by pennywise on 12/16/25.
//

#include <gtest/gtest.h>
#include "../src/expand/tilde.h"

TEST(ExpandTest, TildeBasic) {
    Tilde tilde;

    std::string expandPath = tilde.expandTilde("~testUser/dev");
    ASSERT_EQ("~testUser/dev", expandPath);

    expandPath = tilde.expandTilde("~pennywise/dev");
    ASSERT_EQ("/home/pennywise/dev", expandPath);

    expandPath = tilde.expandTilde("echo ~pennywise/dev");
    ASSERT_EQ("echo /home/pennywise/dev", expandPath);

    expandPath = tilde.expandTilde("echo \"~pennywise/dev\"");
    ASSERT_EQ("echo \"~pennywise/dev\"", expandPath);

    expandPath = tilde.expandTilde("echo \"~pennywise/dev\" ~dev");
    ASSERT_EQ("echo \"~pennywise/dev\" ~dev", expandPath);

    expandPath = tilde.expandTilde("echo ~pennywise/dev ~dev");
    ASSERT_EQ("echo /home/pennywise/dev ~dev", expandPath);

    expandPath = tilde.expandTilde("echo ~pennywise/dev ~/dev");
    ASSERT_EQ("echo /home/pennywise/dev /home/pennywise/dev", expandPath);


    expandPath = tilde.expandTilde("echo \'~pennywise/dev\'");
    ASSERT_EQ("echo \'~pennywise/dev\'", expandPath);

    expandPath = tilde.expandTilde("echo \'~pennywise/dev\' ~dev");
    ASSERT_EQ("echo \'~pennywise/dev\' ~dev", expandPath);
}

TEST(ExpandTest, TildeWordBoundary) {
    Tilde tilde;

    ASSERT_EQ("echo a~pennywise",
              tilde.expandTilde("echo a~pennywise"));

    ASSERT_EQ("echo test~/dev",
              tilde.expandTilde("echo test~/dev"));

    ASSERT_EQ("echo /home/pennywise/devX",
              tilde.expandTilde("echo ~/devX"));

    ASSERT_EQ("echo /home/pennywise/dev_extra",
              tilde.expandTilde("echo ~/dev_extra"));
}

TEST(ExpandTest, TildeWhitespaceSensitive) {
    Tilde tilde;

    ASSERT_EQ("echo   /home/pennywise",
              tilde.expandTilde("echo   ~pennywise"));

    ASSERT_EQ("echo\t/home/pennywise",
              tilde.expandTilde("echo\t~pennywise"));

    ASSERT_EQ("echo \t /home/pennywise/dev",
              tilde.expandTilde("echo \t ~/dev"));
}


TEST(ExpandTest, TildeWithRedirection) {
    Tilde tilde;

    ASSERT_EQ("echo hi > /home/pennywise/out.txt",
              tilde.expandTilde("echo hi > ~/out.txt"));

    ASSERT_EQ("echo hi >> /home/pennywise/out.txt",
              tilde.expandTilde("echo hi >> ~/out.txt"));

    ASSERT_EQ("cat < /home/pennywise/input.txt",
              tilde.expandTilde("cat < ~/input.txt"));

    ASSERT_EQ("echo hi > file~",
              tilde.expandTilde("echo hi > file~"));
}



TEST(ExpandTest, TildeEdgeCases) {
    Tilde tilde;


    // Only tilde
    ASSERT_EQ("/home/pennywise",
              tilde.expandTilde("~"));

    ASSERT_EQ("/home/pennywise/",
              tilde.expandTilde("~/"));

    // Tilde not at the beginning of a word
    ASSERT_EQ("echo test~",
              tilde.expandTilde("echo test~"));

    ASSERT_EQ("prefix~pennywise/dev",
              tilde.expandTilde("prefix~pennywise/dev"));

    // Double tilde
    ASSERT_EQ("/home/pennywise /home/pennywise",
              tilde.expandTilde("~ ~"));

    ASSERT_EQ("/home/pennywise/dev /home/pennywise/doc",
              tilde.expandTilde("~/dev ~/doc"));

    // Tilde with special characters
    ASSERT_EQ("echo /home/pennywise/file.txt",
              tilde.expandTilde("echo ~/file.txt"));

    ASSERT_EQ("echo /home/pennywise/dir-name",
              tilde.expandTilde("echo ~/dir-name"));

    ASSERT_EQ("echo /home/pennywise/dir.name",
              tilde.expandTilde("echo ~/dir.name"));

    // Escape characters
    ASSERT_EQ("echo \\~/dev",
              tilde.expandTilde("echo \\~/dev"));

    const auto & a =  tilde.expandTilde("echo ~pennywise;");

    // Trailing delimiter characters
    ASSERT_EQ("echo /home/pennywise",
              tilde.expandTilde("echo ~pennywise"));

    ASSERT_EQ("echo /home/pennywise |",
              tilde.expandTilde("echo ~pennywise |"));

    ASSERT_EQ("echo /home/pennywise &",
              tilde.expandTilde("echo ~pennywise &"));

    ASSERT_EQ("echo /home/pennywise &&",
              tilde.expandTilde("echo ~pennywise &&"));

    ASSERT_EQ("echo /home/pennywise ||",
              tilde.expandTilde("echo ~pennywise ||"));
}

TEST(ExpandTest, TildeEmptyAndSpecial) {
    Tilde tilde;

    // Empty string
    ASSERT_EQ("", tilde.expandTilde(""));

    // Whitespace only
    ASSERT_EQ("   ", tilde.expandTilde("   "));

    ASSERT_EQ("\t\t", tilde.expandTilde("\t\t"));

    // Contains newline
    ASSERT_EQ("echo /home/pennywise\n",
              tilde.expandTilde("echo ~pennywise\n"));

    // Mixed whitespace types
    ASSERT_EQ("echo  \t /home/pennywise",
              tilde.expandTilde("echo  \t ~pennywise"));
}

TEST(ExpandTest, TildeQuoteMixing) {
    Tilde tilde;

    // Mixed quotes
    ASSERT_EQ("echo \"test\" /home/pennywise",
              tilde.expandTilde("echo \"test\" ~pennywise"));

    ASSERT_EQ("echo 'test' /home/pennywise",
              tilde.expandTilde("echo 'test' ~pennywise"));

    // Nested quotes
    ASSERT_EQ("echo \"'~/dev'\"",
              tilde.expandTilde("echo \"'~/dev'\""));

    ASSERT_EQ("echo '\"~/dev\"'",
              tilde.expandTilde("echo '\"~/dev\"'"));

    // Unterminated quotes
    ASSERT_EQ("echo \"~/dev",
              tilde.expandTilde("echo \"~/dev"));

    ASSERT_EQ("echo '~/dev",
              tilde.expandTilde("echo '~/dev"));

    // Tilde immediately after a quoted section
    ASSERT_EQ("echo \"test\"~pennywise",
              tilde.expandTilde("echo \"test\"~pennywise"));
}

TEST(ExpandTest, TildeComplexPaths) {
    Tilde tilde;

    // Multiple slashes
    ASSERT_EQ("/home/pennywise//dev",
              tilde.expandTilde("~//dev"));

    ASSERT_EQ("/home/pennywise///dev",
              tilde.expandTilde("~///dev"));

    // Relative path segments
    ASSERT_EQ("/home/pennywise/../dev",
              tilde.expandTilde("~/../dev"));

    ASSERT_EQ("/home/pennywise/./dev",
              tilde.expandTilde("~/./dev"));

    ASSERT_EQ("/home/pennywise/dev/../doc",
              tilde.expandTilde("~/dev/../doc"));

    // Long path
    ASSERT_EQ("/home/pennywise/very/long/path/to/directory",
              tilde.expandTilde("~/very/long/path/to/directory"));
}

TEST(ExpandTest, TildeWithVariables) {
    Tilde tilde;

    // Environment-variable-like tokens
    ASSERT_EQ("echo $HOME /home/pennywise",
              tilde.expandTilde("echo $HOME ~pennywise"));

    ASSERT_EQ("echo /home/pennywise $USER",
              tilde.expandTilde("echo ~pennywise $USER"));

    // Tilde combined with variable-like pattern
    ASSERT_EQ("echo /home/pennywise $VAR",
              tilde.expandTilde("echo ~pennywise $VAR"));
}

TEST(ExpandTest, TildeNonExistentUser) {
    Tilde tilde;

    // Non-existent user
    ASSERT_EQ("~nonexistentuser123/dev",
              tilde.expandTilde("~nonexistentuser123/dev"));

    ASSERT_EQ("~nosuchuser",
              tilde.expandTilde("~nosuchuser"));

    // Username containing invalid characters
    ASSERT_EQ("~user@invalid/dev",
              tilde.expandTilde("~user@invalid/dev"));

    ASSERT_EQ("~user#invalid/dev",
              tilde.expandTilde("~user#invalid/dev"));
}

TEST(ExpandTest, TildeMultipleInCommand) {
    Tilde tilde;

    // Multiple tildes in the same command
    ASSERT_EQ("cp /home/pennywise/a /home/pennywise/b",
              tilde.expandTilde("cp ~/a ~/b"));

    ASSERT_EQ("diff /home/pennywise/f1 /home/pennywise/f2",
              tilde.expandTilde("diff ~/f1 ~/f2"));

    // Different users
    ASSERT_EQ("cp /home/pennywise/a ~otheruser/b",
              tilde.expandTilde("cp ~/a ~otheruser/b"));

    const auto &a = tilde.expandTilde("echo ~ \"~/no\" ~/");

    // Mixed expansion and quoting
    ASSERT_EQ("echo /home/pennywise \"~/no\" /home/pennywise/",
              tilde.expandTilde("echo ~ \"~/no\" ~/"));
}


