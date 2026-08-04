package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.stmt.DeclarationCommandNode;
import io.Clash.ast.stmt.DeclarationKind;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.List;

public class cDeclarationCommandParser extends cBaseParser {

    public cDeclarationCommandParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "declaration_command");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        DeclarationKind kind = this.parseKind(syntaxInfo.raw());
        List<AstNode> arguments = this.collectNamedChildren(tsNode);
        return new DeclarationCommandNode(syntaxInfo, kind, arguments);
    }

    private DeclarationKind parseKind(String raw) {
        String keyword = raw.stripLeading().split("\\s+", 2)[0];
        return switch (keyword) {
            case "declare" -> DeclarationKind.DECLARE;
            case "export" -> DeclarationKind.EXPORT;
            case "local" -> DeclarationKind.LOCAL;
            case "readonly" -> DeclarationKind.READONLY;
            case "typeset" -> DeclarationKind.TYPESET;
            default -> throw new IllegalStateException("Unknown declaration kind: " + keyword);
        };
    }
}
