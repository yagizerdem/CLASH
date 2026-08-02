package io.Clash.ast.base;

import io.Clash.ast.stmt.*;

import java.util.Arrays;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;
import java.util.stream.Collectors;

public enum StatementNodeType {
    C_STYLE_FOR_STATEMENT("c_style_for_statement", CStyleForStatementNode.class),
    CASE_STATEMENT("case_statement", CaseStatementNode.class),
    COMMAND("command", CommandNode.class),
    COMPOUND_STATEMENT("compound_statement", CompoundStatementNode.class),
    DECLARATION_COMMAND("declaration_command", DeclarationCommandNode.class),
    FOR_STATEMENT("for_statement", ForStatementNode.class),
    FUNCTION_DEFINITION("function_definition", FunctionDefinitionNode.class),
    IF_STATEMENT("if_statement", IfStatementNode.class),
    LIST("list", ListNode.class),
    NEGATED_COMMAND("negated_command", NegatedCommandNode.class),
    PIPELINE("pipeline", PipelineNode.class),
    REDIRECTED_STATEMENT("redirected_statement", RedirectedStatementNode.class),
    SUBSHELL("subshell", SubshellNode.class),
    TEST_COMMAND("test_command", TestCommandNode.class),
    UNSET_COMMAND("unset_command", UnsetCommandNode.class),
    VARIABLE_ASSIGNMENT("variable_assignment", VariableAssignmentNode.class),
    VARIABLE_ASSIGNMENTS("variable_assignments", VariableAssignmentsNode.class),
    WHILE_STATEMENT("while_statement", WhileStatementNode.class);

    private static final Map<String, StatementNodeType> BY_TREE_SITTER_TYPE =
            Arrays.stream(values()).collect(Collectors.toUnmodifiableMap(
                    StatementNodeType::treeSitterType,
                    Function.identity()
            ));

    private final String treeSitterType;
    private final Class<? extends StmtNode> nodeClass;

    StatementNodeType(String treeSitterType, Class<? extends StmtNode> nodeClass) {
        this.treeSitterType = treeSitterType;
        this.nodeClass = nodeClass;
    }

    public String treeSitterType() {
        return treeSitterType;
    }

    public Class<? extends StmtNode> nodeClass() {
        return nodeClass;
    }

    public static Optional<StatementNodeType> fromTreeSitterType(String type) {
        return Optional.ofNullable(BY_TREE_SITTER_TYPE.get(type));
    }
}
