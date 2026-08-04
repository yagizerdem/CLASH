package io.Clash.ast.base;

import java.util.List;
import java.util.Objects;

public abstract class StmtNode extends BaseAstNode {
    private final StatementNodeType type;

    protected StmtNode(SyntaxInfo syntax, StatementNodeType type) {
        super(Objects.requireNonNull(syntax, "syntax"));
        this.type = type;
    }


    public final StatementNodeType getType() {
        return this.type;
    }


    public static SyntaxInfo syntax(SyntaxInfo syntax) {
        return Objects.requireNonNull(syntax, "syntax");
    }

    public static <T> T required(T value, String name) {
        return Objects.requireNonNull(value, name);
    }

    public static <T> List<T> list(List<T> values, String name) {
        return List.copyOf(Objects.requireNonNull(values, name));
    }
}
