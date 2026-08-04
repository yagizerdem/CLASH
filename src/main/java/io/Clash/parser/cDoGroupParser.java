package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.other.DoGroupNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cDoGroupParser extends cBaseParser {
    public cDoGroupParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "do_group");
        List<AstNode> statements = new ArrayList<>();
        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            statements.add(this.parseChild(tsNode.getNamedChild(i)));
        }
        return new DoGroupNode(this.extractSyntaxInfo(), statements);
    }
}
