IEEE-754 calculator ported to Emscripten
========================================

See README for original description.

Build available in the gh-pages branch too: http://ysangkok.github.com/IEEE-754/index.xhtml

Compatible emscripten for building found here: https://github.com/ysangkok/emscripten

    libgmp.a: needed to build c files
    build.xml: ant build file
    build.sh: just starts build with lots of debugging output
    janus\_frame.html: referenced by index.xhtml, buffers output
    computeBin.php: obsoleted
    computeDec.php: obsoleted
    bin/*: obsoleted

Warning: You need to enable precise I32 math in Emscripten if using another branch than the linked one. You also need a working atoi() for negative numbers, but that is probably committed by the time you read this. You can check if it is by looking at the history of my emscripten branch.
