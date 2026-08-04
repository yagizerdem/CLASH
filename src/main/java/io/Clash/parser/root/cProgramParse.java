package io.Clash.parser.root;

import io.Clash.ast.ProgramNode;
import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.AnsiCStringNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cProgramParse extends cBaseParser {

    public cProgramParse(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "program");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> stmts = this.collectNamedChildren();
        return new ProgramNode(syntaxInfo, stmts);
    }
}
