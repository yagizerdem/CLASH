const { readFileSync, writeFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const set = new Set();
const stack = ["_expression"];

while (stack.length > 0) {
  const current = stack.pop();
  set.add(current);

  const node = jsonData.find((node) => node.type === current);

  if (!node?.subtypes) {
    continue;
  }

  for (const subtype of node.subtypes) {
    if (subtype.type == "_primary_expression") {
      continue;
    }

    if (!set.has(subtype.type)) {
      stack.push(subtype.type);
    }
  }
}

const exprNodes = [];

jsonData.forEach((node) => {
  if (set.has(node.type)) {
    exprNodes.push(node);
  }
});

console.dir(exprNodes, {
  maxArrayLength: null,
});

console.log(`Found ${exprNodes.length} expression nodes.`);

writeFileSync("./expr-nodes.json", JSON.stringify(exprNodes, null, 2), {
  encoding: "utf8",
});
