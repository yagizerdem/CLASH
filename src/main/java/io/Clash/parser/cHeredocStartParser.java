package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.HeredocStartNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cHeredocStartParser extends cBaseParser {
    public cHeredocStartParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "heredoc_start");
        return new HeredocStartNode(this.extractSyntaxInfo());
    }
}
