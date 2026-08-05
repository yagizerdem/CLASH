package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public class SpecialVariableNameNode extends BaseAstNode {
    private final String identifier;

    public SpecialVariableNameNode(
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
        return visitor.visitSpecialVariableName(this);
    }
}
