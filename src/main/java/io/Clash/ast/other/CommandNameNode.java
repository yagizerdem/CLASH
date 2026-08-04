package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.Objects;

public final class CommandNameNode extends BaseAstNode {
    private final AstNode value;

    public CommandNameNode(SyntaxInfo syntax, AstNode value) {
        super(syntax);
        this.value = Objects.requireNonNull(value, "value");
    }

    public AstNode value() {
        return value;
    }
}
