package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.ParenthesizedExpressionNode;
import io.Clash.ast.other.RegexNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cParenthesizedExpressionParser  extends cBaseParser {

    public cParenthesizedExpressionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "parenthesized_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> parts = this.collectNamedChildren(tsNode);
        return new ParenthesizedExpressionNode(syntaxInfo, parts);
    }
}
