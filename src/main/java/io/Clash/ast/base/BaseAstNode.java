package io.Clash.ast.base;

public abstract class BaseAstNode implements AstNode{
    private final SyntaxInfo syntax;

    public BaseAstNode(SyntaxInfo syntax) {
        this.syntax = syntax;
    }

    @Override
    public SyntaxInfo getSyntax() {
        return this.syntax;
    }


}
