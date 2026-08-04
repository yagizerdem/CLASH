package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.TernaryExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cTernaryExpressionParser extends cBaseParser {

    public cTernaryExpressionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "ternary_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        AstNode condition = this.parseField(tsNode, "condition");
        AstNode consequence = this.parseField(tsNode, "consequence");
        AstNode alternative = this.parseField(tsNode, "alternative");

        return new TernaryExpressionNode(
                syntaxInfo,
                condition,
                consequence,
                alternative
        );
    }

    private AstNode parseField(TSNode tsNode, String fieldName) {
        TSNode tsField = tsNode.getChildByFieldName(fieldName);
        return this.dispatcher(tsField).parse(tsField);
    }
}
