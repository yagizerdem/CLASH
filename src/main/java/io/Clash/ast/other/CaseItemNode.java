package io.Clash.ast.other;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CaseItemNode extends BaseAstNode {
    private final List<AstNode> values;
    private final List<StmtNode> statements;
    private final String terminator;

    public CaseItemNode(
            SyntaxInfo syntax,
            List<AstNode> values,
            List<StmtNode> statements,
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

    public List<StmtNode> statements() {
        return statements;
    }

    public String terminator() {
        return terminator;
    }
}
