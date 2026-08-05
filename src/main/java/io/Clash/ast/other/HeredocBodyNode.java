package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class HeredocBodyNode extends BaseAstNode {
    private final List<AstNode> parts;

    public HeredocBodyNode(SyntaxInfo syntax, List<AstNode> parts) {
        super(syntax);
        this.parts = List.copyOf(parts);
    }

    public List<AstNode> parts() {
        return parts;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitHeredocBody(this);
    }
}
