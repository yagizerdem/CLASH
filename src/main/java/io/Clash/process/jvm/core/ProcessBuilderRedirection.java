package io.Clash.process.jvm.core;

import io.Clash.ClashException;
import io.Clash.process.model.RedirectOperator;
import io.Clash.process.model.Redirection;

import java.io.File;
import java.nio.file.Path;

/** Applies the standard descriptors that {@link ProcessBuilder} can represent. */
public final class ProcessBuilderRedirection {
    private ProcessBuilderRedirection() {
    }

    public static void apply(
            ProcessBuilder processBuilder,
            Redirection redirection,
            PathResolver pathResolver
    ) {
        int sourceDescriptor = RedirectionClassifier.resolveSourceFd(
                redirection.operator(),
                redirection.sourceDescriptor().isPresent()
                        ? redirection.sourceDescriptor().getAsInt()
                        : null
        );

        switch (redirection.operator()) {
            case INPUT -> redirectInput(processBuilder, redirection, pathResolver, sourceDescriptor);
            case OUTPUT, OUTPUT_CLOBBER -> redirectOutput(
                    processBuilder, redirection, pathResolver, sourceDescriptor, false);
            case OUTPUT_APPEND -> redirectOutput(
                    processBuilder, redirection, pathResolver, sourceDescriptor, true);
            case STDOUT_STDERR -> redirectOutputAndError(
                    processBuilder, redirection, pathResolver, false);
            case STDOUT_STDERR_APPEND -> redirectOutputAndError(
                    processBuilder, redirection, pathResolver, true);
            case OUTPUT_DUPLICATE -> duplicateOutput(processBuilder, redirection, sourceDescriptor);
            case INPUT_DUPLICATE -> throw unsupported(redirection,
                    "ProcessBuilder cannot support input duplication");
            case INPUT_CLOSE, OUTPUT_CLOSE -> throw unsupported(redirection,
                    "ProcessBuilder cannot close an individual file descriptor");
            case HEREDOC, HEREDOC_STRIP_TABS, HERESTRING -> throw unsupported(redirection,
                    "this redirection requires writing content to the process input stream");
        }
    }

    private static void redirectInput(
            ProcessBuilder processBuilder,
            Redirection redirection,
            PathResolver pathResolver,
            int sourceDescriptor
    ) {
        requireSourceDescriptor(redirection, sourceDescriptor, 0);
        processBuilder.redirectInput(resolveTarget(redirection, pathResolver));
    }

    private static void redirectOutput(
            ProcessBuilder processBuilder,
            Redirection redirection,
            PathResolver pathResolver,
            int sourceDescriptor,
            boolean append
    ) {
        if (sourceDescriptor != 1 && sourceDescriptor != 2) {
            throw unsupported(redirection, "output redirection only supports descriptors 1 and 2");
        }

        if (sourceDescriptor == 1 && processBuilder.redirectErrorStream()) {
            throw unsupported(redirection,
                    "ProcessBuilder cannot preserve an earlier stderr-to-stdout duplication "
                            + "when stdout is redirected afterwards");
        }

        File target = resolveTarget(redirection, pathResolver);
        ProcessBuilder.Redirect redirect = append
                ? ProcessBuilder.Redirect.appendTo(target)
                : ProcessBuilder.Redirect.to(target);

        if (sourceDescriptor == 1) {
            processBuilder.redirectOutput(redirect);
        } else {
            processBuilder.redirectErrorStream(false);
            processBuilder.redirectError(redirect);
        }
    }

    private static void redirectOutputAndError(
            ProcessBuilder processBuilder,
            Redirection redirection,
            PathResolver pathResolver,
            boolean append
    ) {
        File target = resolveTarget(redirection, pathResolver);
        processBuilder.redirectOutput(append
                ? ProcessBuilder.Redirect.appendTo(target)
                : ProcessBuilder.Redirect.to(target));
        processBuilder.redirectErrorStream(true);
    }

    private static void duplicateOutput(
            ProcessBuilder processBuilder,
            Redirection redirection,
            int sourceDescriptor
    ) {
        int targetDescriptor = requireTargetDescriptor(redirection);

        if (sourceDescriptor == targetDescriptor && (sourceDescriptor == 1 || sourceDescriptor == 2)) {
            return;
        }

        if (sourceDescriptor == 2 && targetDescriptor == 1) {
            processBuilder.redirectErrorStream(true);
            return;
        }

        throw unsupported(redirection,
                "ProcessBuilder only supports output descriptor duplication 2>&1");
    }


    private static File resolveTarget(Redirection redirection, PathResolver pathResolver) {
        if (redirection.target().isBlank()) {
            throw unsupported(redirection, "a file target is required");
        }
        return pathResolver.resolve(redirection.target()).toFile();
    }

    private static int requireTargetDescriptor(Redirection redirection) {
        if (redirection.targetDescriptor().isEmpty()) {
            throw unsupported(redirection, "a target descriptor is required");
        }
        return redirection.targetDescriptor().getAsInt();
    }

    private static void requireSourceDescriptor(
            Redirection redirection,
            int actual,
            int expected
    ) {
        if (actual != expected) {
            throw unsupported(redirection,
                    "operator " + redirection.operator().symbol()
                            + " only supports descriptor " + expected);
        }
    }

    private static ClashException unsupported(Redirection redirection, String reason) {
        return new ClashException(
                ClashException.ErrorType.PROCESS,
                "unsupported redirection '" + display(redirection) + "': " + reason
        );
    }

    private static String display(Redirection redirection) {
        String source = redirection.sourceDescriptor().isPresent()
                ? Integer.toString(redirection.sourceDescriptor().getAsInt())
                : "";
        String target = redirection.targetDescriptor().isPresent()
                ? Integer.toString(redirection.targetDescriptor().getAsInt())
                : redirection.target();
        return source + redirection.operator().symbol() + target;
    }
}
