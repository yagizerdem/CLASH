package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;

import java.util.List;
import java.util.Map;
import java.util.Objects;

/** A single executable plus its already-expanded arguments and environment. */
public final class SimpleCommand implements BaseCommand {
    private final List<String> args;
    private final Map<String, String> environment;
    private final List<Redirection> redirections;

    public SimpleCommand(List<String> args) {
        this(args, Map.of(), List.of());
    }

    public SimpleCommand(List<String> args, List<Redirection> redirections) {
        this(args, Map.of(), redirections);
    }

    public SimpleCommand(List<String> args, Map<String, String> environment,
                         List<Redirection> redirections) {
        this.args = List.copyOf(Objects.requireNonNull(args, "args"));
        this.environment = Map.copyOf(Objects.requireNonNull(environment, "environment"));
        this.redirections = List.copyOf(Objects.requireNonNull(redirections, "redirections"));
        if (this.args.isEmpty()) {
            throw new IllegalArgumentException("A simple command must contain an executable");
        }
    }

    public String executable() { return args.getFirst(); }
    public List<String> getArgs() { return args; }
    public Map<String, String> getEnvironment() { return environment; }
    public List<Redirection> getRedirections() { return redirections; }
}
