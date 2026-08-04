package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;
import java.util.Objects;

public final class SubscriptNode extends BaseAstNode {
    private final AstNode name;
    private final AstNode index;
    private final List<AstNode> suffix;

    public SubscriptNode(
            SyntaxInfo syntax,
            AstNode name,
            AstNode index,
            List<AstNode> suffix
    ) {
        super(syntax);
        this.name = Objects.requireNonNull(name, "name");
        this.index = Objects.requireNonNull(index, "index");
        this.suffix = List.copyOf(suffix);
    }

    public AstNode name() { return name; }
    public AstNode index() { return index; }
    public List<AstNode> suffix() { return suffix; }
}
