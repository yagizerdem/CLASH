package io.Clash.process.jvm;

import io.Clash.ClashException;
import io.Clash.ShellContext;
import io.Clash.process.jvm.core.StreamGobbler;
import io.Clash.process.model.SimpleCommand;
import io.Clash.process.model.base.ExecutionResponse;

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
        execute(simpleCommandModel, context);
    }

    public void exec(ShellContext context) {
        execute(simpleCommandModel, context);
    }

    public void exec(SimpleCommand command) {
        execute(command, context);
    }

    public void exec(SimpleCommand command, ShellContext context) {
        execute(command, context);
    }

    public ExecutionResponse execCaptured() {
        return execCaptured(simpleCommandModel, context);
    }

    public ExecutionResponse execCaptured(ShellContext context) {
        return execCaptured(simpleCommandModel, context);
    }

    public ExecutionResponse execCaptured(SimpleCommand command) {
        return execCaptured(command, context);
    }

    public ExecutionResponse execCaptured(SimpleCommand command, ShellContext context) {
        return executeCaptured(command, context);
    }

    public static void execute(SimpleCommand model, ShellContext context) {
        try {
            ProcessBuilder pb =
                    new ProcessBuilder(model.getArgs());

            pb
            .inheritIO()
            .directory(new File(context.cwd));

            Process process = pb.start();

            process.waitFor();
        }catch (Exception ex) {
            throw new ClashException(ClashException.ErrorType.PROCESS,
                    ex.getMessage(),
                    ex);
        }


    }

    public static ExecutionResponse executeCaptured(SimpleCommand model, ShellContext context) {
        try {
            ProcessBuilder pb =
                    new ProcessBuilder(model.getArgs());


            pb.directory(new File(context.cwd));



            Process process = pb.start();

            StreamGobbler outGobbler = new StreamGobbler(process.getInputStream());
            StreamGobbler errGlobber = new StreamGobbler(process.getErrorStream());


            outGobbler.run();
            errGlobber.run();

            int exitCode = process.waitFor();

            return new ExecutionResponse(
                    outGobbler.getResult(),
                    errGlobber.getResult(),
                    exitCode == 0,
                    exitCode
            );

        }catch (Exception ex) {
            throw new ClashException(ClashException.ErrorType.PROCESS,
                    ex.getMessage(),
                    ex);
        }
    }

}
