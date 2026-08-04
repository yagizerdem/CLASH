package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.UnsetCommandNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cUnsetCommandParser extends cBaseParser {

    public cUnsetCommandParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "unset_command");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> targets = this.collectNamedChildren(tsNode);
        return new UnsetCommandNode(syntaxInfo, targets);
    }
}
