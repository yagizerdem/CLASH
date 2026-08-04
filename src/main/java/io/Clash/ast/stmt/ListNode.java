package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ListNode extends StmtNode {
    private final List<AstNode> statements;
    private final List<ListOperator> operators;

    public ListNode(
            SyntaxInfo syntax,
            List<AstNode> statements,
            List<ListOperator> operators
    ) {
        super(syntax, StatementNodeType.LIST);
        this.statements = StmtNode.list(statements, "statements");
        this.operators = StmtNode.list(operators, "operators");
    }

    public List<AstNode> statements() {
        return statements;
    }

    public List<ListOperator> operators() {
        return operators;
    }
}
