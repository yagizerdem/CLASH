package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CompoundStatementNode extends StmtNode {
    private final List<AstNode> elements;

    public CompoundStatementNode(
            SyntaxInfo syntax,
            List<AstNode> elements
    ) {
        super(syntax, StatementNodeType.COMPOUND_STATEMENT);
        this.elements = StmtNode.list(elements, "elements");
    }

    public List<AstNode> elements() {
        return elements;
    }
}
