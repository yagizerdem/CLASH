package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.UnaryExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cUnaryExpressionParser extends cBaseParser {

    public cUnaryExpressionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "unary_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        TSNode tsOperator = tsNode.getChildByFieldName("operator");
        TSNode tsOperand = this.findOperand(tsNode, tsOperator);
        String operator = this.getProgramByOffsets(tsOperator);
        AstNode operand = this.dispatcher(tsOperand).parse(tsOperand);

        return new UnaryExpressionNode(syntaxInfo, operator, operand);
    }

    private TSNode findOperand(TSNode tsNode, TSNode tsOperator) {
        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            if (child.getStartByte() != tsOperator.getStartByte()
                    || child.getEndByte() != tsOperator.getEndByte()) {
                return child;
            }
        }

        throw new IllegalStateException("unary_expression operand is missing");
    }
}
