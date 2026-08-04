package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.VariableNameNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cVariableNameParser extends cBaseParser {

    public cVariableNameParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "variable_name");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        String var = this.getProgramByOffsets();
        VariableNameNode node = new VariableNameNode(syntaxInfo, var);
        return node;
    }

}
