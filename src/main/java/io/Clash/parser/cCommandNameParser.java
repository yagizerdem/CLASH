package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.CommandNameNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cCommandNameParser extends cBaseParser {
    public cCommandNameParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "command_name");
        TSNode tsValue = tsNode.getNamedChild(0);
        return new CommandNameNode(this.extractSyntaxInfo(), this.parseChild(tsValue));
    }
}
