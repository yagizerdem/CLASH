package io.Clash.ast.stmt;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

public final class TestCommandNode extends StmtNode {
    private final AstNode expression;

    public TestCommandNode(
            SyntaxInfo syntax,
            AstNode expression
    ) {
        super(syntax, StatementNodeType.TEST_COMMAND);
        this.expression = expression;
    }

    public AstNode expression() {
        return expression;
    }
}
