package io.Clash.ast.visitor;

import io.Clash.ast.ProgramNode;
import io.Clash.ast.base.ConcreteBaseAstNode;
import io.Clash.ast.expr.BinaryExpressionNode;
import io.Clash.ast.expr.ConcatenationNode;
import io.Clash.ast.expr.ParenthesizedExpressionNode;
import io.Clash.ast.expr.PostfixExpressionNode;
import io.Clash.ast.expr.TernaryExpressionNode;
import io.Clash.ast.expr.UnaryExpressionNode;
import io.Clash.ast.expr.WordNode;
import io.Clash.ast.other.ArrayNode;
import io.Clash.ast.other.CaseItemNode;
import io.Clash.ast.other.CommandNameNode;
import io.Clash.ast.other.CommentNode;
import io.Clash.ast.other.DoGroupNode;
import io.Clash.ast.other.ElifClauseNode;
import io.Clash.ast.other.ElseClauseNode;
import io.Clash.ast.other.ExtglobPatternNode;
import io.Clash.ast.other.FileDescriptorNode;
import io.Clash.ast.other.FileRedirectNode;
import io.Clash.ast.other.HeredocBodyNode;
import io.Clash.ast.other.HeredocContentNode;
import io.Clash.ast.other.HeredocEndNode;
import io.Clash.ast.other.HeredocRedirectNode;
import io.Clash.ast.other.HeredocStartNode;
import io.Clash.ast.other.HerestringRedirectNode;
import io.Clash.ast.other.RegexNode;
import io.Clash.ast.other.SpecialVariableNameNode;
import io.Clash.ast.other.StringContentNode;
import io.Clash.ast.other.SubscriptNode;
import io.Clash.ast.other.TestOperatorNode;
import io.Clash.ast.other.VariableNameNode;
import io.Clash.ast.primaryExpr.AnsiCStringNode;
import io.Clash.ast.primaryExpr.ArithmeticExpansionNode;
import io.Clash.ast.primaryExpr.BraceExpressionNode;
import io.Clash.ast.primaryExpr.CommandSubstitutionNode;
import io.Clash.ast.primaryExpr.ExpansionNode;
import io.Clash.ast.primaryExpr.NumberNode;
import io.Clash.ast.primaryExpr.ProcessSubstitutionNode;
import io.Clash.ast.primaryExpr.RawStringNode;
import io.Clash.ast.primaryExpr.SimpleExpansionNode;
import io.Clash.ast.primaryExpr.StringNode;
import io.Clash.ast.primaryExpr.TranslatedStringNode;
import io.Clash.ast.stmt.CStyleForStatementNode;
import io.Clash.ast.stmt.CaseStatementNode;
import io.Clash.ast.stmt.CommandNode;
import io.Clash.ast.stmt.CompoundStatementNode;
import io.Clash.ast.stmt.DeclarationCommandNode;
import io.Clash.ast.stmt.ForStatementNode;
import io.Clash.ast.stmt.FunctionDefinitionNode;
import io.Clash.ast.stmt.IfStatementNode;
import io.Clash.ast.stmt.ListNode;
import io.Clash.ast.stmt.NegatedCommandNode;
import io.Clash.ast.stmt.PipelineNode;
import io.Clash.ast.stmt.RedirectedStatementNode;
import io.Clash.ast.stmt.SubshellNode;
import io.Clash.ast.stmt.TestCommandNode;
import io.Clash.ast.stmt.UnsetCommandNode;
import io.Clash.ast.stmt.VariableAssignmentNode;
import io.Clash.ast.stmt.VariableAssignmentsNode;
import io.Clash.ast.stmt.WhileStatementNode;

public abstract class BaseVisitor implements Ivisitor {

    private UnsupportedOperationException notImplemented(String methodName) {
        return new UnsupportedOperationException(
                methodName + " is not implemented"
        );
    }

    // expr

    @Override
    public <T> T visitBinaryExpression(BinaryExpressionNode node) {
        throw notImplemented("visitBinaryExpression");
    }

