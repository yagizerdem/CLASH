package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.expr.WordNode;
import io.Clash.ast.other.VariableNameNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cWordParser extends cBaseParser {

    public cWordParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "word");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new WordNode(syntaxInfo);
    }

}