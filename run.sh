#!/bin/sh

zig cc md5.c && ./a.out
echo "\nwaiting for changes...\n"
