package io.Clash.ast;

import org.treesitter.TSNode;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.Queue;

public class Util {


    public static TSNode getFirstMatchedTypePreOrder(TSNode node, String type) {
        if (node.getType().equals(type)) {
            return node;
        }

        for (int i = 0; i < node.getChildCount(); i++) {
            TSNode child = node.getChild(i);

            TSNode result = getFirstMatchedTypePreOrder(child, type);

            if (result != null) {
                return result;
            }
        }

        return null;
    }


    public static TSNode getFirstMatchedTypePostOrder(TSNode node, String type) {
        for (int i = 0; i < node.getChildCount(); i++) {
            TSNode child = node.getChild(i);

            TSNode result = getFirstMatchedTypePostOrder(child, type);

            if (result != null) {
                return result;
            }
        }

        if (node.getType().equals(type)) {
            return node;
        }

        return null;
    }


    public static TSNode getFirstMatchedTypeDFS(TSNode root, String type) {
        Deque<TSNode> stack = new ArrayDeque<>();
        stack.push(root);

        while (!stack.isEmpty()) {
            TSNode node = stack.pop();

            if (node.getType().equals(type)) {
                return node;
            }


            for (int i = node.getChildCount() - 1; i >= 0; i--) {
                stack.push(node.getChild(i));
            }
        }

        return null;
    }


    public static TSNode getFirstMatchedTypeBFS(TSNode root, String type) {
        Queue<TSNode> queue = new ArrayDeque<>();
        queue.add(root);

        while (!queue.isEmpty()) {
            TSNode node = queue.remove();

            if (node.getType().equals(type)) {
                return node;
            }

            for (int i = 0; i < node.getChildCount(); i++) {
                queue.add(node.getChild(i));
            }
        }

        return null;
    }
}