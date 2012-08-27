#!/bin/sh
export BC_LINE_LENGTH=$COLUMNS
CON=`echo "2^1088" | bc -s`
CORRECT=$(echo -n $CON | tail -c $(expr `echo -n "$CON" | wc -c` - 1) | tr -d '\\' | tr -d '\n' | tr -d ' ')
INCORRECT=$(echo console.log\( $(  cat $1 ) .df \) | node)
DEVIANCE=$(echo $CORRECT / $INCORRECT | bc -l)
echo $DEVIANCE
