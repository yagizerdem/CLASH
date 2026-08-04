package io.Clash.ast.other;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class TestOperatorNode extends BaseAstNode {
    private final String operator;

    public TestOperatorNode(SyntaxInfo syntax, String operator) {
        super(syntax);
        this.operator = operator;
    }

    public String operator() {
        return operator;
    }
}
