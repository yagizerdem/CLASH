package io.Clash.parser.core;

import io.Clash.ast.base.*;
import io.Clash.parser.*;
import io.Clash.parser.root.cProgramParse;
import org.treesitter.TSNode;

import java.nio.charset.StandardCharsets;
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
        return this.getProgramByOffsets(tsNode);
    }

    public String getProgramByOffsets(TSNode tsNode) {
        byte[] utf8 = this.program.getBytes(StandardCharsets.UTF_8);
        int start = tsNode.getStartByte();
        int end = tsNode.getEndByte();
        return new String(utf8, start, end - start, StandardCharsets.UTF_8);
    }

    public cBaseParser dispatcher(TSNode node) {
        if (this.isMissing(node)) {
            throw new IllegalArgumentException("Cannot dispatch a missing Tree-sitter node");
        }

        return switch (node.getType()) {
            // root
            case "program" -> new cProgramParse(program, node);

            // primary-expr
            case "expansion" -> new cExpansionParser(program, node);
            case "arithmetic_expansion" -> new cArithmeticExpansionParser(program, node);
            case "command_substitution" -> new cCommandSubstitutionParser(program, node);
            case "number" -> new cNumberParser(program, node);
            case "simple_expansion" -> new cSimpleExpansionParser(program, node);
            case "string" -> new cStringParser(program, node);
            case "raw_string" -> new cRawStringParser(program, node);
            case "ansi_c_string" -> new cAnsiCStringParser(program, node);
            case "brace_expression" -> new cBraceExpressionParser(program, node);
            case "process_substitution" -> new cProcessSubstitutionParser(program, node);
            case "translated_string" -> new cTranslatedStringParser(program, node);

            //expr
            case "word" -> new cWordParser(program, node);
            case "binary_expression" -> new cBinaryExpressionParser(program, node);
            case "concatenation" -> new cConcatenationParser(program, node);
            case "parenthesized_expression" -> new cParenthesizedExpressionParser(program, node);
            case "postfix_expression" -> new cPostfixExpressionParser(program, node);
            case "ternary_expression" -> new cTernaryExpressionParser(program, node);
            case "unary_expression" -> new cUnaryExpressionParser(program, node);

            // stmt
            case "c_style_for_statement" -> new cCStyleForStatementParser(program, node);
            case "case_statement" -> new cCaseStatementParser(program, node);
            case "command" -> new cCommandParser(program, node);
            case "compound_statement" -> new cCompoundStatementParser(program, node);
            case "declaration_command" -> new cDeclarationCommandParser(program, node);
            case "for_statement" -> new cForStatementParser(program, node);
            case "function_definition" -> new cFunctionDefinitionParser(program, node);
            case "if_statement" -> new cIfStatementParser(program, node);
            case "list" -> new cListParser(program, node);
            case "negated_command" -> new cNegatedCommandParser(program, node);
            case "pipeline" -> new cPipelineParser(program, node);
            case "redirected_statement" -> new cRedirectedStatementParser(program, node);
            case "subshell" -> new cSubshellParser(program, node);
            case "test_command" -> new cTestCommandParser(program, node);
            case "unset_command" -> new cUnsetCommandParser(program, node);
            case "variable_assignment" -> new cVariableAssignmentParser(program, node);
            case "variable_assignments" -> new cVariableAssignmentsParser(program, node);
            case "while_statement" -> new cWhileStatementParser(program, node);

            // other
            case "array" -> new cArrayParser(program, node);
            case "comment" -> new cCommentParser(program, node);
            case "case_item" -> new cCaseItemParser(program, node);
            case "command_name" -> new cCommandNameParser(program, node);
            case "do_group" -> new cDoGroupParser(program, node);
            case "elif_clause" -> new cElifClauseParser(program, node);
            case "else_clause" -> new cElseClauseParser(program, node);
            case "extglob_pattern" -> new cExtglobPatternParser(program, node);
            case "file_descriptor" -> new cFileDescriptorParser(program, node);
            case "file_redirect" -> new cFileRedirectParser(program, node);
            case "heredoc_body" -> new cHeredocBodyParser(program, node);
            case "heredoc_content" -> new cHeredocContentParser(program, node);
            case "heredoc_end" -> new cHeredocEndParser(program, node);
            case "heredoc_redirect" -> new cHeredocRedirectParser(program, node);
            case "heredoc_start" -> new cHeredocStartParser(program, node);
            case "herestring_redirect" -> new cHerestringRedirectParser(program, node);
            case "string_content" -> new cStringContentParser(program, node);
            case "subscript" -> new cSubscriptParser(program, node);
            case "test_operator" -> new cTestOperatorParser(program, node);
            case "special_variable_name" -> new cSpecialVariableNameParser(program, node);
            case "variable_name" -> new cVariableNameParser(program, node);
            case "regex" -> new cRegexParser(program, node);


            default -> throw new IllegalStateException("unknown tree sitter node type : " + node.getType());
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
        String text = this.getProgramByOffsets(tsNode);
        SyntaxInfo syntaxInfo = new SyntaxInfo(span, text);
        return syntaxInfo;
    }

    public void checkType(TSNode node, String tsTypeName) {
        if (this.isMissing(node)) {
            throw new IllegalArgumentException("Expected node type '" + tsTypeName + "', but node is missing");
        }

        if (!node.getType().equals(tsTypeName)) {
            throw new IllegalArgumentException(
                    "Expected node type '%s', but got '%s'"
                            .formatted(tsTypeName, node.getType())
            );
        }
    }

    public boolean isMissing(TSNode node) {
        return node == null || node.isNull();
    }

    public AstNode parseChild(TSNode node) {
        if (this.isMissing(node)) {
            throw new IllegalArgumentException("Cannot parse a missing Tree-sitter child node");
        }

        if (node.isNamed()) {
            return this.dispatcher(node).parse(node);
        }

        return new ConcreteBaseAstNode(this.extractSyntaxInfo(node));
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
