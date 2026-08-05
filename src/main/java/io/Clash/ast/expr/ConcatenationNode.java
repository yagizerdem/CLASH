package io.Clash.ast.expr;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ConcatenationNode extends ExprNode {
    private final List<AstNode> parts;

    public ConcatenationNode(
            SyntaxInfo syntax,
            List<AstNode> parts
    ) {
        super(syntax, ExpressionNodeType.CONCATENATION);
        this.parts = ExprNode.list(parts, "parts");
    }

    public List<AstNode> parts() {
        return parts;
    }



    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitConcatenation(this);
    }
}
