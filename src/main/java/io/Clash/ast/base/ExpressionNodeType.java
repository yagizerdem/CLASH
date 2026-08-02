package io.Clash.ast.base;

import io.Clash.ast.expr.*;
import io.Clash.ast.primaryExpr.*;

import java.util.Arrays;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;
import java.util.stream.Collectors;

public enum ExpressionNodeType {
    BINARY_EXPRESSION("binary_expression", BinaryExpressionNode.class, false),
    CONCATENATION("concatenation", ConcatenationNode.class, false),
    PARENTHESIZED_EXPRESSION("parenthesized_expression", ParenthesizedExpressionNode.class, false),
    POSTFIX_EXPRESSION("postfix_expression", PostfixExpressionNode.class, false),
    TERNARY_EXPRESSION("ternary_expression", TernaryExpressionNode.class, false),
    UNARY_EXPRESSION("unary_expression", UnaryExpressionNode.class, false),

    ANSI_C_STRING("ansi_c_string", AnsiCStringNode.class, true),
    ARITHMETIC_EXPANSION("arithmetic_expansion", ArithmeticExpansionNode.class, true),
    BRACE_EXPRESSION("brace_expression", BraceExpressionNode.class, true),
    COMMAND_SUBSTITUTION("command_substitution", CommandSubstitutionNode.class, true),
    EXPANSION("expansion", ExpansionNode.class, true),
    NUMBER("number", NumberNode.class, true),
    PROCESS_SUBSTITUTION("process_substitution", ProcessSubstitutionNode.class, true),
    RAW_STRING("raw_string", RawStringNode.class, true),
    SIMPLE_EXPANSION("simple_expansion", SimpleExpansionNode.class, true),
    STRING("string", StringNode.class, true),
    TRANSLATED_STRING("translated_string", TranslatedStringNode.class, true),
    WORD("word", WordNode.class, true);

    private static final Map<String, ExpressionNodeType> BY_TREE_SITTER_TYPE =
            Arrays.stream(values()).collect(Collectors.toUnmodifiableMap(
                    ExpressionNodeType::treeSitterType,
                    Function.identity()
            ));

    private final String treeSitterType;
    private final Class<? extends ExprNode> nodeClass;
    private final boolean primary;

    ExpressionNodeType(
            String treeSitterType,
            Class<? extends ExprNode> nodeClass,
            boolean primary
    ) {
        this.treeSitterType = treeSitterType;
        this.nodeClass = nodeClass;
        this.primary = primary;
    }

    public String treeSitterType() {
        return treeSitterType;
    }

    public Class<? extends ExprNode> nodeClass() {
        return nodeClass;
    }

    public boolean isPrimary() {
        return primary;
    }

    public static Optional<ExpressionNodeType> fromTreeSitterType(String type) {
        return Optional.ofNullable(BY_TREE_SITTER_TYPE.get(type));
    }
}
