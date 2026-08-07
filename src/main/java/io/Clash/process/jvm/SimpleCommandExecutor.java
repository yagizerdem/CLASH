package io.Clash.process.jvm;

import io.Clash.ShellContext;
import io.Clash.process.model.SimpleCommand;
import io.Clash.process.model.base.BaseCommand;

import java.io.File;
import java.util.Objects;

public class SimpleCommandExecutor {
    private final SimpleCommand simpleCommandModel;
    private final ShellContext context;

    public SimpleCommandExecutor(SimpleCommand simpleCommandModel, ShellContext context) {
        this.simpleCommandModel = Objects.requireNonNull(simpleCommandModel, "simpleCommandModel");
        this.context = context;
    }

    public void exec() {
        exec(simpleCommandModel);
    }

    public void exec(SimpleCommand model) {

        ProcessBuilder pb =
                new ProcessBuilder(model.getArgs());
        pb.directory(new File(context.cwd));
        pb.

    }
}
