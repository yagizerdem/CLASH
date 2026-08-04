package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.HeredocBodyNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cHeredocBodyParser extends cBaseParser {
    public cHeredocBodyParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "heredoc_body");
        return new HeredocBodyNode(this.extractSyntaxInfo(), this.collectNamedChildren(tsNode));
    }
}
