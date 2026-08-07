package io.Clash.process.jvm;

import io.Clash.ClashException;
import io.Clash.ShellContext;
import io.Clash.process.jvm.core.PathResolver;
import io.Clash.process.jvm.core.ProcessBuilderRedirection;
import io.Clash.process.jvm.core.StreamGobbler;
import io.Clash.process.model.Redirection;
import io.Clash.process.model.SimpleCommand;
import io.Clash.process.model.base.ExecutionResponse;

import java.io.File;
import java.util.List;
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

    public void execute(SimpleCommand model, ShellContext context) {
        try {
            ProcessBuilder pb = createProcessBuilder(model, context);
            pb.inheritIO();
            applyRedirections(pb, model.getRedirections(), context);
            Process process = pb.start();
            process.waitFor();
        }
        catch (ClashException ex) {
            throw ex;
        }
        catch (Exception ex) {
            throw new ClashException(ClashException.ErrorType.PROCESS,
                    ex.getMessage(),
                    ex);
        }


    }

    public ExecutionResponse executeCaptured(SimpleCommand model, ShellContext context) {
        try {
            ProcessBuilder pb = createProcessBuilder(model, context);
            applyRedirections(pb, model.getRedirections(), context);
            Process process = pb.start();
            process.getOutputStream().close();

            RunningGobbler outGobbler = startGobbler(
                    process,
                    pb.redirectOutput() == ProcessBuilder.Redirect.PIPE,
                    true
            );
            RunningGobbler errGobbler = startGobbler(
                    process,
                    !pb.redirectErrorStream()
                            && pb.redirectError() == ProcessBuilder.Redirect.PIPE,
                    false
            );
            int exitCode = process.waitFor();
            joinGobbler(outGobbler);
            joinGobbler(errGobbler);

            return new ExecutionResponse(
                    resultOf(outGobbler),
                    resultOf(errGobbler),
                    exitCode == 0,
                    exitCode
            );
        } catch (ClashException ex) {
            throw ex;
        } catch (Exception ex) {
            throw new ClashException(ClashException.ErrorType.PROCESS,
                    ex.getMessage(),
                    ex);
        }
    }

    private ProcessBuilder createProcessBuilder(SimpleCommand model, ShellContext context) {
        Objects.requireNonNull(model, "model");
        Objects.requireNonNull(context, "context");
        Objects.requireNonNull(context.cwd, "context.cwd");

        ProcessBuilder processBuilder = new ProcessBuilder(model.getArgs());
        processBuilder.directory(new File(context.cwd));
        processBuilder.environment().putAll(model.getEnvironment());
        return processBuilder;
    }

    private void applyRedirections(
            ProcessBuilder processBuilder,
            List<Redirection> redirections,
            ShellContext context
    ) {
        PathResolver resolver = new PathResolver(context.cwd);
        for (Redirection redirection : redirections) {
            ProcessBuilderRedirection.apply(processBuilder, redirection, resolver);
        }
    }

    private RunningGobbler startGobbler(Process process, boolean capture, boolean stdout) {
        if (!capture) {
            return null;
        }

        StreamGobbler gobbler = new StreamGobbler(
                stdout ? process.getInputStream() : process.getErrorStream());
        Thread thread = Thread.ofPlatform().daemon().start(gobbler);
        return new RunningGobbler(gobbler, thread);
    }

    private void joinGobbler(RunningGobbler gobbler) throws InterruptedException {
        if (gobbler == null) {
            return;
        }
        gobbler.thread().join();
    }

    private String resultOf(RunningGobbler gobbler) {
        return gobbler == null || gobbler.gobbler().getResult() == null
                ? ""
                : gobbler.gobbler().getResult();
    }

    private record RunningGobbler(StreamGobbler gobbler, Thread thread) {
    }

}
