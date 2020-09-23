#!/bin/bash

if [ "$1" == "-c" ]
then
	rm -v rd* rate* loss* delay* *.mp4 *m4v reference_videos/*yuv
	exit 0
fi

cp exercise_traces/* .
ffmpeg -i reference_videos/highway_cif.mp4 reference_videos/highway_cif.yuv
for rd_trace in rd*
do
	echo $rd_trace
	./etmp4 -F -0 sd_a01_lte $rd_trace traces/st_highway_cif.st reference_videos/highway_cif.mp4 $rd_trace
	ffmpeg -i $rd_trace.mp4 $rd_trace.yuv
	wine msu_metric.exe -f reference_videos/highway_cif.yuv IYUV -yw 288 -yh 352 -f $rd_trace.yuv -sc 1 -cod . -metr ssim_precise -cc YYUV
done
