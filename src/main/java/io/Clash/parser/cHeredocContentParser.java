package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.HeredocContentNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cHeredocContentParser extends cBaseParser {
    public cHeredocContentParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "heredoc_content");
        return new HeredocContentNode(this.extractSyntaxInfo());
    }
}
