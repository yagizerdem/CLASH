package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.other.ElifClauseNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cElifClauseParser extends cBaseParser {
    public cElifClauseParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "elif_clause");
        int thenOffset = this.findThenOffset(tsNode);
        List<StmtNode> condition = new ArrayList<>();
        List<StmtNode> thenBranch = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            StmtNode statement = (StmtNode) this.parseChild(child);
            if (child.getStartByte() < thenOffset) {
                condition.add(statement);
            } else {
                thenBranch.add(statement);
            }
        }

        return new ElifClauseNode(this.extractSyntaxInfo(), condition, thenBranch);
    }

    private int findThenOffset(TSNode tsNode) {
        for (int i = 0; i < tsNode.getChildCount(); i++) {
            TSNode child = tsNode.getChild(i);
            if (this.getProgramByOffsets(child).equals("then")) {
                return child.getEndByte();
            }
        }
        throw new IllegalStateException("elif_clause is missing then");
    }
}
