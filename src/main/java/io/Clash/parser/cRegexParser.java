package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.RegexNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;



public class cRegexParser extends cBaseParser {

    public cRegexParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "regex");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        String regex = this.getProgramByOffsets();

        return new RegexNode(syntaxInfo, regex);
    }
}
