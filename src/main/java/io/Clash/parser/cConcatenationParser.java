package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.ConcatenationNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cConcatenationParser extends cBaseParser {

    public cConcatenationParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "concatenation");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> parts = this.collectNamedChildren(tsNode);
        return new ConcatenationNode(syntaxInfo, parts);
    }
}
