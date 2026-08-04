package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.ForStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cForStatementParser extends cBaseParser {

    public cForStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "for_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        AstNode variable = this.parseChild(tsNode.getChildByFieldName("variable"));
        AstNode body = this.parseChild(tsNode.getChildByFieldName("body"));
        List<AstNode> values = new ArrayList<>();

        for (TSNode child : this.getChildrenByFieldName(tsNode, "value")) {
            values.add(this.parseChild(child));
        }

        return new ForStatementNode(syntaxInfo, variable, values, body);
    }
}
