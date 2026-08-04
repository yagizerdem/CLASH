package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class DoGroupNode extends BaseAstNode {
    private final List<AstNode> statements;

    public DoGroupNode(SyntaxInfo syntax, List<AstNode> statements) {
        super(syntax);
        this.statements = List.copyOf(statements);
    }

    public List<AstNode> statements() {
        return statements;
    }
}
