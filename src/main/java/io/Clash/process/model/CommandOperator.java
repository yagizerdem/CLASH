package io.Clash.process.model;

/** Operator applied after a command in a Bash command list. */
public enum CommandOperator {
    SEQUENCE,
    BACKGROUND,
    LOGICAL_AND,
    LOGICAL_OR
}
