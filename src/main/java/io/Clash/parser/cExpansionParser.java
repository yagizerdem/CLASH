package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SourceSpan;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.ExpansionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cExpansionParser extends cBaseParser {

    public cExpansionParser(String program, TSNode node) {
        super(program, node);
    }


    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "expansion");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        org.treesitter.TSNode fieldNode = tsNode.getChildByFieldName("operator");
        String op = fieldNode != null ? this.getProgramByOffsets(fieldNode) : null;
        List<AstNode> parts = this.collectNamedChildren(tsNode);
        return new ExpansionNode(syntaxInfo, op, parts);
    }
}
