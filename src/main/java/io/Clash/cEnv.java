package io.Clash;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Hashtable;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Objects;
import java.util.Set;
import java.util.function.BiConsumer;

public class cEnv {

    public cEnv enclosing;

    public final Hashtable<String, cVariable> symbolTable = new Hashtable<>();

    public cEnv() {
        this(null);
    }

    public cEnv(cEnv enclosing) {
        this.enclosing = enclosing;
    }

    public cEnv createChild() {
        return new cEnv(this);
    }

    /** Adds a variable to this scope without replacing an existing local variable. */
    public boolean add(cVariable variable) {
        Objects.requireNonNull(variable, "variable");
        return add(requireIdentifier(variable.identifier), variable);
    }

    /** Adds a variable to this scope without replacing an existing local variable. */
    public boolean add(String identifier, cVariable variable) {
        String key = requireIdentifier(identifier);
        Objects.requireNonNull(variable, "variable");
        return symbolTable.putIfAbsent(key, variable) == null;
    }

    public boolean add(String identifier, String value) {
        String key = requireIdentifier(identifier);
        return add(key, new cVariable(key, value));
    }

    /** Inserts or replaces a variable in this scope and returns the previous value. */
    public cVariable put(cVariable variable) {
        Objects.requireNonNull(variable, "variable");
        return put(requireIdentifier(variable.identifier), variable);
    }

    /** Inserts or replaces a variable in this scope and returns the previous value. */
    public cVariable put(String identifier, cVariable variable) {
        String key = requireIdentifier(identifier);
        return symbolTable.put(key, Objects.requireNonNull(variable, "variable"));
    }

    public cVariable put(String identifier, String value) {
        String key = requireIdentifier(identifier);
        return put(key, new cVariable(key, value));
    }

    /** Replaces the nearest visible declaration. */
    public cVariable override(String identifier, cVariable variable) {
        String key = requireIdentifier(identifier);
        Objects.requireNonNull(variable, "variable");
        cEnv owner = findEnvironment(key);
        if (owner == null) {
            throw new NoSuchElementException("Undefined variable: " + key);
        }
        return owner.symbolTable.put(key, variable);
    }

    public cVariable override(String identifier, String value) {
        String key = requireIdentifier(identifier);
        return override(key, new cVariable(key, value));
    }

    public cVariable assign(String identifier, cVariable variable) {
        return override(identifier, variable);
    }

    public cVariable assign(String identifier, String value) {
        return override(identifier, value);
    }

    /** Returns the nearest visible variable, or null when it is undefined. */
    public cVariable get(String identifier) {
        String key = requireIdentifier(identifier);
        cEnv owner = findEnvironment(key);
        return owner == null ? null : owner.symbolTable.get(key);
    }

    public cVariable getLocal(String identifier) {
        return symbolTable.get(requireIdentifier(identifier));
    }

    public cVariable getOrDefault(String identifier, cVariable defaultValue) {
        cVariable variable = get(identifier);
        return variable == null ? defaultValue : variable;
    }

    public cVariable require(String identifier) {
        String key = requireIdentifier(identifier);
        cVariable variable = get(key);
        if (variable == null) {
            throw new NoSuchElementException("Undefined variable: " + key);
        }
        return variable;
    }

    public boolean contains(String identifier) {
        return findEnvironment(requireIdentifier(identifier)) != null;
    }

    public boolean has(String identifier) {
        return contains(identifier);
    }

    public boolean containsLocal(String identifier) {
        return symbolTable.containsKey(requireIdentifier(identifier));
    }

    public boolean hasLocal(String identifier) {
        return containsLocal(identifier);
    }

    /** Removes and returns the nearest visible variable. */
    public cVariable remove(String identifier) {
        String key = requireIdentifier(identifier);
        cEnv owner = findEnvironment(key);
        return owner == null ? null : owner.symbolTable.remove(key);
    }

    public cVariable removeLocal(String identifier) {
        return symbolTable.remove(requireIdentifier(identifier));
    }

    public int size() {
        return symbolTable.size();
    }

    /** Number of unique variables visible from this scope, including enclosing scopes. */
    public int visibleSize() {
        return visibleSymbols().size();
    }

    public boolean isEmpty() {
        return symbolTable.isEmpty();
    }

    public boolean isVisibleEmpty() {
        return visibleSize() == 0;
    }

    public void clear() {
        symbolTable.clear();
    }

    public void clearAll() {
        for (cEnv environment = this; environment != null; environment = environment.enclosing) {
            environment.symbolTable.clear();
        }
    }

    public int depth() {
        int depth = 0;
        for (cEnv environment = enclosing; environment != null; environment = environment.enclosing) {
            depth++;
        }
        return depth;
    }

    public cEnv findEnvironment(String identifier) {
        String key = requireIdentifier(identifier);
        for (cEnv environment = this; environment != null; environment = environment.enclosing) {
            if (environment.symbolTable.containsKey(key)) {
                return environment;
            }
        }
        return null;
    }

    public Set<String> keys() {
        return Set.copyOf(symbolTable.keySet());
    }

    public List<cVariable> values() {
        return Collections.unmodifiableList(new ArrayList<>(symbolTable.values()));
    }

    public Map<String, cVariable> snapshot() {
        return Collections.unmodifiableMap(new LinkedHashMap<>(symbolTable));
    }

    /** Snapshot of all visible symbols; inner declarations shadow outer declarations. */
    public Map<String, cVariable> visibleSymbols() {
        List<cEnv> scopes = new ArrayList<>();
        for (cEnv environment = this; environment != null; environment = environment.enclosing) {
            scopes.add(environment);
        }
        Collections.reverse(scopes);

        Map<String, cVariable> visible = new LinkedHashMap<>();
        for (cEnv scope : scopes) {
            visible.putAll(scope.symbolTable);
        }
        return Collections.unmodifiableMap(visible);
    }

    public void forEach(BiConsumer<String, cVariable> action) {
        symbolTable.forEach(Objects.requireNonNull(action, "action"));
    }

    private String requireIdentifier(String identifier) {
        Objects.requireNonNull(identifier, "identifier");
        if (identifier.isBlank()) {
            throw new IllegalArgumentException("identifier cannot be blank");
        }
        return identifier;
    }
}