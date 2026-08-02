package io.Clash;

import org.treesitter.TSParser;
import org.treesitter.TSTree;
import org.treesitter.TreeSitterBash;
public class Main {
    public static void main(String[] args) {
        String source = "name=\"World\"\necho \"Hello, $name\"\n";

        TSParser parser = new TSParser();
        if (!parser.setLanguage(new TreeSitterBash())) {
            throw new IllegalStateException("Tree-sitter Bash grammar could not be loaded");
        }

        TSTree tree = parser.parseString(null, source);
        System.out.println("Tree-sitter Bash AST:");
        System.out.println(tree.getRootNode());
    }
}
