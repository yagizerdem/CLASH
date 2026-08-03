const { readFileSync, writeFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const set = new Set();
const stack = ["_statement"];

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

const stmtNodes = [];

jsonData.forEach((node) => {
  if (set.has(node.type)) {
    stmtNodes.push(node);
  }
});

console.dir(stmtNodes, {
  maxArrayLength: null,
});

writeFileSync("./stmt-nodes.json", JSON.stringify(stmtNodes, null, 2), {
  encoding: "utf8",
});
