package io.Clash.parser.core;

import io.Clash.ast.base.AstNode;
import org.treesitter.*;

import java.util.ArrayList;
import java.util.List;

public class BashParser extends cBaseParser {


    private final TSParser tsParser;

    public BashParser(String program) {
        super(program, null);

        this.tsParser = new TSParser();
        if (!this.tsParser.setLanguage(new TreeSitterBash())) {
            throw new IllegalStateException("Tree-sitter Bash grammar could not be loaded");
        }
    }

    // tree sitter
    public TSTree parseTS() {
        return this.tsParser.parseString(null, this.getProgram());
    }

    // parser program
    public List<AstNode> parse(TSTree tree) {
        TSNode root = tree.getRootNode();
        List<AstNode> stmts = new ArrayList<>();
        for(int i = 0; i < root.getChildCount(); i++) {
            cBaseParser parser = dispatcher(root.getChild(i));
            stmts.add(parser.parse(root.getChild(i)));
        }
        return stmts;
    }


    @Override
    public AstNode parse(TSNode node) {
        cParser parser = dispatcher(node);
        return parser.parse(node);
    }
}
