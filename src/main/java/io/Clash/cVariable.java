package io.Clash;

public class cVariable {

    public String identifier;
    public String value;

    public cVariable() {
    }

    public cVariable(String identifier) {
        this.identifier = identifier;
    }

    public cVariable(String identifier, String value) {
        this.identifier = identifier;
        this.value = value;
    }
}