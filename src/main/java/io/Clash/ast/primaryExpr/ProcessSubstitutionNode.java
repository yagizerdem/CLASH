package io.Clash.ast.primaryExpr;

import io.Clash.ast.base.ExpressionNodeType;
import io.Clash.ast.base.ExprNode;
import io.Clash.ast.base.PrimaryExprNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;

import java.util.List;

public final class ProcessSubstitutionNode extends PrimaryExprNode {
    private final ProcessSubstitutionKind substitutionKind;
    private final List<StmtNode> statements;

    public ProcessSubstitutionNode(
            SyntaxInfo syntax,
            ProcessSubstitutionKind substitutionKind,
            List<StmtNode> statements
    ) {
        super(syntax, ExpressionNodeType.PROCESS_SUBSTITUTION);
        this.substitutionKind = ExprNode.required(substitutionKind, "substitutionKind");
        this.statements = ExprNode.list(statements, "statements");
    }

    public ProcessSubstitutionKind substitutionKind() {
        return substitutionKind;
    }

    public List<StmtNode> statements() {
        return statements;
    }
}
