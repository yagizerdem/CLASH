const { readFileSync } = require("fs");
const { resolve } = require("path");

const nodeTypesPath = resolve(__dirname, "..", "node-types.json");

const data = readFileSync(nodeTypesPath, { encoding: "utf8" });
const jsonData = JSON.parse(data);

const allTypes = new Set(jsonData.map(node => node.type));

const targetGraph = {};
const reverseGraph = {};

for (const type of allTypes) {
    targetGraph[type] = new Set();
    reverseGraph[type] = new Set();
}

function collectDependencies(value, dependencies) {
    if (value == null) {
        return;
    }

    if (Array.isArray(value)) {
        for (const item of value) {
            collectDependencies(item, dependencies);
        }
        return;
    }

    if (typeof value !== "object") {
        return;
    }

    for (const [key, currentValue] of Object.entries(value)) {
        // subtype ilişkilerini dependency olarak alma
        if (key === "subtypes") {
            continue;
        }

        if (
            key === "type" &&
            typeof currentValue === "string" &&
            allTypes.has(currentValue)
        ) {
            dependencies.add(currentValue);
            continue;
        }

        collectDependencies(currentValue, dependencies);
    }
}

for (const node of jsonData) {
    const dependencies = new Set();

    for (const [key, value] of Object.entries(node)) {
        if (key === "type" || key === "subtypes") {
            continue;
        }

        collectDependencies(value, dependencies);
    }

    for (const dependency of dependencies) {
        if (dependency === node.type) {
            continue;
        }

        targetGraph[node.type].add(dependency);

    }
}

const sortedTargetGraph = Object.fromEntries(
    Object.entries(targetGraph)
        .map(([type, dependencies]) => [
            type,
            [...dependencies].sort()
        ])
        .sort(([, a], [, b]) => b.length - a.length)
);

console.log("TARGET GRAPH:");
console.dir(sortedTargetGraph, {
    depth: null,
});
