package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class CommentNode extends BaseAstNode {
    private final String content;

    public CommentNode(SyntaxInfo syntax, String content) {
        super(syntax);
        this.content = content;
    }

    public String content() {
        return content;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitComment(this);
    }
}
