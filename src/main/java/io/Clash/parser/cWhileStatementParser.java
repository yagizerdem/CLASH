package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.enums.LoopKind;
import io.Clash.ast.stmt.WhileStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cWhileStatementParser extends cBaseParser {

    public cWhileStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "while_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        LoopKind loopKind = syntaxInfo.raw().stripLeading().startsWith("until")
                ? LoopKind.UNTIL
                : LoopKind.WHILE;
        List<AstNode> condition = new ArrayList<>();

        for (TSNode child : this.getChildrenByFieldName(tsNode, "condition")) {
            if (child.isNamed()) {
                condition.add((StmtNode) this.parseChild(child));
            }
        }

        AstNode body = this.parseChild(tsNode.getChildByFieldName("body"));
        return new WhileStatementNode(syntaxInfo, loopKind, condition, body);
    }
}
