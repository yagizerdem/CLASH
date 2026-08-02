package io.Clash.ast.stmt;

import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class PipelineNode extends StmtNode {
    private final List<StmtNode> commands;
    private final List<PipeOperator> operators;

    public PipelineNode(
            SyntaxInfo syntax,
            List<StmtNode> commands,
            List<PipeOperator> operators
    ) {
        super(syntax, StatementNodeType.PIPELINE);
        this.commands = StmtNode.list(commands, "commands");
        this.operators = StmtNode.list(operators, "operators");
    }

    public List<StmtNode> commands() {
        return commands;
    }

    public List<PipeOperator> operators() {
        return operators;
    }
}
