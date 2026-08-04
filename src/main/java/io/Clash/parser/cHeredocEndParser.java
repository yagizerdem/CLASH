package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.HeredocEndNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cHeredocEndParser extends cBaseParser {
    public cHeredocEndParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "heredoc_end");
        return new HeredocEndNode(this.extractSyntaxInfo());
    }
}
