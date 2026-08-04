package io.Clash.ast.base;

import java.util.List;
import java.util.Objects;

public abstract class ExprNode extends BaseAstNode {
    private final ExpressionNodeType type;

    protected ExprNode(SyntaxInfo syntax, ExpressionNodeType type) {
        super(Objects.requireNonNull(syntax, "syntax"));
        this.type = Objects.requireNonNull(type, "type");
    }

    public final ExpressionNodeType getType() {
        return type;
    }



    public static <T> T required(T value, String name) {
        return Objects.requireNonNull(value, name);
    }

    public static <T> List<T> list(List<T> values, String name) {
        return List.copyOf(Objects.requireNonNull(values, name));
    }
}
