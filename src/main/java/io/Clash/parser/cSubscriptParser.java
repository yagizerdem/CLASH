package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.SubscriptNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cSubscriptParser extends cBaseParser {
    public cSubscriptParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "subscript");
        TSNode tsName = tsNode.getChildByFieldName("name");
        TSNode tsIndex = tsNode.getChildByFieldName("index");
        List<AstNode> suffix = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            if (tsNode.getFieldNameForNamedChild(i) == null) {
                suffix.add(this.parseChild(tsNode.getNamedChild(i)));
            }
        }

        return new SubscriptNode(
                this.extractSyntaxInfo(),
                this.parseChild(tsName),
                this.parseChild(tsIndex),
                suffix
        );
    }
}
