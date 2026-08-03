const { readFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const targetType = process.argv[2];

const target = jsonData.find((node) => node.type === targetType);

console.dir(target, {maxArrayLength: null, depth: null });
