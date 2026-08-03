package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.NumberNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cNumberParser extends cBaseParser {

    public cNumberParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "number");
        List<AstNode> parts = new ArrayList<>();
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        for(int i = 0; i < tsNode.getChildCount(); i++) {
            cBaseParser parser  = dispatcher(tsNode.getChild(i));
            AstNode child = parser.parse(tsNode.getChild(i));
            if(child != null) parts.add(child);
        }

        return new NumberNode(syntaxInfo, parts);
    }
}
