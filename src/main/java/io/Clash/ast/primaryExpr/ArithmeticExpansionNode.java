package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ArithmeticExpansionNode extends PrimaryExprNode {
    private final List<AstNode> expressions;

    public ArithmeticExpansionNode(
            SyntaxInfo syntax,
            List<AstNode> expressions
    ) {
        super(syntax, ExpressionNodeType.ARITHMETIC_EXPANSION);
        this.expressions = ExprNode.list(expressions, "expressions");
    }

    public List<AstNode> expressions() {
        return expressions;
    }
}
