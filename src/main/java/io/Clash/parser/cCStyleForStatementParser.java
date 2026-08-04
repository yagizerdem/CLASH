package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.CStyleForStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cCStyleForStatementParser extends cBaseParser {

    public cCStyleForStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "c_style_for_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> initializer = this.parseFields(tsNode, "initializer");
        List<AstNode> condition = this.parseFields(tsNode, "condition");
        List<AstNode> update = this.parseFields(tsNode, "update");
        AstNode body = this.parseChild(tsNode.getChildByFieldName("body"));
        return new CStyleForStatementNode(syntaxInfo, initializer, condition, update, body);
    }

    private List<AstNode> parseFields(TSNode tsNode, String fieldName) {
        List<AstNode> nodes = new ArrayList<>();
        for (TSNode child : this.getChildrenByFieldName(tsNode, fieldName)) {
            nodes.add(this.parseChild(child));
        }
        return nodes;
    }
}
