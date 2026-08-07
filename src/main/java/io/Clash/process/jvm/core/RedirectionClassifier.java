package io.Clash.process.jvm.core;

import io.Clash.process.model.RedirectOperator;

public final class RedirectionClassifier {

    private RedirectionClassifier() {}

    public static boolean hasStdInRedirection(
            RedirectOperator operator,
            Integer sourceFd
    ) {
        return resolveSourceFd(operator, sourceFd) == 0;
    }

    public static boolean hasStdOutRedirection(
            RedirectOperator operator,
            Integer sourceFd
    ) {
        if (redirectsBothStdOutAndStdErr(operator)) {
            return true;
        }

        return resolveSourceFd(operator, sourceFd) == 1;
    }

    public static boolean hasStdErrRedirection(
            RedirectOperator operator,
            Integer sourceFd
    ) {
        if (redirectsBothStdOutAndStdErr(operator)) {
            return true;
        }

        return resolveSourceFd(operator, sourceFd) == 2;
    }

    public static boolean isInputOperator(RedirectOperator operator) {
        return switch (operator) {
            case INPUT,
                 INPUT_DUPLICATE,
                 INPUT_CLOSE,
                 HEREDOC,
                 HEREDOC_STRIP_TABS,
                 HERESTRING -> true;

            default -> false;
        };
    }

    public static boolean isOutputOperator(RedirectOperator operator) {
        return switch (operator) {
            case OUTPUT,
                 OUTPUT_DUPLICATE,
                 OUTPUT_CLOSE,
                 OUTPUT_APPEND,
                 OUTPUT_CLOBBER,
                 STDOUT_STDERR,
                 STDOUT_STDERR_APPEND -> true;

            default -> false;
        };
    }

    public static boolean redirectsBothStdOutAndStdErr(
            RedirectOperator operator
    ) {
        return operator == RedirectOperator.STDOUT_STDERR
                || operator == RedirectOperator.STDOUT_STDERR_APPEND;
    }

    public static int resolveSourceFd(
            RedirectOperator operator,
            Integer sourceFd
    ) {
        if (sourceFd != null) {
            return sourceFd;
        }

        return switch (operator) {
            case INPUT,
                 INPUT_DUPLICATE,
                 INPUT_CLOSE,
                 HEREDOC,
                 HEREDOC_STRIP_TABS,
                 HERESTRING -> 0;

            case OUTPUT,
                 OUTPUT_DUPLICATE,
                 OUTPUT_CLOSE,
                 OUTPUT_APPEND,
                 OUTPUT_CLOBBER -> 1;

            case STDOUT_STDERR,
                 STDOUT_STDERR_APPEND -> 1;
        };
    }
}