package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CommandSubstitutionNode extends PrimaryExprNode {
    private final List<StmtNode> statements;
    private final List<AstNode> redirects;

    public CommandSubstitutionNode(
            SyntaxInfo syntax,
            List<StmtNode> statements,
            List<AstNode> redirects
    ) {
        super(syntax, ExpressionNodeType.COMMAND_SUBSTITUTION);
        this.statements = ExprNode.list(statements, "statements");
        this.redirects = ExprNode.list(redirects, "redirects");
    }

    public List<StmtNode> statements() {
        return statements;
    }

    public List<AstNode> redirects() {
        return redirects;
    }
}
