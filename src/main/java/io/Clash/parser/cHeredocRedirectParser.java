package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.other.HeredocRedirectNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cHeredocRedirectParser extends cBaseParser {
    public cHeredocRedirectParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "heredoc_redirect");
        TSNode tsDescriptor = tsNode.getChildByFieldName("descriptor");
        TSNode tsOperator = tsNode.getChildByFieldName("operator");
        TSNode tsRight = tsNode.getChildByFieldName("right");

        AstNode descriptor = this.isMissing(tsDescriptor) ? null : this.parseChild(tsDescriptor);
        String operator = this.isMissing(tsOperator) ? null : this.getProgramByOffsets(tsOperator);
        StmtNode right = this.isMissing(tsRight) ? null : (StmtNode) this.parseChild(tsRight);
        List<AstNode> arguments = this.parseFields(tsNode, "argument");
        List<AstNode> redirects = this.parseFields(tsNode, "redirect");
        List<AstNode> parts = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            if (tsNode.getFieldNameForNamedChild(i) == null) {
                parts.add(this.parseChild(tsNode.getNamedChild(i)));
            }
        }

        return new HeredocRedirectNode(
                this.extractSyntaxInfo(),
                descriptor,
                this.findRedirectOperator(tsNode),
                arguments,
                operator,
                redirects,
                right,
                parts
        );
    }

    private List<AstNode> parseFields(TSNode tsNode, String fieldName) {
        List<AstNode> nodes = new ArrayList<>();
        for (TSNode child : this.getChildrenByFieldName(tsNode, fieldName)) {
            nodes.add(this.parseChild(child));
        }
        return nodes;
    }

    private String findRedirectOperator(TSNode tsNode) {
        for (int i = 0; i < tsNode.getChildCount(); i++) {
            String raw = this.getProgramByOffsets(tsNode.getChild(i));
            if (raw.equals("<<") || raw.equals("<<-")) {
                return raw;
            }
        }
        throw new IllegalStateException("heredoc_redirect operator is missing");
    }
}
