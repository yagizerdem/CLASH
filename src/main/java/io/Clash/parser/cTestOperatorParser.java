package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.TestOperatorNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cTestOperatorParser extends cBaseParser {
    public cTestOperatorParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "test_operator");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new TestOperatorNode(syntaxInfo, syntaxInfo.raw());
    }
}
