package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.ProcessSubstitutionNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cProcessSubstitutionParser  extends cBaseParser {

    public cProcessSubstitutionParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "process_substitution");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> parts = this.collectNamedChildren(tsNode);
        return new ProcessSubstitutionNode(syntaxInfo, parts);
    }
}
