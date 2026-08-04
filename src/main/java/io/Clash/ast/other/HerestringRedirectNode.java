package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.Objects;

public final class HerestringRedirectNode extends BaseAstNode {
    private final AstNode descriptor;
    private final AstNode value;

    public HerestringRedirectNode(SyntaxInfo syntax, AstNode descriptor, AstNode value) {
        super(syntax);
        this.descriptor = descriptor;
        this.value = Objects.requireNonNull(value, "value");
    }

    public AstNode descriptor() {
        return descriptor;
    }

    public AstNode value() {
        return value;
    }
}
