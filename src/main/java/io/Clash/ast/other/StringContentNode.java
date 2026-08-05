package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class StringContentNode extends BaseAstNode {
    private final String value;

    public StringContentNode(SyntaxInfo syntax, String value) {
        super(syntax);
        this.value = value;
    }

    public String value() {
        return value;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitStringContent(this);
    }
}
