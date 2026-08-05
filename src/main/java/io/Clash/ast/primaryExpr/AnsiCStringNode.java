package io.Clash.ast.primaryExpr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class AnsiCStringNode extends PrimaryExprNode {
    public AnsiCStringNode(
            SyntaxInfo syntax
    ) {
        super(syntax, ExpressionNodeType.ANSI_C_STRING);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitAnsiCString(this);
    }
}
