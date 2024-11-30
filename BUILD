cc_library(
    name = "schema",
    hdrs = glob(["schema/*.hpp"]),
)

cc_library(
    name = "controller",
    srcs = ["controller.cpp"],
    hdrs = ["controller.hpp"]
)

cc_library(
    name = "perse_parameters",
    hdrs = ["perse_parameters.hpp"],
    srcs = ["perse_parameters.cpp"],
    deps = [
        "schema",
        "@boost//:program_options",
    ],
)


cc_binary(
    name = "mine",
    srcs = ["main.cpp"],
    deps = [
      "controller",
      "perse_parameters",
    ],
)
