package io.Clash.ast.base;

import io.Clash.ast.visitor.Ivisitor;

public interface AstNode {
    public SyntaxInfo getSyntax();

    public <T> T visit(Ivisitor visitor);
}
