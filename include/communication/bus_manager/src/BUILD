load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "bus_manager",
    srcs = ["bus_manager.cc", "packet.cc"],
    hdrs = ["bus_manager.h", "packet.h", "subsystems.h"],
    includes = ["."],
    deps = ["//lib/serial/src:serial"],
    visibility = ["//visibility:public"],
)
