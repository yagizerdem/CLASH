package io.Clash.ast.expr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.visitor.Ivisitor;

import java.util.List;

public final class BinaryExpressionNode extends ExprNode {
    private final AstNode left;
    private final String operator;
    private final List<AstNode> right;

    public BinaryExpressionNode(
            SyntaxInfo syntax,
            AstNode left,
            String operator,
            List<AstNode> right
    ) {
        super(syntax, ExpressionNodeType.BINARY_EXPRESSION);
        this.left = left;
        this.operator = ExprNode.required(operator, "operator");
        this.right = ExprNode.list(right, "right");
    }

    public AstNode left() {
        return left;
    }

    public String operator() {
        return operator;
    }

    public List<AstNode> right() {
        return right;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitBinaryExpression(this);
    }
}
