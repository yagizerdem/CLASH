package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.VariableAssignmentNode;
import io.Clash.ast.other.VariableNameNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cVariableAssignmentParser extends cBaseParser {

    public cVariableAssignmentParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "variable_assignment");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        String var = this.getProgramByOffsets();

        TSNode tsNameNode = tsNode.getChildByFieldName("name");
        TSNode tsValueNode = tsNode.getChildByFieldName("value");

        AstNode nameNode = this.dispatcher(tsNameNode).parse(tsNameNode);
        AstNode valueNode = this.dispatcher(tsValueNode).parse(tsValueNode);

        VariableAssignmentNode node = new VariableAssignmentNode(syntaxInfo, nameNode, valueNode);
        return node;
    }
}
