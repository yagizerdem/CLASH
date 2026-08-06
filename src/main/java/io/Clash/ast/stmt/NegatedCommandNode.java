package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.enums.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

public final class NegatedCommandNode extends StmtNode {
    private final AstNode command;

    public NegatedCommandNode(
            SyntaxInfo syntax,
            AstNode command
    ) {
        super(syntax, StatementNodeType.NEGATED_COMMAND);
        this.command = StmtNode.required(command, "command");
    }

    public AstNode command() {
        return command;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitNegatedCommand(this);
    }
}
