package io.Clash;

import io.Clash.ast.Util;
import io.Clash.ast.base.AstNode;
import io.Clash.parser.core.BashParser;
import org.treesitter.TSNode;
import org.treesitter.TSTree;

public class Main {
    public static void main(String[] args) {
        try {
            String source = """
                    echo hit <> output.txt
                    """;

            BashParser parser = new BashParser(source);
            TSTree tree = parser.parseTS();
            TSNode tsRootNode = tree.getRootNode();
            System.out.println(tsRootNode);



           // TSNode matched = Util.getFirstMatchedTypeDFS(tree.getRootNode(), "expansion");

           AstNode node = parser.parse(tsRootNode);

           var a = 10;

           AstPrinter printer =  new AstPrinter();
           String  ser = node.visit(printer);
           System.out.println(ser);


        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }

    }
}
