package io.Clash.ast.expr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.enums.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class UnaryExpressionNode extends ExprNode {
    private final String operator;
    private final AstNode operand;

    public UnaryExpressionNode(
            SyntaxInfo syntax,
            String operator,
            AstNode operand
    ) {
        super(syntax, ExpressionNodeType.UNARY_EXPRESSION);
        this.operator = ExprNode.required(operator, "operator");
        this.operand = ExprNode.required(operand, "operand");
    }

    public String operator() {
        return operator;
    }

    public AstNode operand() {
        return operand;
    }


    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitUnaryExpression(this);
    }
}
