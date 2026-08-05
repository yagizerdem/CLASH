package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public class RegexNode extends BaseAstNode {
    private final String regex;

    public RegexNode(
            SyntaxInfo syntax,
            String regex
    ) {
        super(syntax);
        this.regex = regex;
    }

    public String getRegex() {
        return this.regex;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitRegex(this);
    }
}
