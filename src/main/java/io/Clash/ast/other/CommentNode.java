package io.Clash.ast.other;

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
}
