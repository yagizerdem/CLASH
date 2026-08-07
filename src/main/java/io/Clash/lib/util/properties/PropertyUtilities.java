package io.Clash.lib.util.properties;

import java.util.Objects;

/** Accessor for JVM system properties with an explicit fallback value. */
public final class PropertyUtilities {
    private PropertyUtilities() {
    }

    public static String getProperty(String key, String defaultValue) {
        return System.getProperty(Objects.requireNonNull(key, "key"), defaultValue);
    }
}
