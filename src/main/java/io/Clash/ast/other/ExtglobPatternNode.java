package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class ExtglobPatternNode extends BaseAstNode {
    public ExtglobPatternNode(SyntaxInfo syntax) {
        super(syntax);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitExtglobPattern(this);
    }
}
