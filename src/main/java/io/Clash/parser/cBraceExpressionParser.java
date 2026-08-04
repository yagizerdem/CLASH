package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.BinaryExpressionNode;
import io.Clash.ast.primaryExpr.BraceExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cBraceExpressionParser extends cBaseParser {

    public cBraceExpressionParser(String program, TSNode node) {
        super(program, node);
    }


    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "brace_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> parts = this.collectNamedChildren();
        return new BraceExpressionNode(syntaxInfo, parts);
    }
}
