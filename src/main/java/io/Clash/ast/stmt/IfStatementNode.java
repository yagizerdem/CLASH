package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class IfStatementNode extends StmtNode {
    private final List<StmtNode> condition;
    private final List<StmtNode> thenBranch;
    private final List<AstNode> elifClauses;
    private final AstNode elseClause;

    public IfStatementNode(
            SyntaxInfo syntax,
            List<StmtNode> condition,
            List<StmtNode> thenBranch,
            List<AstNode> elifClauses,
            AstNode elseClause
    ) {
        super(syntax, StatementNodeType.IF_STATEMENT);
        this.condition = StmtNode.list(condition, "condition");
        this.thenBranch = StmtNode.list(thenBranch, "thenBranch");
        this.elifClauses = StmtNode.list(elifClauses, "elifClauses");
        this.elseClause = elseClause;
    }

    public List<StmtNode> condition() {
        return condition;
    }

    public List<StmtNode> thenBranch() {
        return thenBranch;
    }

    public List<AstNode> elifClauses() {
        return elifClauses;
    }

    public AstNode elseClause() {
        return elseClause;
    }
}
