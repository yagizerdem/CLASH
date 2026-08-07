package io.Clash.process.jvm.core;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Objects;

public final class PathResolver {

    private final Path basePath;

    public PathResolver() {
        this(Path.of(System.getProperty("user.dir")));
    }

    public PathResolver(String basePath) {
        this(Path.of(Objects.requireNonNull(basePath, "basePath")));
    }

    public PathResolver(Path basePath) {
        this.basePath = Objects.requireNonNull(basePath, "basePath")
                .toAbsolutePath()
                .normalize();
    }

    public String toAbsolutePath(String path) {
        return resolve(path).toString();
    }

    public Path resolve(String path) {
        Objects.requireNonNull(path, "path");

        Path target = Path.of(path);

        if (target.isAbsolute()) {
            return target.normalize();
        }

        return basePath.resolve(target).normalize();
    }

    public Path resolve(Path path) {
        Objects.requireNonNull(path, "path");

        if (path.isAbsolute()) {
            return path.normalize();
        }

        return basePath.resolve(path).normalize();
    }

    public String normalize(String path) {
        Objects.requireNonNull(path, "path");
        return Path.of(path).normalize().toString();
    }

    public boolean isAbsolute(String path) {
        Objects.requireNonNull(path, "path");
        return Path.of(path).isAbsolute();
    }

    public boolean isRelative(String path) {
        return !isAbsolute(path);
    }

    public boolean exists(String path) {
        return Files.exists(resolve(path));
    }

    public boolean isFile(String path) {
        return Files.isRegularFile(resolve(path));
    }

    public boolean isDirectory(String path) {
        return Files.isDirectory(resolve(path));
    }

    public Path parent(String path) {
        Path resolved = resolve(path);
        return resolved.getParent();
    }

    public String fileName(String path) {
        Path fileName = resolve(path).getFileName();
        return fileName != null ? fileName.toString() : "";
    }

    public String extension(String path) {
        String fileName = fileName(path);

        int index = fileName.lastIndexOf('.');

        if (index <= 0 || index == fileName.length() - 1) {
            return "";
        }

        return fileName.substring(index + 1);
    }

    public String withoutExtension(String path) {
        String fileName = fileName(path);

        int index = fileName.lastIndexOf('.');

        if (index <= 0) {
            return fileName;
        }

        return fileName.substring(0, index);
    }

    public Path relativize(String path) {
        Path target = resolve(path);
        return basePath.relativize(target);
    }

    public Path getBasePath() {
        return basePath;
    }

    public PathResolver withBasePath(String newBasePath) {
        return new PathResolver(newBasePath);
    }

    public PathResolver withBasePath(Path newBasePath) {
        return new PathResolver(newBasePath);
    }
}