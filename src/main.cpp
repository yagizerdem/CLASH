#include <cassert>
#include <cstdio>
#include <string>
#include <cpp-tree-sitter.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include "util.h"

extern "C" {
TSLanguage* tree_sitter_bash(void);
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