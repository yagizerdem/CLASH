package io.Clash.ast.base;

import io.Clash.ast.enums.ExpressionNodeType;

public abstract class PrimaryExprNode extends ExprNode {
    protected PrimaryExprNode(SyntaxInfo syntax, ExpressionNodeType type) {
        super(syntax, type);
    }
}
