#!/bin/bash
#convert -background lightblue -fill blue -size 256x256 -define bmp:subtype=ARGB8888 caption:'0' 0.bmp
#used by MultiplesTextures
PER_SIDE=16
PER_RESOLUTION=512
SHEET_RESOLUTION=$(expr $PER_RESOLUTION \* $PER_SIDE)
TOTAL_IMAGES=$(expr $PER_SIDE \* $PER_SIDE)
IMAGES_FILES=""
mkdir -p textures
for x in $(seq 0 $(expr $TOTAL_IMAGES - 1));
do \
NEW_FILE=textures/$x.bmp
convert -background lightblue -fill blue -size $PER_RESOLUTION\ x$PER_RESOLUTION -shave 1x1 -bordercolor black -border 1 -define bmp:subtype=ARGB8888 -gravity Center caption:$x $NEW_FILE;
IMAGES_FILES+=$NEW_FILE" "
done;
montage -geometry 1:1 -background none -tile $PER_SIDE\x$PER_SIDE $IMAGES_FILES -define bmp:subtype=ARGB8888 textures/sheet.bmp
