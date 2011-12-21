s2e = {
	--  kleeArgs = {
	--     "--use-batching-search=true", "--batch-time=1.0",
	--     "--use-forked-stp", "--max-stp-time=10"
	--  }
	kleeArgs = {
--		"--help",
--		"--use-dfs-search",
		"--use-query-log",
		"--use-query-pc-log",
		"--use-stp-query-pc-log",
		"--use-batching-search=true",
		"--batch-time=1.0",
		"--use-random-path",
		"--use-cex-cache=true",
		"--use-cache=true",
		"--use-fast-cex-solver=true",
		"--max-stp-time=10",
		"--use-expr-simplifier=true",
		"--print-expr-simplifier=false",
		"--flush-tbs-on-state-switch=false"
	}
}

plugins = {
  -- Enable S2E custom opcodes
  "BaseInstructions",

  -- Basic tracing, required for test case generation
  "ExecutionTracer",

  -- Enable the test case generator plugin
  "TestCaseGenerator",

  -- s2eget
  "HostFiles"
}

pluginsConfig = {
  HostFiles = {
    baseDir = "./demos"
  }
}


