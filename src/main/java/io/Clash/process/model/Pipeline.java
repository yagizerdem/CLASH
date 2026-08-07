package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.List;
import java.util.Objects;

/** A Bash pipeline such as {@code producer | filter |& consumer}. */
public record Pipeline(List<BaseCommand> stages, List<PipeMode> pipes) implements BaseCommand {
    public Pipeline {
        stages = List.copyOf(Objects.requireNonNull(stages, "stages"));
        pipes = List.copyOf(Objects.requireNonNull(pipes, "pipes"));
        if (stages.size() < 2) throw new IllegalArgumentException("A pipeline needs at least two stages");
        if (pipes.size() != stages.size() - 1) {
            throw new IllegalArgumentException("A pipeline needs one pipe between each stage");
        }
    }
}
