const { readFileSync, writeFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");
const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const set = new Set();
const stack = ["_primary_expression"];

while (stack.length > 0) {
  const current = stack.pop();
  set.add(current);

  const node = jsonData.find((node) => node.type === current);

  if (!node?.subtypes) {
    continue;
  }

  for (const subtype of node.subtypes) {
    if (!set.has(subtype.type)) {
      stack.push(subtype.type);
    }
  }
}

const primaryExprNodes = [];

jsonData.forEach((node) => {
  if (set.has(node.type)) {
    primaryExprNodes.push(node);
  }
});

console.dir(primaryExprNodes, {
  maxArrayLength: null,
});

console.log(`Found ${primaryExprNodes.length} primary expression nodes.`);

writeFileSync(
  "./primary-expr-nodes.json",
  JSON.stringify(primaryExprNodes, null, 2),
  {
    encoding: "utf8",
  },
);
