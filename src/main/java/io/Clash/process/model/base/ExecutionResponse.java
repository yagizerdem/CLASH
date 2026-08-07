package io.Clash.process.model.base;

public class ExecutionResponse {

    public String stdout;
    public String stderr;
    public boolean isSuccess;
    public int exitCode;

    public ExecutionResponse() {
    }

    public ExecutionResponse(int exitCode) {
        this.exitCode = exitCode;
        this.isSuccess = exitCode == 0;
    }

    public ExecutionResponse(boolean isSuccess, int exitCode) {
        this.isSuccess = isSuccess;
        this.exitCode = exitCode;
    }

    public ExecutionResponse(
            String stdout,
            String stderr
    ) {
        this.stdout = stdout;
        this.stderr = stderr;
    }

    public ExecutionResponse(
            String stdout,
            String stderr,
            int exitCode
    ) {
        this.stdout = stdout;
        this.stderr = stderr;
        this.exitCode = exitCode;
        this.isSuccess = exitCode == 0;
    }

    public ExecutionResponse(
            String stdout,
            String stderr,
            boolean isSuccess,
            int exitCode
    ) {
        this.stdout = stdout;
        this.stderr = stderr;
        this.isSuccess = isSuccess;
        this.exitCode = exitCode;
    }

    @Override
    public String toString() {
        return "ExecutionResponse{" +
                "stdout='" + stdout + '\'' +
                ", stderr='" + stderr + '\'' +
                ", isSuccess=" + isSuccess +
                ", exitCode=" + exitCode +
                '}';
    }

    public String getStdout() {
        return this.stdout;
    }

    public String getStderr() {
        return this.stderr;
    }

}