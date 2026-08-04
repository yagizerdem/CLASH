const { readFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const types = jsonData.filter(node => node.named).map((node) => node.type);

console.dir(types, {
    maxArrayLength: null,
    depth: null,
});
