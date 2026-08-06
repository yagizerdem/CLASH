package io.Clash;

import io.Clash.ast.ProgramNode;
import io.Clash.ast.base.AstNode;
import io.Clash.ast.base.ConcreteBaseAstNode;
import io.Clash.ast.expr.*;
import io.Clash.ast.other.*;
import io.Clash.ast.primaryExpr.*;
import io.Clash.ast.stmt.*;
import io.Clash.ast.visitor.BaseVisitor;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class AstPrinter extends BaseVisitor {

    private final int tabSize;

    public AstPrinter() {
        this(4);
    }

    public AstPrinter(int tabSize) {
        if (tabSize < 0) {
            throw new IllegalArgumentException("tabSize cannot be negative");
        }
        this.tabSize = tabSize;
    }

    private String printNode(AstNode node) {
        StringBuilder builder = new StringBuilder();
        appendNode(builder, node, 0, null);
        return builder.toString();
    }

    private void appendNode(StringBuilder builder, AstNode node, int level, String label) {
        indent(builder, level);
        if (label != null) {
            builder.append(label).append(": ");
        }
        builder.append(node.getClass().getSimpleName()).append('\n');

        for (Field field : fieldsOf(node.getClass())) {
            if (Modifier.isStatic(field.getModifiers()) || field.getName().equals("syntax")) {
                continue;
            }
            appendValue(builder, field.getName(), read(field, node), level + 1);
        }
    }

    private void appendValue(StringBuilder builder, String name, Object value, int level) {
        if (value instanceof AstNode child) {
            appendNode(builder, child, level, name);
            return;
        }

        if (value instanceof List<?> values) {
            if (values.isEmpty()) {
                appendProperty(builder, level, name, "[]");
                return;
            }

            boolean containsNode = values.stream().anyMatch(AstNode.class::isInstance);
            if (containsNode) {
                for (int i = 0; i < values.size(); i++) {
                    Object element = values.get(i);
                    String indexedName = name + "[" + i + "]";
                    if (element instanceof AstNode child) {
                        appendNode(builder, child, level, indexedName);
                    } else {
                        appendProperty(builder, level, indexedName, scalar(element));
                    }
                }
            } else {
                appendProperty(builder, level, name, scalar(values));
            }
            return;
        }

        appendProperty(builder, level, name, scalar(value));
    }

    private void appendProperty(StringBuilder builder, int level, String name, String value) {
        indent(builder, level);
        builder.append(name).append(": ").append(value).append('\n');
    }

    private void indent(StringBuilder builder, int level) {
        builder.append(" ".repeat(level * tabSize));
    }

    private String scalar(Object value) {
        if (value == null) {
            return "null";
        }
        if (value instanceof String text) {
            return "\"" + text
                    .replace("\\", "\\\\")
                    .replace("\r", "\\r")
                    .replace("\n", "\\n")
                    .replace("\t", "\\t")
                    .replace("\"", "\\\"") + "\"";
        }
        if (value instanceof List<?> values) {
            return values.stream()
                    .map(this::scalar)
                    .reduce((left, right) -> left + ", " + right)
                    .map(content -> "[" + content + "]")
                    .orElse("[]");
        }
        return String.valueOf(value);
    }

    private Object read(Field field, AstNode node) {
        try {
            field.setAccessible(true);
            return field.get(node);
        } catch (IllegalAccessException exception) {
            throw new IllegalStateException("Cannot read AST field: " + field.getName(), exception);
        }
    }

    private List<Field> fieldsOf(Class<?> nodeClass) {
        List<Class<?>> hierarchy = new ArrayList<>();
        for (Class<?> type = nodeClass; type != null && type != Object.class; type = type.getSuperclass()) {
            hierarchy.add(type);
        }
        Collections.reverse(hierarchy);

        List<Field> fields = new ArrayList<>();
        for (Class<?> type : hierarchy) {
            Collections.addAll(fields, type.getDeclaredFields());
        }
        return fields;
    }


    // expr

    @Override
    public String visitBinaryExpression(BinaryExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitConcatenation(ConcatenationNode node) {
        return printNode(node);
    }

    @Override
    public String visitParenthesizedExpression(ParenthesizedExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitPostfixExpression(PostfixExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitTernaryExpression(TernaryExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitUnaryExpression(UnaryExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitWord(WordNode node) {
        return printNode(node);
    }

    // primary expr

    @Override
    public String visitAnsiCString(AnsiCStringNode node) {
        return printNode(node);
    }

    @Override
    public String visitArithmeticExpansion(ArithmeticExpansionNode node) {
        return printNode(node);
    }

    @Override
    public String visitBraceExpression(BraceExpressionNode node) {
        return printNode(node);
    }

    @Override
    public String visitCommandSubstitution(CommandSubstitutionNode node) {
        return printNode(node);
    }

    @Override
    public String visitExpansion(ExpansionNode node) {
        return printNode(node);
    }

    @Override
    public String visitNumber(NumberNode node) {
        return printNode(node);
    }

    @Override
    public String visitProcessSubstitution(ProcessSubstitutionNode node) {
        return printNode(node);
    }

    @Override
    public String visitRawString(RawStringNode node) {
        return printNode(node);
    }

    @Override
    public String visitSimpleExpansion(SimpleExpansionNode node) {
        return printNode(node);
    }

    @Override
    public String visitString(StringNode node) {
        return printNode(node);
    }

    @Override
    public String visitTranslatedString(TranslatedStringNode node) {
        return printNode(node);
    }

    // stmt

    @Override
    public String visitCStyleForStatement(CStyleForStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitCaseStatement(CaseStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitCommand(CommandNode node) {
        return printNode(node);
    }

    @Override
    public String visitCompoundStatement(CompoundStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitDeclarationCommand(DeclarationCommandNode node) {
        return printNode(node);
    }

    @Override
    public String visitForStatement(ForStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitFunctionDefinition(FunctionDefinitionNode node) {
        return printNode(node);
    }

    @Override
    public String visitIfStatement(IfStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitList(ListNode node) {
        return printNode(node);
    }

    @Override
    public String visitNegatedCommand(NegatedCommandNode node) {
        return printNode(node);
    }

    @Override
    public String visitPipeline(PipelineNode node) {
        return printNode(node);
    }

    @Override
    public String visitRedirectedStatement(RedirectedStatementNode node) {
        return printNode(node);
    }

    @Override
    public String visitSubshell(SubshellNode node) {
        return printNode(node);
    }

    @Override
    public String visitTestCommand(TestCommandNode node) {
        return printNode(node);
    }

    @Override
    public String visitUnsetCommand(UnsetCommandNode node) {
        return printNode(node);
    }

    @Override
    public String visitVariableAssignment(VariableAssignmentNode node) {
        return printNode(node);
    }

    @Override
    public String visitVariableAssignments(VariableAssignmentsNode node) {
        return printNode(node);
    }

    @Override
    public String visitWhileStatement(WhileStatementNode node) {
        return printNode(node);
    }

    // other

    @Override
    public String visitArray(ArrayNode node) {
        return printNode(node);
    }

    @Override
    public String visitCaseItem(CaseItemNode node) {
        return printNode(node);
    }

    @Override
    public String visitCommandName(CommandNameNode node) {
        return printNode(node);
    }

    @Override
    public String visitComment(CommentNode node) {
        return printNode(node);
    }

    @Override
    public String visitDoGroup(DoGroupNode node) {
        return printNode(node);
    }

    @Override
    public String visitElifClause(ElifClauseNode node) {
        return printNode(node);
    }

    @Override
    public String visitElseClause(ElseClauseNode node) {
        return printNode(node);
    }

    @Override
    public String visitExtglobPattern(ExtglobPatternNode node) {
        return printNode(node);
    }

    @Override
    public String visitFileDescriptor(FileDescriptorNode node) {
        return printNode(node);
    }

    @Override
    public String visitFileRedirect(FileRedirectNode node) {
        return printNode(node);
    }

    @Override
    public String visitHeredocBody(HeredocBodyNode node) {
        return printNode(node);
    }

    @Override
    public String visitHeredocContent(HeredocContentNode node) {
        return printNode(node);
    }

    @Override
    public String visitHeredocEnd(HeredocEndNode node) {
        return printNode(node);
    }

    @Override
    public String visitHeredocRedirect(HeredocRedirectNode node) {
        return printNode(node);
    }

    @Override
    public String visitHeredocStart(HeredocStartNode node) {
        return printNode(node);
    }

    @Override
    public String visitHerestringRedirect(HerestringRedirectNode node) {
        return printNode(node);
    }

    @Override
    public String visitRegex(RegexNode node) {
        return printNode(node);
    }

    @Override
    public String visitSpecialVariableName(SpecialVariableNameNode node) {
        return printNode(node);
    }

    @Override
    public String visitStringContent(StringContentNode node) {
        return printNode(node);
    }

    @Override
    public String visitSubscript(SubscriptNode node) {
        return printNode(node);
    }

    @Override
    public String visitTestOperator(TestOperatorNode node) {
        return printNode(node);
    }

    @Override
    public String visitVariableName(VariableNameNode node) {
        return printNode(node);
    }

    // root / base

    @Override
    public String visitProgram(ProgramNode node) {
        return printNode(node);
    }

    @Override
    public String visitConcreteBase(ConcreteBaseAstNode node) {
        return printNode(node);
    }
}
