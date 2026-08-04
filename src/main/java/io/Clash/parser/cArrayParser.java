package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.ArrayNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cArrayParser extends cBaseParser {
    public cArrayParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "array");
        return new ArrayNode(this.extractSyntaxInfo(), this.collectNamedChildren(tsNode));
    }
}
