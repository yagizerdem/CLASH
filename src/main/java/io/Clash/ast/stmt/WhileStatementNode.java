package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class WhileStatementNode extends StmtNode {
    private final LoopKind loopKind;
    private final List<AstNode> condition;
    private final AstNode body;

    public WhileStatementNode(
            SyntaxInfo syntax,
            LoopKind loopKind,
            List<AstNode> condition,
            AstNode body
    ) {
        super(syntax, StatementNodeType.WHILE_STATEMENT);
        this.loopKind = StmtNode.required(loopKind, "loopKind");
        this.condition = StmtNode.list(condition, "condition");
        this.body = StmtNode.required(body, "body");
    }

    public LoopKind loopKind() {
        return loopKind;
    }

    public List<AstNode> condition() {
        return condition;
    }

    public AstNode body() {
        return body;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitWhileStatement(this);
    }
}
