package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class SubshellNode extends StmtNode {
    private final List<AstNode> statements;

    public SubshellNode(
            SyntaxInfo syntax,
            List<AstNode> statements
    ) {
        super(syntax, StatementNodeType.SUBSHELL);
        this.statements = StmtNode.list(statements, "statements");
    }

    public List<AstNode> statements() {
        return statements;
    }
}
