package io.Clash.ast;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.visitor.Ivisitor;

import java.util.List;

public final class ProgramNode implements AstNode {
    private final List<AstNode> statements;
    private final SyntaxInfo syntax;

    public ProgramNode(SyntaxInfo syntax,  List<AstNode> statements) {
        this.statements = statements;
        this.syntax = syntax;
    }

    public List<AstNode> getStatements() {
        return statements;
    }

    public SyntaxInfo getSyntax() {
        return syntax;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitProgram(this);
    }
}