package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.Objects;
import java.util.Optional;

/** A command and the operator that follows it, if one exists. */
public record CommandListItem(BaseCommand command, Optional<CommandOperator> operatorAfter) {
    public CommandListItem {
        Objects.requireNonNull(command, "command");
        Objects.requireNonNull(operatorAfter, "operatorAfter");
    }
    public CommandListItem(BaseCommand command, CommandOperator operatorAfter) {
        this(command, Optional.of(Objects.requireNonNull(operatorAfter, "operatorAfter")));
    }
    public CommandListItem(BaseCommand command) { this(command, Optional.empty()); }
}
