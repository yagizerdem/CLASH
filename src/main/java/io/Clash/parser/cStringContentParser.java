package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.StringContentNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cStringContentParser extends cBaseParser {
    public cStringContentParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "string_content");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new StringContentNode(syntaxInfo, syntaxInfo.raw());
    }
}
