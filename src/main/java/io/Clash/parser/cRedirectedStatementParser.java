package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.RedirectedStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cRedirectedStatementParser extends cBaseParser {

    public cRedirectedStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "redirected_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        TSNode tsBody = tsNode.getChildByFieldName("body");
        StmtNode body = tsBody == null ? null : (StmtNode) this.parseChild(tsBody);
        List<AstNode> redirects = new ArrayList<>();

        for (TSNode child : this.getChildrenByFieldName(tsNode, "redirect")) {
            redirects.add(this.parseChild(child));
        }

        return new RedirectedStatementNode(syntaxInfo, body, redirects);
    }
}
