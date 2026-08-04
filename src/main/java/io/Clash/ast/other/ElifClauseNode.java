package io.Clash.ast.other;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ElifClauseNode extends BaseAstNode {
    private final List<StmtNode> condition;
    private final List<StmtNode> thenBranch;

    public ElifClauseNode(
            SyntaxInfo syntax,
            List<StmtNode> condition,
            List<StmtNode> thenBranch
    ) {
        super(syntax);
        this.condition = List.copyOf(condition);
        this.thenBranch = List.copyOf(thenBranch);
    }

    public List<StmtNode> condition() {
        return condition;
    }

    public List<StmtNode> thenBranch() {
        return thenBranch;
    }
}
