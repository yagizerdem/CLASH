package io.Clash.lib.util.context;

import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;

/** Thread-safe application context used by the utility classes. */
public final class ContextStore {
    private static final Map<String, Object> VALUES = new ConcurrentHashMap<>();

    private ContextStore() {
    }

    public static void put(String key, Object value) {
        VALUES.put(Objects.requireNonNull(key, "key"), Objects.requireNonNull(value, "value"));
    }

    @SuppressWarnings("unchecked")
    public static <T> T get(String key) {
        return (T) VALUES.get(Objects.requireNonNull(key, "key"));
    }

    @SuppressWarnings("unchecked")
    public static <T> T get(String key, T defaultValue) {
        return (T) VALUES.getOrDefault(Objects.requireNonNull(key, "key"), defaultValue);
    }

    public static void remove(String key) {
        VALUES.remove(Objects.requireNonNull(key, "key"));
    }

    public static void clear() {
        VALUES.clear();
    }
}
