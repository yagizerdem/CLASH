package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.IfStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cIfStatementParser extends cBaseParser {

    public cIfStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "if_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<StmtNode> condition = new ArrayList<>();
        List<StmtNode> thenBranch = new ArrayList<>();
        List<AstNode> elifClauses = new ArrayList<>();
        AstNode elseClause = null;

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            String fieldName = tsNode.getFieldNameForNamedChild(i);
            AstNode parsed = this.parseChild(child);

            if ("condition".equals(fieldName)) {
                condition.add((StmtNode) parsed);
            } else if (child.getType().equals("elif_clause")) {
                elifClauses.add(parsed);
            } else if (child.getType().equals("else_clause")) {
                elseClause = parsed;
            } else {
                thenBranch.add((StmtNode) parsed);
            }
        }

        return new IfStatementNode(
                syntaxInfo,
                condition,
                thenBranch,
                elifClauses,
                elseClause
        );
    }
}
