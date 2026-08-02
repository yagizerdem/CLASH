package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class RawStringNode extends PrimaryExprNode {
    public RawStringNode(
            SyntaxInfo syntax
    ) {
        super(syntax, ExpressionNodeType.RAW_STRING);
    }
}
