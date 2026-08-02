package io.Clash.ast.base;

public record SourceSpan(int startByte, int endByte) {
    public SourceSpan {
        if (startByte < 0 || endByte < startByte) {
            throw new IllegalArgumentException("Invalid span: " + startByte + ".." + endByte);
        }
    }
}
