package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.RawStringNode;
import io.Clash.ast.primaryExpr.StringNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cRawStringParser extends cBaseParser {

    public cRawStringParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "raw_string");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new RawStringNode(syntaxInfo);
    }
}
