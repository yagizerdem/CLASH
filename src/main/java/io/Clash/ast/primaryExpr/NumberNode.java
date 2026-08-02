package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class NumberNode extends PrimaryExprNode {
    private final List<AstNode> parts;

    public NumberNode(
            SyntaxInfo syntax,
            List<AstNode> parts
    ) {
        super(syntax, ExpressionNodeType.NUMBER);
        this.parts = ExprNode.list(parts, "parts");
    }

    public List<AstNode> parts() {
        return parts;
    }
}
