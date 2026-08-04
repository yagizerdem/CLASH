package io.Clash.ast.other;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ElseClauseNode extends BaseAstNode {
    private final List<StmtNode> statements;

    public ElseClauseNode(SyntaxInfo syntax, List<StmtNode> statements) {
        super(syntax);
        this.statements = List.copyOf(statements);
    }

    public List<StmtNode> statements() {
        return statements;
    }
}
