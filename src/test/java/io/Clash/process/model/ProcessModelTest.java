package io.Clash.process.model;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

class ProcessModelTest {
    @Test
    void representsMixedPipelineOperators() {
        var first = new SimpleCommand(List.of("printf", "hello"));
        var second = new SimpleCommand(List.of("grep", "hello"));
        var third = new SimpleCommand(List.of("wc", "-l"));

        var pipeline = new Pipeline(
                List.of(first, second, third),
                List.of(PipeMode.STDOUT, PipeMode.STDOUT_AND_STDERR));

        assertEquals(3, pipeline.stages().size());
        assertEquals(PipeMode.STDOUT_AND_STDERR, pipeline.pipes().get(1));
    }

    @Test
    void validatesPipelineShape() {
        var command = new SimpleCommand(List.of("true"));
        assertThrows(IllegalArgumentException.class,
                () -> new Pipeline(List.of(command, command), List.of()));
    }

    @Test
    void defensivelyCopiesArguments() {
        var args = new ArrayList<>(List.of("echo", "before"));
        var command = new SimpleCommand(args);
        args.set(1, "after");
        assertEquals(List.of("echo", "before"), command.getArgs());
    }

    @Test
    void allowsBackgroundAsTrailingListOperator() {
        var item = new CommandListItem(
                new SimpleCommand(List.of("job")), CommandOperator.BACKGROUND);
        var list = new CommandList(List.of(item));
        assertEquals(CommandOperator.BACKGROUND, list.items().getFirst().operatorAfter().orElseThrow());
    }
}
