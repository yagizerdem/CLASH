package io.Clash.parser;


import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cSpecialVariableNameParser extends cBaseParser {

    public cSpecialVariableNameParser(String program, TSNode node) {
        super(program, node);
    }

    public String parse(TSNode tsNode) {
        this.checkType(tsNode, "special_variable_name");
        String var = this.getProgramByOffsets();
        return var;
    }

}
