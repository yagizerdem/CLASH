package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.other.ElseClauseNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cElseClauseParser extends cBaseParser {
    public cElseClauseParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "else_clause");
        List<AstNode> statements = new ArrayList<>();
        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            statements.add((StmtNode) this.parseChild(tsNode.getNamedChild(i)));
        }
        return new ElseClauseNode(this.extractSyntaxInfo(), statements);
    }
}
