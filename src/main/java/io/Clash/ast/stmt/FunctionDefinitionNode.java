package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class FunctionDefinitionNode extends StmtNode {
    private final AstNode name;
    private final AstNode body;
    private final List<AstNode> redirects;

    public FunctionDefinitionNode(
            SyntaxInfo syntax,
            AstNode name,
            AstNode body,
            List<AstNode> redirects
    ) {
        super(syntax, StatementNodeType.FUNCTION_DEFINITION);
        this.name = StmtNode.required(name, "name");
        this.body = StmtNode.required(body, "body");
        this.redirects = StmtNode.list(redirects, "redirects");
    }

    public AstNode name() {
        return name;
    }

    public AstNode body() {
        return body;
    }

    public List<AstNode> redirects() {
        return redirects;
    }
}
