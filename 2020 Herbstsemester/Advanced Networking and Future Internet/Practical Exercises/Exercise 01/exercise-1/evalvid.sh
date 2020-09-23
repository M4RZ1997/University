#!/bin/bash

#-------------------------------------------------------------
#	EVALVID VIDEO ENCODER FOR SIMULATIONS
#	REQUIRES THE EVALVID BINARIES IN THE PATH
#	INPUT: MP4 VIDEO TO BE ENCODED
#	OUTPUT: st AND yuv FILES FOR EVALUATION
#-------------------------------------------------------------

# resolution of the input video
res=$(ffmpeg -i $1  2>&1 | grep -E "Stream.*Video" |sed 's/.* \([0-9]\{3,4\}x[0-9]\{3,4\}\) .*/\1/g')
#basename of input video
name=$(basename $1 .mp4)
#duration of the clip to be cut, leave 0 if you eant to use the entire video
duration=1 # mins
bitrate=400 # kbps

if [ $duration != 0 ]
then
	ffmpeg -y -ss 00:00:00.0 -i $1 -c copy -t 00:0${duration}:00.0 cut_$name.mp4
	ffmpeg -y -i cut_$name.mp4 $name.yuv
else
	ffmpeg -y -i $name.mp4 $name.yuv
fi

x264 -I 30 -B $bitrate --fps 30 -o a03.264 --input-res $res $name.yuv

MP4Box -hint -mtu 1024 -fps 30 -add a03.264 ${name}_ref.mp4
ffmpeg -y -i ${name}_ref.mp4 ${name}_ref.yuv
mp4trace -f -s 192.168.0.2 12346 ${name}_ref.mp4 > st_$name
