package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.NegatedCommandNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cNegatedCommandParser extends cBaseParser {

    public cNegatedCommandParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "negated_command");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode tsCommand = tsNode.getNamedChild(0);
        StmtNode command = (StmtNode) this.parseChild(tsCommand);
        return new NegatedCommandNode(syntaxInfo, command);
    }
}
