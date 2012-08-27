var Module = {};
//Module["arguments"] = ["1", "1087", "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"];
Module["arguments"] = "1 0 5 0 -1".split(" ");
Module["noExitRuntime"] = true;
Module["print"] = function(x) { parent.stdoutLine(x); };
