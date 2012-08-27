IEEE-754 calculator ported to Emscripten
========================================

See [README](https://github.com/ysangkok/IEEE-754/blob/master/README) for original description.

Build available in the gh-pages branch too: http://ysangkok.github.com/IEEE-754/index.xhtml

Building
--------

Compatible emscripten for building found here: https://github.com/ysangkok/emscripten
gmp for Emscripten and build instructions found here: https://github.com/kripken/gmp.js

**Warning**: You need to enable precise I32 math in Emscripten if using another branch than the linked one. You also need a working atoi() for negative numbers, but that is probably committed by the time you read this. You can check if it is by looking at the history of my emscripten branch.

Files
-----

    libgmp.a: needed to build c files
    build.xml: ant build file
    build.sh: just starts build with lots of debugging output
    janus_frame.html: referenced by index.xhtml, buffers output
    computeBin.php: obsoleted
    computeDec.php: obsoleted
    bin/*: obsoleted
    src/{dtb2,btd2}.links.c: C ports of original scripts for faster (build time) and smaller builds
    {dtb2,btd2}.links.html: Emscripten results wrapped in default Emscripten shell (the shell is not used)

Calling btd and dtb
-------------------

btd and dtb that used to be called over JSON can still be executed.

For example, go to http://ysangkok.github.com/IEEE-754/janus_frame.html?frame=btd2.links.html and copy the contents of the text area in the upper right corner textarea to the clipboard and execute it in the JavaScript console or Firebug or similar.  Now enter 

    call("1 1087 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000".split(" "),10)

and the output of ``./btd 1 1087 000...000`` should appear in the JavaScript console. The second argument is the number of lines to wait for before calling the callback, for dtb this needs to be 7 of course, or the callback will not be called. In Chrome, the call to "copy" will put the results in the clipboard so that they can easily be compared with the reference.