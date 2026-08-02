package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class WordNode extends PrimaryExprNode {
    public WordNode(
            SyntaxInfo syntax
    ) {
        super(syntax, ExpressionNodeType.WORD);
    }
}
