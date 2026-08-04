package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.VariableAssignmentNode;
import io.Clash.ast.stmt.VariableAssignmentsNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cVariableAssignmentsParser extends cBaseParser {

    public cVariableAssignmentsParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "variable_assignments");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<VariableAssignmentNode> assignments = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            assignments.add((VariableAssignmentNode) this.parseChild(tsNode.getNamedChild(i)));
        }

        return new VariableAssignmentsNode(syntaxInfo, assignments);
    }
}
