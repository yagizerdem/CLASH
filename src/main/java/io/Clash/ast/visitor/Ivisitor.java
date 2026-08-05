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

public interface Ivisitor {

    // expr
    public <T> T visitBinaryExpression(BinaryExpressionNode node);

    public <T> T visitConcatenation(ConcatenationNode node);

    public <T> T visitParenthesizedExpression(ParenthesizedExpressionNode node);

    public <T> T visitPostfixExpression(PostfixExpressionNode node);

    public <T> T visitTernaryExpression(TernaryExpressionNode node);

    public <T> T visitUnaryExpression(UnaryExpressionNode node);

    public <T> T visitWord(WordNode node);

    // primary expr

    public <T> T visitAnsiCString(AnsiCStringNode node);

    public <T> T visitArithmeticExpansion(ArithmeticExpansionNode node);

    public <T> T visitBraceExpression(BraceExpressionNode node);

    public <T> T visitCommandSubstitution(CommandSubstitutionNode node);

    public <T> T visitExpansion(ExpansionNode node);

    public <T> T visitNumber(NumberNode node);

    public <T> T visitProcessSubstitution(ProcessSubstitutionNode node);

    public <T> T visitRawString(RawStringNode node);

    public <T> T visitSimpleExpansion(SimpleExpansionNode node);

    public <T> T visitString(StringNode node);

    public <T> T visitTranslatedString(TranslatedStringNode node);

    // stmt

    public <T> T visitCStyleForStatement(CStyleForStatementNode node);

    public <T> T visitCaseStatement(CaseStatementNode node);

    public <T> T visitCommand(CommandNode node);

    public <T> T visitCompoundStatement(CompoundStatementNode node);

    public <T> T visitDeclarationCommand(DeclarationCommandNode node);

    public <T> T visitForStatement(ForStatementNode node);

    public <T> T visitFunctionDefinition(FunctionDefinitionNode node);

    public <T> T visitIfStatement(IfStatementNode node);

    public <T> T visitList(ListNode node);

    public <T> T visitNegatedCommand(NegatedCommandNode node);

    public <T> T visitPipeline(PipelineNode node);

    public <T> T visitRedirectedStatement(RedirectedStatementNode node);

    public <T> T visitSubshell(SubshellNode node);

    public <T> T visitTestCommand(TestCommandNode node);

    public <T> T visitUnsetCommand(UnsetCommandNode node);

    public <T> T visitVariableAssignment(VariableAssignmentNode node);

    public <T> T visitVariableAssignments(VariableAssignmentsNode node);

    public <T> T visitWhileStatement(WhileStatementNode node);

    // other

    public <T> T visitArray(ArrayNode node);

    public <T> T visitCaseItem(CaseItemNode node);

    public <T> T visitCommandName(CommandNameNode node);

    public <T> T visitComment(CommentNode node);

    public <T> T visitDoGroup(DoGroupNode node);

    public <T> T visitElifClause(ElifClauseNode node);

    public <T> T visitElseClause(ElseClauseNode node);

    public <T> T visitExtglobPattern(ExtglobPatternNode node);

    public <T> T visitFileDescriptor(FileDescriptorNode node);

    public <T> T visitFileRedirect(FileRedirectNode node);

    public <T> T visitHeredocBody(HeredocBodyNode node);

    public <T> T visitHeredocContent(HeredocContentNode node);

    public <T> T visitHeredocEnd(HeredocEndNode node);

    public <T> T visitHeredocRedirect(HeredocRedirectNode node);

    public <T> T visitHeredocStart(HeredocStartNode node);

    public <T> T visitHerestringRedirect(HerestringRedirectNode node);

    public <T> T visitRegex(RegexNode node);

    public <T> T visitSpecialVariableName(SpecialVariableNameNode node);

    public <T> T visitStringContent(StringContentNode node);

    public <T> T visitSubscript(SubscriptNode node);

    public <T> T visitTestOperator(TestOperatorNode node);

    public <T> T visitVariableName(VariableNameNode node);


    // root
    public <T> T visitProgram(ProgramNode node);


    // core
    public <T> T visitConcreteBase(ConcreteBaseAstNode node);
}
