package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.enums.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class RedirectedStatementNode extends StmtNode {
    private final AstNode body;
    private final List<AstNode> redirects;

    public RedirectedStatementNode(
            SyntaxInfo syntax,
            AstNode body,
            List<AstNode> redirects
    ) {
        super(syntax, StatementNodeType.REDIRECTED_STATEMENT);
        this.body = body;
        this.redirects = StmtNode.list(redirects, "redirects");
    }

    public AstNode body() {
        return body;
    }

    public List<AstNode> redirects() {
        return redirects;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitRedirectedStatement(this);
    }
}
