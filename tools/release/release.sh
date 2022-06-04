#!/bin/bash

RELEASE=yes

# we suppose that we are the 1 level upper the release folder. 
REL_PWD=`pwd`
REL_DIR=$REL_PWD/at103-sdk

if [ "$1" = "" ]
then
	echo "No specified path, use ./at1k-sdk-v2 by default"
else
	echo "[$1] is used as the target release folder."
	REL_DIR=$1
fi

echo -e "\n\e[93m#STEP1: Delete some directories and files...\e[0m\n"
cd $REL_DIR
rm $REL_DIR/test -rf
rm $REL_DIR/doc -rf
rm -rf .git
rm -rf $REL_DIR/tools/release
rm .gitignore
rm .clang-format
sed -i 's/RELEASE=no/RELEASE=yes/' sourceme-lin.sh

echo -e "\n\e[93m#STEP2: Build all projects...\e[0m\n"
source sourceme-lin.sh
build_all.sh ./

echo -e "\n\e[93m#STEP3: Clean up project folders...\e[0m\n"
build_all.sh ./ clean
