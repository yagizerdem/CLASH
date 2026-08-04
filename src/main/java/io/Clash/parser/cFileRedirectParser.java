package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.FileRedirectNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class cFileRedirectParser extends cBaseParser {
    private static final Set<String> OPERATORS = Set.of(
            "<", ">", ">>", "&>", "&>>", "<&", ">&", ">|", "<&-", ">&-"
    );

    public cFileRedirectParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "file_redirect");
        TSNode tsDescriptor = tsNode.getChildByFieldName("descriptor");
        AstNode descriptor = this.isMissing(tsDescriptor) ? null : this.parseChild(tsDescriptor);
        List<AstNode> destinations = new ArrayList<>();
        for (TSNode child : this.getChildrenByFieldName(tsNode, "destination")) {
            destinations.add(this.parseChild(child));
        }

        return new FileRedirectNode(
                this.extractSyntaxInfo(),
                descriptor,
                this.findOperator(tsNode),
                destinations
        );
    }

    private String findOperator(TSNode tsNode) {
        for (int i = 0; i < tsNode.getChildCount(); i++) {
            String raw = this.getProgramByOffsets(tsNode.getChild(i));
            if (OPERATORS.contains(raw)) {
                return raw;
            }
        }
        throw new IllegalStateException("file_redirect operator is missing");
    }
}
