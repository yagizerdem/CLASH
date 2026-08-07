package io.Clash.parser;

import io.Clash.ast.ProgramNode;
import io.Clash.ast.base.AstNode;
import io.Clash.parser.core.BashParser;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.treesitter.TSNode;
import org.treesitter.TSTree;

import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;

public class BashParsable {

    public AstNode tryParseFromFile(String testName, String resourcePath) {
        try {
            URL resource = getClass().getClassLoader().getResource(resourcePath);
            Assertions.assertNotNull(resource, "Resource file not found!");

            // Convert the resource URL to a file Path and read it
            Path path = Path.of(resource.toURI());
            String program = Files.readString(path);

            return tryParse(testName, program);
        }catch (Exception ex) {
            System.out.println(ex.getMessage());
            Assertions.fail("exception thrown at test: " + testName);
        }
        return null; // shout up java
    }

    public AstNode tryParse(String testName, String program) {
        try {
            BashParser parser = new BashParser(program);
            TSTree tree = parser.parseTS();
            AstNode ast = parser.parse(tree);

            return ast;
        }catch (Exception ex) {
            System.out.println(ex.getMessage());
            Assertions.fail("exception thrown at test: " + testName);
        }
        return null; // shout up java
    }


    @Test
    public void Basic()  {
        tryParseFromFile("Basic", "program/basic.txt");
    }

    @Test
    public void ControlFlow()  {
        tryParseFromFile("ControlFlow", "program/control_flow.txt");
    }

    @Test
    public void PipelinesAndRedirects()  {
        tryParseFromFile("PipelinesAndRedirects", "program/pipelines_and_redirects.txt");
    }

    @Test
    public void ExpansionsAndArrays()  {
        tryParseFromFile("ExpansionsAndArrays", "program/expansions_and_arrays.txt");
    }

    @Test
    public void CaseAndConditions()  {
        tryParseFromFile("CaseAndConditions", "program/case_and_conditions.txt");
    }

    @Test
    public void Heredocs()  {
        tryParseFromFile("Heredocs", "program/heredocs.txt");
    }

    @Test
    public void NestedStructures()  {
        tryParseFromFile("NestedStructures", "program/nested_structures.txt");
    }
}
