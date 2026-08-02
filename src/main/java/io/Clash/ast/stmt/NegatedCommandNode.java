package io.Clash.ast.stmt;

import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

public final class NegatedCommandNode extends StmtNode {
    private final StmtNode command;

    public NegatedCommandNode(
            SyntaxInfo syntax,
            StmtNode command
    ) {
        super(syntax, StatementNodeType.NEGATED_COMMAND);
        this.command = StmtNode.required(command, "command");
    }

    public StmtNode command() {
        return command;
    }
}
