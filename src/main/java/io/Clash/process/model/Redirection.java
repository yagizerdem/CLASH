package io.Clash.process.model;

import java.util.Objects;
import java.util.OptionalInt;

public record Redirection(
        OptionalInt sourceDescriptor,
        RedirectOperator operator,
        String target,
        OptionalInt targetDescriptor
) {
    public Redirection {
        Objects.requireNonNull(sourceDescriptor, "sourceDescriptor");
        Objects.requireNonNull(operator, "operator");
        Objects.requireNonNull(target, "target");
        Objects.requireNonNull(targetDescriptor, "targetDescriptor");

        if (sourceDescriptor.isPresent() && !isStandardDescriptor(sourceDescriptor.getAsInt())) {
            throw new IllegalArgumentException("Source descriptor must be 0, 1, or 2");
        }

        if (targetDescriptor.isPresent() && !isStandardDescriptor(targetDescriptor.getAsInt())) {
            throw new IllegalArgumentException("Target descriptor must be 0, 1, or 2");
        }
    }

    public Redirection(RedirectOperator operator, String target) {
        this(
                OptionalInt.empty(),
                operator,
                target,
                OptionalInt.empty()
        );
    }

    public Redirection(
            int sourceDescriptor,
            RedirectOperator operator,
            String target
    ) {
        this(
                OptionalInt.of(sourceDescriptor),
                operator,
                target,
                OptionalInt.empty()
        );
    }

    public Redirection(
            int sourceDescriptor,
            RedirectOperator operator,
            int targetDescriptor
    ) {
        this(
                OptionalInt.of(sourceDescriptor),
                operator,
                "",
                OptionalInt.of(targetDescriptor)
        );
    }

    private static boolean isStandardDescriptor(int descriptor) {
        return descriptor >= 0 && descriptor <= 2;
    }
}
