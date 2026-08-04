package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.TestCommandNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cTestCommandParser extends cBaseParser {

    public cTestCommandParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "test_command");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        AstNode expression = tsNode.getNamedChildCount() == 0
                ? null
                : this.parseChild(tsNode.getNamedChild(0));
        return new TestCommandNode(syntaxInfo, expression);
    }
}
