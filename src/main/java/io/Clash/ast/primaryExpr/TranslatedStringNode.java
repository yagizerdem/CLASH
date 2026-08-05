package io.Clash.ast.primaryExpr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

public final class TranslatedStringNode extends PrimaryExprNode {
    private final AstNode value;

    public TranslatedStringNode(
            SyntaxInfo syntax,
            AstNode value
    ) {
        super(syntax, ExpressionNodeType.TRANSLATED_STRING);
        this.value = ExprNode.required(value, "value");
    }

    public AstNode value() {
        return value;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitTranslatedString(this);
    }
}
