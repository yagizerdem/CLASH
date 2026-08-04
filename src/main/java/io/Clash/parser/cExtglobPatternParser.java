package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.ExtglobPatternNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cExtglobPatternParser extends cBaseParser {
    public cExtglobPatternParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "extglob_pattern");
        return new ExtglobPatternNode(this.extractSyntaxInfo());
    }
}
