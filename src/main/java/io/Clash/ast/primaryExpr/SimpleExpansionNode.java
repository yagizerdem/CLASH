package io.Clash.ast.primaryExpr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class SimpleExpansionNode extends PrimaryExprNode {
    private final AstNode variable;

    public SimpleExpansionNode(
            SyntaxInfo syntax,
            AstNode variable
    ) {
        super(syntax, ExpressionNodeType.SIMPLE_EXPANSION);
        this.variable = ExprNode.required(variable, "variable");
    }

    public AstNode getVariable() {
        return variable;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitSimpleExpansion(this);
    }
}
