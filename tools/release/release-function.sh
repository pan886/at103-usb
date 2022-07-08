#!/bin/bash

#$1:file or file list
#$2:match type
#$3:replace target
function match_string_replace(){
    echo -e "\e[32;1m[note][$FUNCNAME][match:$2]\e[0m";
    for file in $1;
    do
        awk '{
                if(match($0,/'$2'/)){
                    gsub(/'$2'/,"'$3'",$0);
                }
                print $0 > "FILENAME";
            }
            END{
                if(NR){
                    system("mv -f FILENAME "FILENAME);
                }
            }' $file
    done
    echo -e "\e[32;1m[pass][$FUNCNAME][replace: $3]\e[0m";
}

#$1:file or file list
#$2:match type
#$3:replace target
function match_filename_replace(){
    echo -e "\e[32;1m[note][$FUNCNAME][match:$2]\e[0m";
    for file in $1;
    do
        local FILENAME="$(basename "$file")";
        local FILEPATH="$(dirname "$file")";
        awk 'END{
                if(match("'$FILENAME'",/'$2'/)){
                    FILENAME0="'$FILENAME'";
                    sub(/'$2'/,"'$3'",FILENAME0);
                    FILEPATH0="'$FILEPATH'";
                    # system("echo "FILEPATH0"/"FILENAME0);
                    system("mv -f "FILENAME" "FILEPATH0"/"FILENAME0);
                }
            }' $file
    done
    echo -e "\e[32;1m[pass][$FUNCNAME][replace: $3]\e[0m";
}