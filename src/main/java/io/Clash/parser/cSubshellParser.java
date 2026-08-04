package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.SubshellNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cSubshellParser extends cBaseParser {

    public cSubshellParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "subshell");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> statements = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            statements.add(this.parseChild(tsNode.getNamedChild(i)));
        }

        return new SubshellNode(syntaxInfo, statements);
    }
}
