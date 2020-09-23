#!/bin/bash

if [ $1 == "-c" ]
then
    rm Video* loss* rate* delay* rd* sd*
    exit 0
fi

if [ $4 == "container" ]
then
    video_st="st_container_cif_h264_300_20.st"
    video_yuv="container_cif_h264_300_20"
    yres=288
    xres=352

elif [ $4 == "highway" ]
then
    video_st="st_highway_cif.st"
    video_yuv="highway_cif"
    yres=288
    xres=352

elif [ $4 == "highway600" ]
then
    video_st="st_highway_600_cif"
    video_yuv="highway_600_cif"
    yres=288
    xres=352

elif [ $4 == "akiyo" ]
then
    video_st="st_akiyo_cif_h264_300_18"
    video_yuv="akiyo_cif_h264_300_20"
    yres=288
    xres=352

elif [ $4 == "masha" ]
then
    video_st="st_masha360p"
    video_yuv="masha360p_ref"
    yres=360
    xres=640

elif [ $4 == "despacito" ]
then
    video_st="st_despacito"
    video_yuv="despacito_ref"
    yres=360
    xres=640

elif [ $4 == "babyshark" ]
then
    video_st="st_baby_shark"
    video_yuv="baby_shark_ref"
    yres=360
    xres=640

else
    exit 1
fi

echo "executing for video ${4}"
echo "st: ${video_st}"

for i in $1; do
	for k in $(seq 1 $2); do
		cp $i/simul$k/rd* $i/simul$k/sd* .
        echo status of copying files: $?
		for y in $(seq 0 $3); do
			 ./etmp4 -F -0 sd_a01_$y rd_a01_$y "traces/$video_st" "reference_videos/$video_yuv.mp4" Video$y > /dev/null 2>&1
             echo mp4 generated with status $?

			 ffmpeg -i Video$y.mp4 Video$y.yuv > /dev/null 2>&1
             echo ffmpeg status: $?

			 wine msu_metric.exe -f reference_videos/$video_yuv.yuv IYUV -yw $xres -yh $yres -f Video$y.yuv -sc 1 -cod . -metr ssim_precise -cc YYUV > /dev/null 2>&1
			 wine msu_metric.exe -f reference_videos/$video_yuv.yuv IYUV -yw $xres -yh $yres -f Video$y.yuv -sc 1 -cod . -metr vqm -cc YYUV > /dev/null 2>&1
             echo wine msu_metrics status: $?

			 mv reference_videos/*csv loss* delay* rate* Video*.mp4  $i/simul$k/ > /dev/null 2>&1
             rm *.yuv
             echo
		done
	done
done
