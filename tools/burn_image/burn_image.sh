#!/usr/bin/env bash

#$1 the elffilename
#$2 the tooldir

filename=`basename $1`

# get the ttyUSB device from the dmesg output
ttyx=`dmesg|grep -i "attached to ttyusb"|cut -d ':' -f 2`
ttyx=`echo $ttyx|awk '{print $NF}'`

echo "Use $ttyx for burn"

cp $2/burn_template.ini burn.ini
sed -i -e "s/template.bin/$filename.bin/g" burn.ini
cp $2/flashburn_stub-data.bin ./
cp $2/flashburn_stub-text.bin ./
sudo $2/downloader -c /dev/$ttyx -f burn.ini -v 2 -a
rm ./flashburn_stub-text.bin -f
rm ./flashburn_stub-data.bin -f