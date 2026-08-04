package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.StringNode;
import io.Clash.ast.primaryExpr.TranslatedStringNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cTranslatedStringParser extends cBaseParser {

    public cTranslatedStringParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "translated_string");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode stringTsNode = tsNode.getNamedChild(0);
        AstNode stringAst = this.dispatcher(stringTsNode).parse(stringTsNode);
        return new TranslatedStringNode(syntaxInfo, stringAst);
    }
}