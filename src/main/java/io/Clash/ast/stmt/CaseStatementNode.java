package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CaseStatementNode extends StmtNode {
    private final AstNode value;
    private final List<AstNode> items;

    public CaseStatementNode(
            SyntaxInfo syntax,
            AstNode value,
            List<AstNode> items
    ) {
        super(syntax, StatementNodeType.CASE_STATEMENT);
        this.value = StmtNode.required(value, "value");
        this.items = StmtNode.list(items, "items");
    }

    public AstNode value() {
        return value;
    }

    public List<AstNode> items() {
        return items;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitCaseStatement(this);
    }
}
