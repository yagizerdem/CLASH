package io.Clash.process.model;

import java.util.List;
import java.util.Objects;

public class Command {
    private final List<String> args;

    public Command(List<String> args) {
        this.args = Objects.requireNonNull(args);
    }

    public List<String> getArgs() {
        return this.args;
    }

}
