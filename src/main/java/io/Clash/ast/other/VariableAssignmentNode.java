package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public class VariableAssignmentNode extends BaseAstNode {
    private final AstNode name;

    private final AstNode value;

    public VariableAssignmentNode(
            SyntaxInfo syntax,
            AstNode name,
            AstNode value
    ) {
        super(syntax);
        this.name = name;
        this.value = value;
    }

    public AstNode getName() {
        return this.name;
    }

    public AstNode getValue() {
        return this.value;
    }
}