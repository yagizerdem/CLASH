package io.Clash.parser;

import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cVariableName extends cBaseParser {

    public cVariableName(String program, TSNode node) {
        super(program, node);
    }

    public String parse(TSNode tsNode) {
        this.checkType(tsNode, "variable_name");
        String var = this.getProgramByOffsets();
        return var;
    }

}
