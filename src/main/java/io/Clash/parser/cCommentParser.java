package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.other.CommentNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cCommentParser extends cBaseParser {
    public cCommentParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "comment");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        return new CommentNode(syntaxInfo, syntaxInfo.raw());
    }
}
