package io.Clash.process.model;

import io.Clash.process.model.base.BaseCommand;
import java.util.List;
import java.util.Objects;

/** Commands joined by {@code ;}, {@code &}, {@code &&}, or {@code ||}. */
public record CommandList(List<CommandListItem> items) implements BaseCommand {
    public CommandList {
        items = List.copyOf(Objects.requireNonNull(items, "items"));
        if (items.isEmpty()) throw new IllegalArgumentException("A command list cannot be empty");
        for (int i = 0; i < items.size() - 1; i++) {
            if (items.get(i).operatorAfter().isEmpty()) {
                throw new IllegalArgumentException("Every non-final item needs an operator");
            }
        }
        var tail = items.getLast().operatorAfter();
        if (tail.isPresent() && tail.get() != CommandOperator.SEQUENCE
                && tail.get() != CommandOperator.BACKGROUND) {
            throw new IllegalArgumentException("Only ';' or '&' may terminate a command list");
        }
    }
}
