package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CaseItemNode extends BaseAstNode {
    private final List<AstNode> values;
    private final List<AstNode> statements;
    private final String terminator;

    public CaseItemNode(
            SyntaxInfo syntax,
            List<AstNode> values,
            List<AstNode> statements,
            String terminator
    ) {
        super(syntax);
        this.values = List.copyOf(values);
        this.statements = List.copyOf(statements);
        this.terminator = terminator;
    }

    public List<AstNode> values() {
        return values;
    }

    public List<AstNode> statements() {
        return statements;
    }

    public String terminator() {
        return terminator;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitCaseItem(this);
    }
}
