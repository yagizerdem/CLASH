package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class BraceExpressionNode extends PrimaryExprNode {
    private final List<AstNode> elements;

    public BraceExpressionNode(
            SyntaxInfo syntax,
            List<AstNode> elements
    ) {
        super(syntax, ExpressionNodeType.BRACE_EXPRESSION);
        this.elements = ExprNode.list(elements, "elements");
    }

    public List<AstNode> elements() {
        return elements;
    }
}
