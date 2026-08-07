package io.Clash;

public class ClashException extends RuntimeException {

    public enum ErrorType {
        SYNTAX,
        SEMANTIC,
        PROCESS
    }

    private final ErrorType errorType;
    private final String fileName;
    private final int line;
    private final int column;

    public ClashException(
            ErrorType errorType,
            String message
    ) {
        this(errorType, message, null, null, -1, -1);
    }

    public ClashException(
            ErrorType errorType,
            String message,
            Throwable cause
    ) {
        this(errorType, message, cause, null, -1, -1);
    }

    public ClashException(
            ErrorType errorType,
            String message,
            int line,
            int column
    ) {
        this(errorType, message, null, null, line, column);
    }

    public ClashException(
            ErrorType errorType,
            String message,
            String fileName,
            int line,
            int column
    ) {
        this(errorType, message, null, fileName, line, column);
    }

    public ClashException(
            ErrorType errorType,
            String message,
            Throwable cause,
            String fileName,
            int line,
            int column
    ) {
        super(message, cause);

        this.errorType = errorType;
        this.fileName = fileName;
        this.line = line;
        this.column = column;
    }

    public ErrorType getErrorType() {
        return errorType;
    }

    public String getFileName() {
        return fileName;
    }

    public int getLine() {
        return line;
    }

    public int getColumn() {
        return column;
    }

    @Override
    public String getMessage() {
        return getFormattedMessage();
    }

    public String getFormattedMessage() {
        String severity = errorType.name().toLowerCase();
        String rawMessage = super.getMessage();

        if (!hasSourceLocation()) {
            return severity + " error: " + rawMessage;
        }

        return "%s:%d:%d: %s error: %s"
                .formatted(
                        fileName,
                        line,
                        column,
                        severity,
                        rawMessage
                );
    }

    public String getRawMessage() {
        return super.getMessage();
    }

    private boolean hasSourceLocation() {
        return fileName != null
                && line >= 0
                && column >= 0;
    }
}