package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class UnsetCommandNode extends StmtNode {
    private final List<AstNode> targets;

    public UnsetCommandNode(
            SyntaxInfo syntax,
            List<AstNode> targets
    ) {
        super(syntax, StatementNodeType.UNSET_COMMAND);
        this.targets = StmtNode.list(targets, "targets");
    }

    public List<AstNode> targets() {
        return targets;
    }
}
