package io.Clash.ast.stmt;

import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ListNode extends StmtNode {
    private final List<StmtNode> statements;
    private final List<ListOperator> operators;

    public ListNode(
            SyntaxInfo syntax,
            List<StmtNode> statements,
            List<ListOperator> operators
    ) {
        super(syntax, StatementNodeType.LIST);
        this.statements = StmtNode.list(statements, "statements");
        this.operators = StmtNode.list(operators, "operators");
    }

    public List<StmtNode> statements() {
        return statements;
    }

    public List<ListOperator> operators() {
        return operators;
    }
}
