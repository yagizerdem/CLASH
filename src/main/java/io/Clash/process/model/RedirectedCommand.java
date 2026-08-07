package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.List;
import java.util.Objects;

/** Adds redirects to a compound command, subshell, or pipeline. */
public record RedirectedCommand(BaseCommand command, List<Redirection> redirections)
        implements BaseCommand {
    public RedirectedCommand {
        Objects.requireNonNull(command, "command");
        redirections = List.copyOf(Objects.requireNonNull(redirections, "redirections"));
        if (redirections.isEmpty()) throw new IllegalArgumentException("Redirections cannot be empty");
    }
}
