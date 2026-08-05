package io.Clash.ast.base;

import io.Clash.ast.visitor.Ivisitor;

public class ConcreteBaseAstNode extends BaseAstNode {

    public ConcreteBaseAstNode(SyntaxInfo syntax) {
        super(syntax);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitConcreteBase(this);
    }
}
