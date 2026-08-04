package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.CompoundStatementNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cCompoundStatementParser extends cBaseParser {

    public cCompoundStatementParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "compound_statement");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> elements = this.collectNamedChildren(tsNode);
        return new CompoundStatementNode(syntaxInfo, elements);
    }
}
