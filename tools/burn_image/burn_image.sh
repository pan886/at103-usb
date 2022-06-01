#!/usr/bin/env bash

#$1 the elffilename
#$2 the tooldir
#$3 serial port

filename=`basename $1`
t=`env|grep OS|grep -i windows`
if [ $3 ]
then
ttyx=`echo $3`
else
if [ $t ]
then
echo 'On Windows'
ttyx=`ls /dev/ -a |grep -i "ttyS"|cut -d 'S' -f 2`
ttyx=`echo $ttyx|awk '{print $NF}'`
ttyx=`echo $(($ttyx+1))`
ttyx=`echo "COM$ttyx"`
else
echo 'On Linux'
ttyx=`dmesg|grep -i "attached to ttyusb"|cut -d ':' -f 2`
ttyx=`echo $ttyx|awk '{print $NF}'`
fi
fi

echo "Use $ttyx for burn"

cp $2/burn_template.ini burn.ini
sed -i -e "s/template.bin/$filename.bin/g" burn.ini
cp $2/flashburn_stub-data.bin ./
cp $2/flashburn_stub-text.bin ./
if [ $t ]
then
$2/downloader.exe -f burn.ini -c $ttyx -a
else
sudo $2/downloader -c /dev/$ttyx -f burn.ini -v 2 -a
fi
rm ./flashburn_stub-text.bin -f
rm ./flashburn_stub-data.bin -f