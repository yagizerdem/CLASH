package io.Clash.process.model;

import java.util.Objects;
import java.util.OptionalInt;

/** A resolved redirect whose target is a filename, descriptor, or inline input. */
public record Redirection(OptionalInt sourceDescriptor, RedirectOperator operator, String target) {
    public Redirection {
        Objects.requireNonNull(sourceDescriptor, "sourceDescriptor");
        Objects.requireNonNull(operator, "operator");
        Objects.requireNonNull(target, "target");
        if (sourceDescriptor.isPresent() && sourceDescriptor.getAsInt() < 0) {
            throw new IllegalArgumentException("File descriptor cannot be negative");
        }
    }

    public Redirection(RedirectOperator operator, String target) {
        this(OptionalInt.empty(), operator, target);
    }

    public Redirection(int sourceDescriptor, RedirectOperator operator, String target) {
        this(OptionalInt.of(sourceDescriptor), operator, target);
    }
}
