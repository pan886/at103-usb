file filename
target remote localhost:3333
set {int*}0x4001D010=0x80622008
load
set $pc=&Reset_Handler