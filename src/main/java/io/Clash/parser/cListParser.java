package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.ListNode;
import io.Clash.ast.stmt.ListOperator;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cListParser extends cBaseParser {

    public cListParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "list");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<StmtNode> statements = new ArrayList<>();
        List<ListOperator> operators = new ArrayList<>();

        for (int i = 0; i < tsNode.getChildCount(); i++) {
            TSNode child = tsNode.getChild(i);
            if (child.isNamed()) {
                statements.add((StmtNode) this.parseChild(child));
            } else {
                ListOperator operator = this.parseOperator(this.getProgramByOffsets(child));
                if (operator != null) {
                    operators.add(operator);
                }
            }
        }

        return new ListNode(syntaxInfo, statements, operators);
    }

    private ListOperator parseOperator(String raw) {
        return switch (raw) {
            case ";" -> ListOperator.SEQUENCE;
            case "&" -> ListOperator.BACKGROUND;
            case "&&" -> ListOperator.LOGICAL_AND;
            case "||" -> ListOperator.LOGICAL_OR;
            default -> null;
        };
    }
}
