package io.Clash.ast.base;

import java.util.Objects;

public record SyntaxInfo(SourceSpan span, String raw) {
    public SyntaxInfo {
        Objects.requireNonNull(span, "span");
        Objects.requireNonNull(raw, "raw");
    }
}
