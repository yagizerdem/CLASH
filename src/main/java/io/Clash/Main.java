package io.Clash;

import io.Clash.ast.Util;
import io.Clash.ast.base.AstNode;
import io.Clash.parser.core.BashParser;
import org.treesitter.TSNode;
import org.treesitter.TSTree;

public class Main {
    public static void main(String[] args) {
        try {
            String source = "${FILE/foo_${OLD}/bar_${NEW}}";

            BashParser parser = new BashParser(source);
            TSTree tree = parser.parseTS();
            System.out.println(tree.getRootNode());

           TSNode matched = Util.getFirstMatchedTypeDFS(tree.getRootNode(), "expansion");
//
           AstNode node = parser.parse(matched);


        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }

    }
}
