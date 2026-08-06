package io.Clash.parser;

import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.StmtNode;
import io.Clash.ast.base.SyntaxInfo;
import io.Clash.ast.enums.PipeOperator;
import io.Clash.ast.stmt.PipelineNode;
import io.Clash.parser.core.cBaseParser;
import org.treesitter.TSNode;

import java.util.ArrayList;
import java.util.List;

public class cPipelineParser extends cBaseParser {

    public cPipelineParser(String program, TSNode node) {
        super(program, node);
    }

    public AstNode parse(TSNode tsNode) {
        this.checkType(tsNode, "pipeline");
        SyntaxInfo syntaxInfo = this.extractSyntaxInfo();
        List<AstNode> commands = new ArrayList<>();
        List<PipeOperator> operators = new ArrayList<>();

        for (int i = 0; i < tsNode.getChildCount(); i++) {
            TSNode child = tsNode.getChild(i);
            if (child.isNamed()) {
                commands.add((StmtNode) this.parseChild(child));
            } else {
                String raw = this.getProgramByOffsets(child);
                if (raw.equals("|")) {
                    operators.add(PipeOperator.PIPE);
                } else if (raw.equals("|&")) {
                    operators.add(PipeOperator.PIPE_STDERR);
                }
            }
        }

        return new PipelineNode(syntaxInfo, commands, operators);
    }
}
