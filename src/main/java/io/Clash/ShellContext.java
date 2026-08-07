package io.Clash;

public class ShellContext {

    public cEnv env;
    public String cwd;

    public ShellContext() {
    }

    public ShellContext(cEnv env) {
        this.env = env;
    }

    public ShellContext(String cwd) {
        this.cwd = cwd;
    }

    public ShellContext(cEnv env, String cwd) {
        this.env = env;
        this.cwd = cwd;
    }
}