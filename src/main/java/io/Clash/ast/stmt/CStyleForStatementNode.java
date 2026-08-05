package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CStyleForStatementNode extends StmtNode {
    private final List<AstNode> initializer;
    private final List<AstNode> condition;
    private final List<AstNode> update;
    private final AstNode body;

    public CStyleForStatementNode(
            SyntaxInfo syntax,
            List<AstNode> initializer,
            List<AstNode> condition,
            List<AstNode> update,
            AstNode body
    ) {
        super(syntax, StatementNodeType.C_STYLE_FOR_STATEMENT);
        this.initializer = StmtNode.list(initializer, "initializer");
        this.condition = StmtNode.list(condition, "condition");
        this.update = StmtNode.list(update, "update");
        this.body = StmtNode.required(body, "body");
    }

    public List<AstNode> initializer() {
        return initializer;
    }

    public List<AstNode> condition() {
        return condition;
    }

    public List<AstNode> update() {
        return update;
    }

    public AstNode body() {
        return body;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitCStyleForStatement(this);
    }
}
