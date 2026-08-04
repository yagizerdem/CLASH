package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.*;

import java.util.List;

public final class ProcessSubstitutionNode extends PrimaryExprNode {
    // private final ProcessSubstitutionKind substitutionKind;
    private final List<AstNode> statements;

    public ProcessSubstitutionNode(
            SyntaxInfo syntax,
           //  ProcessSubstitutionKind substitutionKind,
            List<AstNode> statements
    ) {
        super(syntax, ExpressionNodeType.PROCESS_SUBSTITUTION);
        // this.substitutionKind = ExprNode.required(substitutionKind, "substitutionKind");
        this.statements = ExprNode.list(statements, "statements");
    }

//    public ProcessSubstitutionKind substitutionKind() {
//        return substitutionKind;
//    }

    public List<AstNode> statements() {
        return statements;
    }
}
