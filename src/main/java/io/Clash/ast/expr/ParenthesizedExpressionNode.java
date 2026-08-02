package io.Clash.ast.expr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class ParenthesizedExpressionNode extends ExprNode {
    private final AstNode expression;

    public ParenthesizedExpressionNode(
            SyntaxInfo syntax,
            AstNode expression
    ) {
        super(syntax, ExpressionNodeType.PARENTHESIZED_EXPRESSION);
        this.expression = ExprNode.required(expression, "expression");
    }

    public AstNode expression() {
        return expression;
    }
}
