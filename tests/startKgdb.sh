#!/usr/bin/env bash
set -e

gdb -q build/kernel/arcade.debug -x tests/gdb.boot