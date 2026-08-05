package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;
import java.util.Objects;

public final class FileRedirectNode extends BaseAstNode {
    private final AstNode descriptor;
    private final String operator;
    private final List<AstNode> destinations;

    public FileRedirectNode(
            SyntaxInfo syntax,
            AstNode descriptor,
            String operator,
            List<AstNode> destinations
    ) {
        super(syntax);
        this.descriptor = descriptor;
        this.operator = Objects.requireNonNull(operator, "operator");
        this.destinations = List.copyOf(destinations);
    }

    public AstNode descriptor() {
        return descriptor;
    }

    public String operator() {
        return operator;
    }

    public List<AstNode> destinations() {
        return destinations;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitFileRedirect(this);
    }
}