    @Override
    public <T> T visitConcatenation(ConcatenationNode node) {
        throw notImplemented("visitConcatenation");
    }

    @Override
    public <T> T visitParenthesizedExpression(ParenthesizedExpressionNode node) {
        throw notImplemented("visitParenthesizedExpression");
    }

    @Override
    public <T> T visitPostfixExpression(PostfixExpressionNode node) {
        throw notImplemented("visitPostfixExpression");
    }

    @Override
    public <T> T visitTernaryExpression(TernaryExpressionNode node) {
        throw notImplemented("visitTernaryExpression");
    }

    @Override
    public <T> T visitUnaryExpression(UnaryExpressionNode node) {
        throw notImplemented("visitUnaryExpression");
    }

    @Override
    public <T> T visitWord(WordNode node) {
        throw notImplemented("visitWord");
    }

    // primary expr

    @Override
    public <T> T visitAnsiCString(AnsiCStringNode node) {
        throw notImplemented("visitAnsiCString");
    }

    @Override
    public <T> T visitArithmeticExpansion(ArithmeticExpansionNode node) {
        throw notImplemented("visitArithmeticExpansion");
    }

    @Override
    public <T> T visitBraceExpression(BraceExpressionNode node) {
        throw notImplemented("visitBraceExpression");
    }

    @Override
    public <T> T visitCommandSubstitution(CommandSubstitutionNode node) {
        throw notImplemented("visitCommandSubstitution");
    }

    @Override
    public <T> T visitExpansion(ExpansionNode node) {
        throw notImplemented("visitExpansion");
    }

    @Override
    public <T> T visitNumber(NumberNode node) {
        throw notImplemented("visitNumber");
    }

    @Override
    public <T> T visitProcessSubstitution(ProcessSubstitutionNode node) {
        throw notImplemented("visitProcessSubstitution");
    }

    @Override
    public <T> T visitRawString(RawStringNode node) {
        throw notImplemented("visitRawString");
    }

    @Override
    public <T> T visitSimpleExpansion(SimpleExpansionNode node) {
        throw notImplemented("visitSimpleExpansion");
    }

    @Override
    public <T> T visitString(StringNode node) {
        throw notImplemented("visitString");
    }

    @Override
    public <T> T visitTranslatedString(TranslatedStringNode node) {
        throw notImplemented("visitTranslatedString");
    }

    // stmt

    @Override
    public <T> T visitCStyleForStatement(CStyleForStatementNode node) {
        throw notImplemented("visitCStyleForStatement");
    }

    @Override
    public <T> T visitCaseStatement(CaseStatementNode node) {
        throw notImplemented("visitCaseStatement");
    }

    @Override
    public <T> T visitCommand(CommandNode node) {
        throw notImplemented("visitCommand");
    }

    @Override
    public <T> T visitCompoundStatement(CompoundStatementNode node) {
        throw notImplemented("visitCompoundStatement");
    }

    @Override
    public <T> T visitDeclarationCommand(DeclarationCommandNode node) {
        throw notImplemented("visitDeclarationCommand");
    }

    @Override
    public <T> T visitForStatement(ForStatementNode node) {
        throw notImplemented("visitForStatement");
    }

    @Override
    public <T> T visitFunctionDefinition(FunctionDefinitionNode node) {
        throw notImplemented("visitFunctionDefinition");
    }

    @Override
    public <T> T visitIfStatement(IfStatementNode node) {
        throw notImplemented("visitIfStatement");
    }

    @Override
    public <T> T visitList(ListNode node) {
        throw notImplemented("visitList");
    }

    @Override
    public <T> T visitNegatedCommand(NegatedCommandNode node) {
        throw notImplemented("visitNegatedCommand");
    }

    @Override
    public <T> T visitPipeline(PipelineNode node) {
        throw notImplemented("visitPipeline");
    }

    @Override
    public <T> T visitRedirectedStatement(RedirectedStatementNode node) {
        throw notImplemented("visitRedirectedStatement");
    }

