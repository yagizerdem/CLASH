package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.BinaryExpressionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cBinaryExpressionParser extends cBaseParser {

    public cBinaryExpressionParser(String program, TSNode node) {
        super(program, node);
    }


    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "binary_expression");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode tsLeftNode = tsNode.getChildByFieldName("left");
        String operator = this.getProgramByOffsets(tsNode.getChildByFieldName("operator"));
        List<TSNode> tsRightNodes = this.getChildrenByFieldName(tsNode, "right");

        AstNode cLeftNode = this.dispatcher(tsLeftNode).parse(tsLeftNode);
        List<AstNode> cRightNodes = new ArrayList<>();
        tsRightNodes.forEach(tsn -> {
            cRightNodes.add(this.dispatcher(tsn).parse(tsn));
        });

        return new BinaryExpressionNode(syntaxInfo, cLeftNode, operator, cRightNodes);
    }
}
