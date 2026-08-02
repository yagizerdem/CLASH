package io.Clash.ast.expr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class TernaryExpressionNode extends ExprNode {
    private final AstNode condition;
    private final AstNode consequence;
    private final AstNode alternative;

    public TernaryExpressionNode(
            SyntaxInfo syntax,
            AstNode condition,
            AstNode consequence,
            AstNode alternative
    ) {
        super(syntax, ExpressionNodeType.TERNARY_EXPRESSION);
        this.condition = ExprNode.required(condition, "condition");
        this.consequence = ExprNode.required(consequence, "consequence");
        this.alternative = ExprNode.required(alternative, "alternative");
    }

    public AstNode condition() {
        return condition;
    }

    public AstNode consequence() {
        return consequence;
    }

    public AstNode alternative() {
        return alternative;
    }
}
