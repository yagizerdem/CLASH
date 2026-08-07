package io.Clash.process.jvm;

import io.Clash.process.model.base.BaseCommand;

import java.util.Objects;

/** Placeholder runtime entry point for the evaluated command graph. */
public class CommandExecutor {
    private final BaseCommand commandModel;

    public CommandExecutor(BaseCommand commandModel) {
        this.commandModel = Objects.requireNonNull(commandModel, "commandModel");
    }

    public void execute() {
        exec(commandModel);
    }

    public void exec(BaseCommand model) {
        throw new UnsupportedOperationException(
                "JVM process execution is not implemented yet for: " + model);
    }
}
