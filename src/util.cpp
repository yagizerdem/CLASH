#include <cassert>
#include <cstdio>
#include <string>
#include <cpp-tree-sitter.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <tree_sitter/api.h>
#include "util.h"


void traverse(TSNode root, int depth) {
    std::cout
        << std::string(depth * 2, ' ')
        << ts_node_type(root)
        << '\n';

    TSTreeCursor cursor = ts_tree_cursor_new(root);

    if (ts_tree_cursor_goto_first_child(&cursor)) {
        do {
            TSNode child =
                ts_tree_cursor_current_node(&cursor);
            traverse(child, depth + 1);

        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }

    ts_tree_cursor_delete(&cursor);
}