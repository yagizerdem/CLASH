package io.Clash;

import io.Clash.ast.Util;
import io.Clash.parser.core.BashParser;
import org.treesitter.TSNode;
import org.treesitter.TSTree;

public class Main {
    public static void main(String[] args) {
        String source = "$((4 + 5))";

        BashParser parser = new BashParser(source);
        TSTree tree = parser.parseTS();
        TSNode matched = Util.getFirstMatchedTypeDFS(tree.getRootNode(), "arithmetic_expansion");

        System.out.println(matched);

    }
}
