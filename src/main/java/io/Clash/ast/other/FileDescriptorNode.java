package io.Clash.ast.other;

import io.Clash.ast.visitor.Ivisitor;

import io.Clash.ast.base.BaseAstNode;
import io.Clash.ast.base.SyntaxInfo;

public final class FileDescriptorNode extends BaseAstNode {
    public FileDescriptorNode(SyntaxInfo syntax) {
        super(syntax);
    }

    @Override
    public <T> T visit(Ivisitor visitor) {
        return visitor.visitFileDescriptor(this);
    }
}
