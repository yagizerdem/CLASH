package io.Clash;

import io.Clash.ast.Util;
import io.Clash.ast.base.AstNode;
import io.Clash.ast.enums.RedirectionKind;
import io.Clash.parser.core.BashParser;
import io.Clash.process.jvm.SimpleCommandExecutor;
import io.Clash.process.model.RedirectOperator;
import io.Clash.process.model.Redirection;
import io.Clash.process.model.SimpleCommand;
import io.Clash.process.model.base.ExecutionResponse;
import org.treesitter.TSNode;
import org.treesitter.TSTree;

import java.util.List;

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

            ShellContext context = new ShellContext();
            cEnv env = new cEnv();
            String cwd = System.getProperty("user.dir");
            context.env = env;
            context.cwd = cwd;

            SimpleCommand cmd = new SimpleCommand(List.of("tasklist"), List.of(
                    new Redirection(0, RedirectOperator.OUTPUT, "test.txt"),
                    new Redirection(0, RedirectOperator.OUTPUT, "test2.txt")));

            SimpleCommandExecutor executor = new SimpleCommandExecutor(cmd, context);
            ExecutionResponse response = executor.execCaptured();

            System.out.println(response.getStdout());

        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }

    }
}
