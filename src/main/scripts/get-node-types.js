const { readFileSync } = require("fs");

const data = readFileSync("./node-types.json", { encoding: "utf8" });
const jsonData = JSON.parse(data);

const types = jsonData.map((node) => node.type);
console.dir(types, { maxArrayLength: null, depth: null });
