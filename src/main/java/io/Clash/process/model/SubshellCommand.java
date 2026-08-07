package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.Objects;

/** A graph that must execute in an isolated subshell environment. */
public record SubshellCommand(BaseCommand body) implements BaseCommand {
    public SubshellCommand { Objects.requireNonNull(body, "body"); }
}
