package io.Clash.parser.core;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SourceSpan;
import io.Clash.ast.base.SyntaxInfo;
import org.treesitter.*;

public class BashParser {

    private final String program;

    private final TSParser tsParser;

    public BashParser(String program) {
        this.program = program;

        this.tsParser = new TSParser();
        if (!this.tsParser.setLanguage(new TreeSitterBash())) {
            throw new IllegalStateException("Tree-sitter Bash grammar could not be loaded");
        }
    }

    // tree sitter
    public TSTree parseTS() {
        return this.tsParser.parseString(null, program);
    }

    // custom parser
    public void parse(TSTree tree) {
        TSNode root = tree.getRootNode();
        parseNode(root);
    }

    public AstNode parseNode(TSNode node) {
        return switch (node.getType()) {
            // primary-expr
            case "arithmetic_expansion" -> parsearithmetic_expansion(node);

            // stmt
            case "c_style_for_statement" -> parsec_style_for_statement(node);
            default -> throw new IllegalStateException("unknown stmt");
        };
    }

    // primary-expr parser
    public AstNode parsearithmetic_expansion(TSNode node) {

    }


    // stmt parser

    public AstNode parsec_style_for_statement(TSNode node) {
        SourceSpan span = new SourceSpan(node.getStartByte(), node.getEndByte());
        SyntaxInfo syntaxInfo = new SyntaxInfo(span, this.program.substring(span.startByte(), span.endByte()));

//        CStyleForStatementNode stmt = new CStyleForStatementNode();

        return null;
    }


}
