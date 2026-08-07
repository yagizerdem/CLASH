package io.Clash.process.jvm;

import io.Clash.ClashException;
import io.Clash.ShellContext;
import io.Clash.lib.javaUtils.utils.FileUtilities;
import io.Clash.process.jvm.core.PathResolver;
import io.Clash.process.jvm.core.RedirectionClassifier;
import io.Clash.process.jvm.core.StreamGobbler;
import io.Clash.process.model.RedirectOperator;
import io.Clash.process.model.Redirection;
import io.Clash.process.model.SimpleCommand;
import io.Clash.process.model.base.ExecutionResponse;

import java.io.File;
import java.util.List;
import java.util.Objects;
import java.util.OptionalInt;

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
            ProcessBuilder pb =
                    new ProcessBuilder(model.getArgs());

            pb
            .inheritIO()
            .directory(new File(context.cwd));

            List<Redirection> redirections = model.getRedirections();
            PathResolver resolver = new PathResolver(context.cwd);

            for(int i = 0; i < redirections.size(); i++) {
                Redirection redirection = redirections.get(i);
                String target = redirection.target();
                String absolutePath = resolver.toAbsolutePath(target);
                boolean flag = FileUtilities.isValidFilePath(absolutePath);
                if(!flag) {
                    throw new ClashException(ClashException.ErrorType.PROCESS, "not valid file path");
                }

                File targetFile = new File(redirection.target());
                if(redirection.operator() == RedirectOperator.OUTPUT) {
                    pb.redirectOutput(targetFile);
                }
                else if(redirection.operator() == RedirectOperator.OUTPUT_APPEND) {
                    pb.redirectOutput(ProcessBuilder.Redirect.appendTo(targetFile));
                }
            }


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
            ProcessBuilder pb =
                    new ProcessBuilder(model.getArgs());


            pb.directory(new File(context.cwd));

            List<Redirection> redirections = model.getRedirections();
            PathResolver resolver = new PathResolver(context.cwd);


            boolean hasStdOutRedirection = false;
            boolean hasStdErrRedirection = false;


            for (int i = 0; i < redirections.size(); i++) {
                Redirection redirection = redirections.get(i);
                String target = redirection.target();
                String absolutePath = resolver.toAbsolutePath(target);
                boolean flag = FileUtilities.isValidFilePath(absolutePath);
                if (!flag) {
                    FileUtilities.createIfAbsent(absolutePath);
                }

                File targetFile = new File(redirection.target());
                if (redirection.operator() == RedirectOperator.OUTPUT) {
                    pb.redirectOutput(targetFile);
                } else if (redirection.operator() == RedirectOperator.OUTPUT_APPEND) {
                    pb.redirectOutput(ProcessBuilder.Redirect.appendTo(targetFile));
                }

                hasStdErrRedirection |= RedirectionClassifier.hasStdErrRedirection(redirection.operator(),
                        redirection.sourceDescriptor().isPresent() ? redirection.sourceDescriptor().getAsInt() : null);

                hasStdOutRedirection |= RedirectionClassifier.hasStdOutRedirection(redirection.operator(),
                        redirection.sourceDescriptor().isPresent() ? redirection.sourceDescriptor().getAsInt() : null);
            }

            Process process = pb.start();

            String stdOut = "";
            String stdErr = "";
            StreamGobbler outGobbler = null;
            StreamGobbler errGlobber = null;

            if (hasStdOutRedirection) {
                 outGobbler = new StreamGobbler(process.getInputStream());
                outGobbler.run();
            }

            if (hasStdErrRedirection) {
                errGlobber = new StreamGobbler(process.getErrorStream());
                errGlobber.run();
            }

            int exitCode = process.waitFor();

            if(hasStdOutRedirection) {
                stdOut = outGobbler.getResult();
            }

            if(hasStdErrRedirection) {
                stdErr = errGlobber.getResult();
            }

            return new ExecutionResponse(
                    stdOut,
                    stdErr,
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
