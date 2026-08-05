package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

public final class VariableAssignmentNode extends StmtNode {
    private final AstNode name;
    private final AstNode value;

    public VariableAssignmentNode(
            SyntaxInfo syntax,
            AstNode name,
            AstNode value
    ) {
        super(syntax, StatementNodeType.VARIABLE_ASSIGNMENT);
        this.name = StmtNode.required(name, "name");
        this.value = StmtNode.required(value, "value");
    }

    public AstNode name() {
        return name;
    }

    public AstNode value() {
        return value;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitVariableAssignment(this);
    }
}
