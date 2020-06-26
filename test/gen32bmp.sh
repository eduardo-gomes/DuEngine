#!/bin/bash
#convert -background lightblue -fill blue -size 256x256 -define bmp:subtype=ARGB8888 caption:'0' 0.bmp
#used by MultiplesTextures

for x in {0..31};
do \
convert -background lightblue -fill blue -size 256x256 -define bmp:subtype=ARGB8888 caption:$x $x.bmp;
done;
