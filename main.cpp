#include <cassert>
#include <cstdio>
#include <string>
#include <cpp-tree-sitter.h>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <tree_sitter/api.h>

extern "C" {
TSLanguage* tree_sitter_bash(void);
}

void traverse(TSNode root, int depth = 0) {
    std::cout
        << std::string(depth * 2, ' ')
        << ts_node_type(root)
        << '\n';

    TSTreeCursor cursor = ts_tree_cursor_new(root);

    if (ts_tree_cursor_goto_first_child(&cursor)) {
        do {
            TSNode child =
                ts_tree_cursor_current_node(&cursor);
            if (ts_node_start_byte())

            traverse(child, depth + 1);

        } while (ts_tree_cursor_goto_next_sibling(&cursor));
    }

    ts_tree_cursor_delete(&cursor);
}

int main() {

    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_bash());

    std::string source_code = R"(
#!/usr/bin/env bash

name="Yağız"
count=3

echo "Hello, $name"

if [[ $count -gt 0 ]]; then
    for ((i=1; i<=count; i++)); do
        echo "Iteration $i"
    done
else
    echo "Nothing to do"
fi

files=("main.c" "util.c" "test.c")

for file in "${files[@]}"; do
    if [[ -f "$file" ]]; then
        echo "Found: $file"
    else
        echo "Missing: $file"
    fi
done

result=$(date +"%Y-%m-%d")
echo "Today: $result"
)";

    TSTree *tree = ts_parser_parse_string(
      parser,
      NULL,
      source_code.data(),
      source_code.size()
    );

    TSNode root_node = ts_tree_root_node(tree);

    traverse(root_node);

    return 0;
}