const { spawnSync } = require("child_process");

function spawnSyncWrapper(program, args = [], options = {}) {
  const normalizedArgs = Array.isArray(args) ? args : [args];

  return spawnSync(program, normalizedArgs, {
    encoding: "utf8",
    ...options,
  });
}

function gitWrapper(args = [], options = {}) {
  return spawnSyncWrapper("git", args, options);
}

module.exports = { spawnSyncWrapper, gitWrapper };
