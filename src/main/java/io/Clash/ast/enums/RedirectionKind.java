package io.Clash.ast.enums;

public enum RedirectionKind {

    INPUT("<"),
    INPUT_DUPLICATE("<&"),
    INPUT_CLOSE("<&-"),

    HEREDOC("<<"),
    HEREDOC_STRIP_TABS("<<-"),
    HERESTRING("<<<"),

    OUTPUT(">"),
    OUTPUT_DUPLICATE(">&"),
    OUTPUT_CLOSE(">&-"),

    OUTPUT_APPEND(">>"),
    OUTPUT_CLOBBER(">|"),

    STDOUT_STDERR("&>"),
    STDOUT_STDERR_APPEND("&>>");

    private final String symbol;

    RedirectionKind(String symbol) {
        this.symbol = symbol;
    }

    public String symbol() {
        return symbol;
    }

    public static RedirectionKind fromSymbol(String symbol) {
        for (RedirectionKind operator : values()) {
            if (operator.symbol.equals(symbol)) {
                return operator;
            }
        }

        throw new IllegalArgumentException(
                "Unknown redirection operator: " + symbol
        );
    }
}