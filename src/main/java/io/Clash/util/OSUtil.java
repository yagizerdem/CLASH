package io.Clash.util;

import java.util.Locale;

public final class OSUtil {

    public enum OSType {
        WINDOWS,
        LINUX,
        MACOS,
        FREEBSD,
        OPENBSD,
        NETBSD,
        DRAGONFLY_BSD,
        SOLARIS,
        AIX,
        HPUX,
        ANDROID,
        UNKNOWN
    }

    private static final String OS_NAME =
            System.getProperty("os.name", "unknown")
                    .toLowerCase(Locale.ROOT);

    private static final OSType OS_TYPE = detectOS();

    private OSUtil() {
    }

    public static OSType getOSType() {
        return OS_TYPE;
    }

    private static OSType detectOS() {
        if (OS_NAME.contains("android")) {
            return OSType.ANDROID;
        }

        if (OS_NAME.contains("win")) {
            return OSType.WINDOWS;
        }

        if (
                OS_NAME.contains("mac")
                        || OS_NAME.contains("darwin")
        ) {
            return OSType.MACOS;
        }

        if (OS_NAME.contains("dragonfly")) {
            return OSType.DRAGONFLY_BSD;
        }

        if (OS_NAME.contains("freebsd")) {
            return OSType.FREEBSD;
        }

        if (OS_NAME.contains("openbsd")) {
            return OSType.OPENBSD;
        }

        if (OS_NAME.contains("netbsd")) {
            return OSType.NETBSD;
        }

        if (
                OS_NAME.contains("sunos")
                        || OS_NAME.contains("solaris")
        ) {
            return OSType.SOLARIS;
        }

        if (OS_NAME.contains("aix")) {
            return OSType.AIX;
        }

        if (
                OS_NAME.contains("hp-ux")
                        || OS_NAME.contains("hpux")
        ) {
            return OSType.HPUX;
        }

        if (
                OS_NAME.contains("linux")
                        || OS_NAME.contains("nux")
                        || OS_NAME.contains("nix")
        ) {
            return OSType.LINUX;
        }

        return OSType.UNKNOWN;
    }

    public static boolean isWindows() {
        return OS_TYPE == OSType.WINDOWS;
    }

    public static boolean isLinux() {
        return OS_TYPE == OSType.LINUX;
    }

    public static boolean isMacOS() {
        return OS_TYPE == OSType.MACOS;
    }

    public static boolean isAndroid() {
        return OS_TYPE == OSType.ANDROID;
    }

    public static boolean isBSD() {
        return switch (OS_TYPE) {
            case FREEBSD,
                 OPENBSD,
                 NETBSD,
                 DRAGONFLY_BSD -> true;

            default -> false;
        };
    }

    public static boolean isUnixLike() {
        return switch (OS_TYPE) {
            case LINUX,
                 MACOS,
                 FREEBSD,
                 OPENBSD,
                 NETBSD,
                 DRAGONFLY_BSD,
                 SOLARIS,
                 AIX,
                 HPUX,
                 ANDROID -> true;

            default -> false;
        };
    }

    public static boolean isUnknown() {
        return OS_TYPE == OSType.UNKNOWN;
    }

    public static String getRawOSName() {
        return System.getProperty("os.name");
    }
}