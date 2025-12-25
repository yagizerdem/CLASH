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