    @Override
    public <T> T visitSubshell(SubshellNode node) {
        throw notImplemented("visitSubshell");
    }

    @Override
    public <T> T visitTestCommand(TestCommandNode node) {
        throw notImplemented("visitTestCommand");
    }

    @Override
    public <T> T visitUnsetCommand(UnsetCommandNode node) {
        throw notImplemented("visitUnsetCommand");
    }

    @Override
    public <T> T visitVariableAssignment(VariableAssignmentNode node) {
        throw notImplemented("visitVariableAssignment");
    }

    @Override
    public <T> T visitVariableAssignments(VariableAssignmentsNode node) {
        throw notImplemented("visitVariableAssignments");
    }

    @Override
    public <T> T visitWhileStatement(WhileStatementNode node) {
        throw notImplemented("visitWhileStatement");
    }

    // other

    @Override
    public <T> T visitArray(ArrayNode node) {
        throw notImplemented("visitArray");
    }

    @Override
    public <T> T visitCaseItem(CaseItemNode node) {
        throw notImplemented("visitCaseItem");
    }

    @Override
    public <T> T visitCommandName(CommandNameNode node) {
        throw notImplemented("visitCommandName");
    }

    @Override
    public <T> T visitComment(CommentNode node) {
        throw notImplemented("visitComment");
    }

    @Override
    public <T> T visitDoGroup(DoGroupNode node) {
        throw notImplemented("visitDoGroup");
    }

    @Override
    public <T> T visitElifClause(ElifClauseNode node) {
        throw notImplemented("visitElifClause");
    }

    @Override
    public <T> T visitElseClause(ElseClauseNode node) {
        throw notImplemented("visitElseClause");
    }

    @Override
    public <T> T visitExtglobPattern(ExtglobPatternNode node) {
        throw notImplemented("visitExtglobPattern");
    }

    @Override
    public <T> T visitFileDescriptor(FileDescriptorNode node) {
        throw notImplemented("visitFileDescriptor");
    }

    @Override
    public <T> T visitFileRedirect(FileRedirectNode node) {
        throw notImplemented("visitFileRedirect");
    }

    @Override
    public <T> T visitHeredocBody(HeredocBodyNode node) {
        throw notImplemented("visitHeredocBody");
    }

    @Override
    public <T> T visitHeredocContent(HeredocContentNode node) {
        throw notImplemented("visitHeredocContent");
    }

    @Override
    public <T> T visitHeredocEnd(HeredocEndNode node) {
        throw notImplemented("visitHeredocEnd");
    }

    @Override
    public <T> T visitHeredocRedirect(HeredocRedirectNode node) {
        throw notImplemented("visitHeredocRedirect");
    }

    @Override
    public <T> T visitHeredocStart(HeredocStartNode node) {
        throw notImplemented("visitHeredocStart");
    }

    @Override
    public <T> T visitHerestringRedirect(HerestringRedirectNode node) {
        throw notImplemented("visitHerestringRedirect");
    }

    @Override
    public <T> T visitRegex(RegexNode node) {
        throw notImplemented("visitRegex");
    }

    @Override
    public <T> T visitSpecialVariableName(SpecialVariableNameNode node) {
        throw notImplemented("visitSpecialVariableName");
    }

    @Override
    public <T> T visitStringContent(StringContentNode node) {
        throw notImplemented("visitStringContent");
    }

    @Override
    public <T> T visitSubscript(SubscriptNode node) {
        throw notImplemented("visitSubscript");
    }

    @Override
    public <T> T visitTestOperator(TestOperatorNode node) {
        throw notImplemented("visitTestOperator");
    }

    @Override
    public <T> T visitVariableName(VariableNameNode node) {
        throw notImplemented("visitVariableName");
    }

    @Override
    public <T> T visitProgram(ProgramNode node) {
        throw notImplemented("visitProgram");
    }

    @Override
    public <T> T visitConcreteBase(ConcreteBaseAstNode node) {
        throw notImplemented("visitConcreteBase");
    }
}