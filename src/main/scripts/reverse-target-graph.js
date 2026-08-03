const { readFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const allTypes = new Set(jsonData.map(node => node.type));

const reverseGraph = {};

for (const type of allTypes) {
  reverseGraph[type] = new Set();
}


function collectDependencies(value, dependencies, key = null) {
  if (value == null) {
    return;
  }

  if (Array.isArray(value)) {
    for (const item of value) {
      collectDependencies(item, dependencies, key);
    }
    return;
  }

  if (typeof value !== "object") {
    return;
  }

  for (const [currentKey, currentValue] of Object.entries(value)) {
    if (currentKey === "subtypes") {
      continue;
    }

    if (
        currentKey === "type" &&
        typeof currentValue === "string" &&
        allTypes.has(currentValue)
    ) {
      dependencies.add(currentValue);
      continue;
    }

    collectDependencies(currentValue, dependencies, currentKey);
  }
}


for (const node of jsonData) {
  const dependencies = new Set();

  for (const [key, value] of Object.entries(node)) {
    if (
        key === "type" ||
        key === "subtypes"
    ) {
      continue;
    }

    collectDependencies(value, dependencies, key);
  }

  for (const dependency of dependencies) {
    // self dependency istemiyorsan bunu bırak
    if (dependency === node.type) {
      continue;
    }

    reverseGraph[dependency].add(node.type);
  }
}

const sortedReverseGraph = Object.fromEntries(
    Object.entries(reverseGraph)
        .map(([type, dependents]) => [
          type,
          [...dependents].sort()
        ])
        .sort(([, a], [, b]) => b.length - a.length)
);

console.dir(sortedReverseGraph, {
  depth: null,
});