package io.Clash.ast.expr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class WordNode extends PrimaryExprNode {
    public WordNode(
            SyntaxInfo syntax
    ) {
        super(syntax, ExpressionNodeType.WORD);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitWord(this);
    }
}
