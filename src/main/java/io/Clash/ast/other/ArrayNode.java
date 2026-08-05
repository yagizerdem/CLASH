package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ArrayNode extends BaseAstNode {
    private final List<AstNode> elements;

    public ArrayNode(SyntaxInfo syntax, List<AstNode> elements) {
        super(syntax);
        this.elements = List.copyOf(elements);
    }

    public List<AstNode> elements() {
        return elements;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitArray(this);
    }
}
