package io.Clash.ast.stmt;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StatementNodeType;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class DeclarationCommandNode extends StmtNode {
    private final DeclarationKind declarationKind;
    private final List<AstNode> arguments;

    public DeclarationCommandNode(
            SyntaxInfo syntax,
            DeclarationKind declarationKind,
            List<AstNode> arguments
    ) {
        super(syntax, StatementNodeType.DECLARATION_COMMAND);
        this.declarationKind = StmtNode.required(declarationKind, "declarationKind");
        this.arguments = StmtNode.list(arguments, "arguments");
    }

    public DeclarationKind declarationKind() {
        return declarationKind;
    }

    public List<AstNode> arguments() {
        return arguments;
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitDeclarationCommand(this);
    }
}
