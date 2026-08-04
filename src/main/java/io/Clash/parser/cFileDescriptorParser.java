package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.other.FileDescriptorNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

public class cFileDescriptorParser extends cBaseParser {
    public cFileDescriptorParser(String program, TSNode node) { super(program, node); }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "file_descriptor");
        return new FileDescriptorNode(this.extractSyntaxInfo());
    }
}
