package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CommandSubstitutionNode extends PrimaryExprNode {
    private final List<AstNode> statements;
    private final AstNode redirect;

    public CommandSubstitutionNode(
            SyntaxInfo syntax,
            List<AstNode> statements,
            AstNode redirect
    ) {
        super(syntax, ExpressionNodeType.COMMAND_SUBSTITUTION);
        this.statements = ExprNode.list(statements, "statements");
        this.redirect = redirect;
    }

    public List<AstNode> statements() {
        return statements;
    }

    public AstNode redirect() {
        return redirect;
    }
}
