package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.ParenthesizedExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cParenthesizedExpressionParser extends cBaseParser {

    public cParenthesizedExpressionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "parenthesized_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode tsExpression = tsNode.getNamedChild(0);
        AstNode expression = this.dispatcher(tsExpression).parse(tsExpression);
        return new ParenthesizedExpressionNode(syntaxInfo, expression);
    }
}
