package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ExpansionNode extends PrimaryExprNode {
    private final List<String> operator;
    private final List<AstNode> parts;

    public ExpansionNode(
            SyntaxInfo syntax,
            List<String> operator,
            List<AstNode> parts
    ) {
        super(syntax, ExpressionNodeType.EXPANSION);
        this.operator = operator;
        this.parts = ExprNode.list(parts, "parts");
    }

    public List<String> operator() {
        return operator;
    }

    public List<AstNode> parts() {
        return parts;
    }
}
