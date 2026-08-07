package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.Objects;

/** Bash {@code ! command}; its result status is inverted. */
public record NegatedCommand(BaseCommand command) implements BaseCommand {
    public NegatedCommand { Objects.requireNonNull(command, "command"); }
}
