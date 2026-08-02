package io.Clash.ast.stmt;

import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class SubshellNode extends StmtNode {
    private final List<StmtNode> statements;

    public SubshellNode(
            SyntaxInfo syntax,
            List<StmtNode> statements
    ) {
        super(syntax, StatementNodeType.SUBSHELL);
        this.statements = StmtNode.list(statements, "statements");
    }

    public List<StmtNode> statements() {
        return statements;
    }
}
