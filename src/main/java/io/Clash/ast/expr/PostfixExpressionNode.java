package io.Clash.ast.expr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class PostfixExpressionNode extends ExprNode {
    private final AstNode operand;
    private final String operator;

    public PostfixExpressionNode(
            SyntaxInfo syntax,
            AstNode operand,
            String operator
    ) {
        super(syntax, ExpressionNodeType.POSTFIX_EXPRESSION);
        this.operand = ExprNode.required(operand, "operand");
        this.operator = ExprNode.required(operator, "operator");
    }

    public AstNode operand() {
        return operand;
    }

    public String operator() {
        return operator;
    }
}
