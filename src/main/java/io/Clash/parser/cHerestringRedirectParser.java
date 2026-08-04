package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.HerestringRedirectNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cHerestringRedirectParser extends cBaseParser {
    public cHerestringRedirectParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "herestring_redirect");
        TSNode tsDescriptor = tsNode.getChildByFieldName("descriptor");
        AstNode descriptor = this.isMissing(tsDescriptor) ? null : this.parseChild(tsDescriptor);
        TSNode tsValue = this.findValue(tsNode, tsDescriptor);
        return new HerestringRedirectNode(
                this.extractSyntaxInfo(),
                descriptor,
                this.parseChild(tsValue)
        );
    }

    private TSNode findValue(TSNode tsNode, TSNode tsDescriptor) {
        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            if (this.isMissing(tsDescriptor)
                    || child.getStartByte() != tsDescriptor.getStartByte()
                    || child.getEndByte() != tsDescriptor.getEndByte()) {
                return child;
            }
        }
        throw new IllegalStateException("herestring_redirect value is missing");
    }
}
