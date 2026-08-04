package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.PostfixExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cPostfixExpressionParser extends cBaseParser {

    public cPostfixExpressionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "postfix_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        TSNode tsOperand = tsNode.getNamedChild(0);
        TSNode tsOperator = tsNode.getChildByFieldName("operator");
        AstNode operand = this.dispatcher(tsOperand).parse(tsOperand);
        String operator = this.getProgramByOffsets(tsOperator);

        return new PostfixExpressionNode(syntaxInfo, operand, operator);
    }
}
