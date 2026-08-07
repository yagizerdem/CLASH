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

        if (sourceDescriptor.isPresent() && sourceDescriptor.getAsInt() < 0) {
            throw new IllegalArgumentException("Source descriptor cannot be negative");
        }

        if (targetDescriptor.isPresent() && targetDescriptor.getAsInt() < 0) {
            throw new IllegalArgumentException("Target descriptor cannot be negative");
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
}