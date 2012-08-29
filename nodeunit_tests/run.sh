#!/bin/sh -ex
#NODE_PATH=node_modules node tests.js $@
./node_modules/nodeunit/bin/nodeunit tests.js
