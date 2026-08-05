package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class HeredocStartNode extends BaseAstNode {
    public HeredocStartNode(SyntaxInfo syntax) {
        super(syntax);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitHeredocStart(this);
    }
}
