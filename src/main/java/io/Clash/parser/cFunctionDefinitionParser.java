package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.FunctionDefinitionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cFunctionDefinitionParser extends cBaseParser {

    public cFunctionDefinitionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "function_definition");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        AstNode name = this.parseChild(tsNode.getChildByFieldName("name"));
        AstNode body = this.parseChild(tsNode.getChildByFieldName("body"));
        List<AstNode> redirects = new ArrayList<>();

        for (TSNode child : this.getChildrenByFieldName(tsNode, "redirect")) {
            redirects.add(this.parseChild(child));
        }

        return new FunctionDefinitionNode(syntaxInfo, name, body, redirects);
    }
}
