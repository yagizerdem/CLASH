package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SourceSpan;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.primaryExpr.CommandSubstitutionNode;
import io.Clash.ast.primaryExpr.NumberNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cCommandSubstitutionParser extends cBaseParser {

    public cCommandSubstitutionParser(String program, TSNode node) {
        super(program, node);
    }


    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "command_substitution");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();

        TSNode tsRedirectNode = tsNode.getChildByFieldName("redirect");
        AstNode fileRedirectNode = this.isMissing(tsRedirectNode)
                ? null
                : this.parseChild(tsRedirectNode);
        List<AstNode> parts = this.collectNamedChildren(tsNode);

        return new CommandSubstitutionNode(syntaxInfo, parts, fileRedirectNode);
    }
}
