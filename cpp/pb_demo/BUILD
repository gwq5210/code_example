package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
  name = "demo_proto",
  srcs = ["demo.proto"],
)

cc_proto_library(
  name = "demo_proto_lib",
  deps = [":demo_proto"],
)

cc_binary(
  name = "demo",
  srcs = ["main.cc"],
  deps = [
    "demo_proto_lib"
  ]
)
