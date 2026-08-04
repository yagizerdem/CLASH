package io.Clash.parser.core;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.SourceSpan;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.parser.*;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public abstract class cBaseParser implements cParser {

    private final String program; // stores all program

    private final TSNode tsNode;

    public cBaseParser(String program, TSNode tsNode) {
        this.program = program;
        this.tsNode = tsNode;
    }

    public String getProgram() {
        return this.program;
    }

    public TSNode getTsNode() {
        return this.tsNode;
    }

    public String getProgramByOffsets() {
        return this.program.substring(tsNode.getStartByte(), tsNode.getEndByte());
    }

    public String getProgramByOffsets(TSNode tsNode) {
        return this.program.substring(tsNode.getStartByte(), tsNode.getEndByte());
    }

    public cBaseParser dispatcher(TSNode node) {
        return switch (node.getType()) {
            // primary-expr
            case "expansion" -> new cExpansionParser(program, node);
            case "command_substitution" -> new cCommandSubstitutionParser(program, node);
            case "number" -> new cNumberParser(program, node);
            case "simple_expansion" -> new cSimpleExpansionParser(program, node);
            case "string" -> new cStringParser(program, node);
            case "raw_string" -> new cRawStringParser(program, node);
            case "ansi_c_string" -> new cAnsiCStringParser(program, node);
            case "brace_expression" -> new cBraceExpressionParser(program, node);
            case "process_substitution" -> new cProcessSubstitutionParser(program, node);
            //expr
            case "word" -> new cWordParser(program, node);

            case "binary_expression" -> new cBinaryExpressionParser(program, node);
            // stmt



            // util
            case "special_variable_name" -> new cSpecialVariableNameParser(program, node);
            case "variable_name" -> new cVariableNameParser(program, node);
            case "regex" -> new cRegexParser(program, node);
            case "parenthesized_expression" -> new cParenthesizedExpressionParser(program, node);


            default -> throw new IllegalStateException("unknown stmt");
        };

    }

    public SyntaxInfo extractSyntaxInfo() {
        SourceSpan span = new SourceSpan(tsNode.getStartByte(), tsNode.getEndByte());
        String text = this.getProgramByOffsets();
        SyntaxInfo syntaxInfo = new SyntaxInfo(span, text);
        return syntaxInfo;
    }

    public SyntaxInfo extractSyntaxInfo(TSNode tsNode) {
        SourceSpan span = new SourceSpan(tsNode.getStartByte(), tsNode.getEndByte());
        String text = this.getProgramByOffsets();
        SyntaxInfo syntaxInfo = new SyntaxInfo(span, text);
        return syntaxInfo;
    }

    public void checkType(TSNode node, String tsTypeName) {
        if (!node.getType().equals(tsTypeName)) {
            throw new IllegalArgumentException(
                    "Expected node type '%s', but got '%s'"
                            .formatted(tsTypeName, node.getType())
            );
        }
    }

    public List<AstNode> collectNamedChildren(TSNode tsNode){
        List<AstNode> parts = new ArrayList<>();
        for(int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            cBaseParser parser = this.dispatcher(child);
            AstNode cNode =parser.parse(child);
            parts.add(cNode);
        }
        return parts;
    }

    public List<AstNode> collectNamedChildren(){
        List<AstNode> parts = new ArrayList<>();
        for(int i = 0; i < tsNode.getNamedChildCount(); i++) {
            TSNode child = tsNode.getNamedChild(i);
            cBaseParser parser = this.dispatcher(child);
            AstNode cNode =parser.parse(child);
            parts.add(cNode);
        }
        return parts;
    }

    public List<TSNode> getChildrenByFieldName(
            String fieldName
    ) {
        List<TSNode> result = new ArrayList<>();

        for (int i = 0; i < tsNode.getChildCount(); i++) {
            String currentFieldName = tsNode.getFieldNameForChild(i);

            if (fieldName.equals(currentFieldName)) {
                result.add(tsNode.getChild(i));
            }
        }

        return result;
    }

    public List<TSNode> getChildrenByFieldName(
            TSNode tsNode,
            String fieldName
    ) {
        List<TSNode> result = new ArrayList<>();

        for (int i = 0; i < tsNode.getChildCount(); i++) {
            String currentFieldName = tsNode.getFieldNameForChild(i);

            if (fieldName.equals(currentFieldName)) {
                result.add(tsNode.getChild(i));
            }
        }

        return result;
    }
}
