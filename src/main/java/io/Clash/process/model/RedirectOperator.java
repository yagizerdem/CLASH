package io.Clash.process.model;

/** Process-layer equivalent of Bash redirection operators. */
public enum RedirectOperator {
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
    RedirectOperator(String symbol) { this.symbol = symbol; }
    public String symbol() { return symbol; }

    public static RedirectOperator fromSymbol(String symbol) {
        for (RedirectOperator operator : values()) {
            if (operator.symbol.equals(symbol)) return operator;
        }
        throw new IllegalArgumentException("Unknown redirection operator: " + symbol);
    }
}
