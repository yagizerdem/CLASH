package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ElifClauseNode extends BaseAstNode {
    private final List<AstNode> condition;
    private final List<AstNode> thenBranch;

    public ElifClauseNode(
            SyntaxInfo syntax,
            List<AstNode> condition,
            List<AstNode> thenBranch
    ) {
        super(syntax);
        this.condition = List.copyOf(condition);
        this.thenBranch = List.copyOf(thenBranch);
    }

    public List<AstNode> condition() {
        return condition;
    }

    public List<AstNode> thenBranch() {
        return thenBranch;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitElifClause(this);
    }
}
