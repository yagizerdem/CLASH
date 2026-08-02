package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ForStatementNode extends StmtNode {
    private final AstNode variable;
    private final List<AstNode> values;
    private final AstNode body;

    public ForStatementNode(
            SyntaxInfo syntax,
            AstNode variable,
            List<AstNode> values,
            AstNode body
    ) {
        super(syntax, StatementNodeType.FOR_STATEMENT);
        this.variable = StmtNode.required(variable, "variable");
        this.values = StmtNode.list(values, "values");
        this.body = StmtNode.required(body, "body");
    }

    public AstNode variable() {
        return variable;
    }

    public List<AstNode> values() {
        return values;
    }

    public AstNode body() {
        return body;
    }
}
