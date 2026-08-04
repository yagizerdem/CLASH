package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.CaseStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cCaseStatementParser extends cBaseParser {

    public cCaseStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "case_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode tsValue = tsNode.getChildByFieldName("value");
        AstNode value = this.parseChild(tsValue);
        List<AstNode> items = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            if (child.getType().equals("case_item")) {
                items.add(this.parseChild(child));
            }
        }

        return new CaseStatementNode(syntaxInfo, value, items);
    }
}
