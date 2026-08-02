package io.Clash.ast.base;

import java.util.List;
import java.util.Objects;

public abstract class ExprNode implements AstNode {
    private final SyntaxInfo syntax;
    private final ExpressionNodeType type;

    protected ExprNode(SyntaxInfo syntax, ExpressionNodeType type) {
        this.syntax = Objects.requireNonNull(syntax, "syntax");
        this.type = Objects.requireNonNull(type, "type");
    }

    public final ExpressionNodeType getType() {
        return type;
    }

    @Override
    public final SyntaxInfo getSyntax() {
        return syntax;
    }

    public static <T> T required(T value, String name) {
        return Objects.requireNonNull(value, name);
    }

    public static <T> List<T> list(List<T> values, String name) {
        return List.copyOf(Objects.requireNonNull(values, name));
    }
}
