package io.Clash.ast.other;

import io.Clash.ast.base.*;

import java.util.List;

public class ParenthesizedExpressionNode extends BaseAstNode {
    private final List<AstNode> parts;

    public ParenthesizedExpressionNode(
            SyntaxInfo syntax,
            List<AstNode> parts
    ) {
        super(syntax);
        this.parts = parts;
    }

    public  List<AstNode> getParts() {
        return parts;
    }
}
