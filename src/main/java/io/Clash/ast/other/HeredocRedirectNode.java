package io.Clash.ast.other;

import io.Clash.ast.enums.RedirectionKind;
import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;
import java.util.Objects;

public final class HeredocRedirectNode extends BaseAstNode {
    private final AstNode descriptor;
    private final RedirectionKind redirectOperator;
    private final List<AstNode> arguments;
    private final String operator;
    private final List<AstNode> redirects;
    private final StmtNode right;
    private final List<AstNode> parts;

    public HeredocRedirectNode(
            SyntaxInfo syntax,
            AstNode descriptor,
            String redirectOperator,
            List<AstNode> arguments,
            String operator,
            List<AstNode> redirects,
            StmtNode right,
            List<AstNode> parts
    ) {
        super(syntax);
        this.descriptor = descriptor;
        this.redirectOperator = RedirectionKind.fromSymbol(Objects.requireNonNull(redirectOperator, "redirectOperator"));
        this.arguments = List.copyOf(arguments);
        this.operator = operator;
        this.redirects = List.copyOf(redirects);
        this.right = right;
        this.parts = List.copyOf(parts);
    }

    public HeredocRedirectNode(
            SyntaxInfo syntax,
            AstNode descriptor,
            RedirectionKind redirectOperator,
            List<AstNode> arguments,
            String operator,
            List<AstNode> redirects,
            StmtNode right,
            List<AstNode> parts
    ) {
        super(syntax);
        this.descriptor = descriptor;
        this.redirectOperator = Objects.requireNonNull(redirectOperator, "redirectOperator");
        this.arguments = List.copyOf(arguments);
        this.operator = operator;
        this.redirects = List.copyOf(redirects);
        this.right = right;
        this.parts = List.copyOf(parts);
    }



    public AstNode descriptor() { return descriptor; }
    public RedirectionKind redirectOperator() { return redirectOperator; }
    public List<AstNode> arguments() { return arguments; }
    public String operator() { return operator; }
    public List<AstNode> redirects() { return redirects; }
    public StmtNode right() { return right; }
    public List<AstNode> parts() { return parts; }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitHeredocRedirect(this);
    }
}
