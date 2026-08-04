package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.AnsiCStringNode;
import io.Clash.ast.primaryExpr.ArithmeticExpansionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cArithmeticExpansionParser extends cBaseParser {

    public cArithmeticExpansionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "arithmetic_expansion");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> parts = this.collectNamedChildren(tsNode);
        return new ArithmeticExpansionNode(syntaxInfo, parts);
    }
}
