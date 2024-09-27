#!/bin/bash

OUTPUT_FILE="bimbam.txt"

FILE_SIZE=442368

dd if=/dev/urandom of=$OUTPUT_FILE bs=1 count=$FILE_SIZE status=none

SUM=$(od -An -tu1 -w1 $OUTPUT_FILE | awk '{sum += $1} END {print sum}')

echo "Файл $OUTPUT_FILE размером $FILE_SIZE байт создан."
echo "Сумма всех байт в файле: $SUM"
