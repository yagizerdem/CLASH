package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.other.CaseItemNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cCaseItemParser extends cBaseParser {
    public cCaseItemParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "case_item");
        List<AstNode> values = new ArrayList<>();
        List<StmtNode> statements = new ArrayList<>();

        for (TSNode child : this.getChildrenByFieldName(tsNode, "value")) {
            values.add(this.parseChild(child));
        }

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            if (tsNode.getFieldNameForNamedChild(i) == null) {
                statements.add((StmtNode) this.parseChild(child));
            }
        }

        TSNode tsTerminator = tsNode.getChildByFieldName("termination");
        if (this.isMissing(tsTerminator)) {
            tsTerminator = tsNode.getChildByFieldName("fallthrough");
        }
        String terminator = this.isMissing(tsTerminator)
                ? null
                : this.getProgramByOffsets(tsTerminator);

        return new CaseItemNode(this.extractSyntaxInfo(), values, statements, terminator);
    }
}
