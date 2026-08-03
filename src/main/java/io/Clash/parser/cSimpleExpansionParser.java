package io.Clash.parser;

import io.Clash.ClashException;
import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.SimpleExpansionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cSimpleExpansionParser extends cBaseParser {

    public cSimpleExpansionParser(String program, TSNode node) {
        super(program, node);
    }


    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "simple_expansion");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        if(tsNode.getNamedChildCount() != 1) {
            throw new ClashException(ClashException.ErrorType.SYNTAX, "must has 1 child");
        }

        TSNode childTsNode = tsNode.getNamedChild(0);
        cBaseParser parser = this.dispatcher(childTsNode);
        String varName = parser.parse(childTsNode);

        SimpleExpansionNode node = new SimpleExpansionNode(syntaxInfo, varName);
        return node;
    }
}
