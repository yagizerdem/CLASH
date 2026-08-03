package io.Clash.parser.core;

import io.Clash.ast.base.AstNode;
import org.treesitter.TSNode;

public interface cParser {

    void checkType(TSNode tsNode, String tsTypeName);
    <T> T parse(TSNode tsNode);
}
