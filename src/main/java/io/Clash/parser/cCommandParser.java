package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.CommandNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cCommandParser extends cBaseParser {

    public cCommandParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "command");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        AstNode name = this.parseChild(tsNode.getChildByFieldName("name"));
        List<AstNode> arguments = this.parseFields(tsNode, "argument");
        List<AstNode> redirects = this.parseFields(tsNode, "redirect");
        List<AstNode> prefixes = new ArrayList<>();

        for (int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            if (child.getType().equals("variable_assignment")
                    || child.getType().equals("subshell")) {
                prefixes.add(this.parseChild(child));
            }
        }

        return new CommandNode(syntaxInfo, name, arguments, redirects, prefixes);
    }

    private List<AstNode> parseFields(TSNode tsNode, String fieldName) {
        List<AstNode> nodes = new ArrayList<>();
        for (TSNode child : this.getChildrenByFieldName(tsNode, fieldName)) {
            nodes.add(this.parseChild(child));
        }
        return nodes;
    }
}
