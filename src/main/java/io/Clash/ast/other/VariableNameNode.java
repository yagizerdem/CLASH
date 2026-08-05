package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.*;

import java.util.List;

public class VariableNameNode extends BaseAstNode {
    private final String identifier;

    public VariableNameNode(
            SyntaxInfo syntax,
            String identifier
    ) {
        super(syntax);
        this.identifier = identifier;
    }

    public String getVar() {
        return this.identifier;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitVariableName(this);
    }
}
