#!/bin/bash
echo "static void"
echo "RP"$1"()"
echo "{"
echo $'\t'$1" *t = GETMEM (sizeof("$1"));"
echo $'\t'"if (STOP) return;"
echo $'\t'"read"$1" (t, 0);"
echo "}"
