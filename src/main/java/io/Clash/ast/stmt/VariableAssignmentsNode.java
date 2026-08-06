package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.enums.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class VariableAssignmentsNode extends StmtNode {
    private final List<VariableAssignmentNode> assignments;

    public VariableAssignmentsNode(
            SyntaxInfo syntax,
            List<VariableAssignmentNode> assignments
    ) {
        super(syntax, StatementNodeType.VARIABLE_ASSIGNMENTS);
        this.assignments = StmtNode.list(assignments, "assignments");
    }

    public List<VariableAssignmentNode> assignments() {
        return assignments;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitVariableAssignments(this);
    }
}
