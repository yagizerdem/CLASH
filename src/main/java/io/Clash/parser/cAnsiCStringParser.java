package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.AnsiCStringNode;
import io.Clash.ast.primaryExpr.RawStringNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cAnsiCStringParser extends cBaseParser {

    public cAnsiCStringParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "ansi_c_string");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new AnsiCStringNode(syntaxInfo);
    }
}
