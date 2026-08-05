package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class CommandNode extends StmtNode {
    private final AstNode name;
    private final List<AstNode> arguments;
    private final List<AstNode> redirects;
    private final List<AstNode> prefixes;

    public CommandNode(
            SyntaxInfo syntax,
            AstNode name,
            List<AstNode> arguments,
            List<AstNode> redirects,
            List<AstNode> prefixes
    ) {
        super(syntax, StatementNodeType.COMMAND);
        this.name = StmtNode.required(name, "name");
        this.arguments = StmtNode.list(arguments, "arguments");
        this.redirects = StmtNode.list(redirects, "redirects");
        this.prefixes = StmtNode.list(prefixes, "prefixes");
    }

    public AstNode name() {
        return name;
    }

    public List<AstNode> arguments() {
        return arguments;
    }

    public List<AstNode> redirects() {
        return redirects;
    }

    public List<AstNode> prefixes() {
        return prefixes;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitCommand(this);
    }
}
