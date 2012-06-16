#!/bin/bash

#Sandy bridge processor architecture:0x062A or 0x062D. The followig definitions use the integer equivalents.

SB_FAMILY=6
SB_MODEL_1=42  #Supports RAPL without DRAM clamping
SB_MODEL_2=45  #Supports RAPL with DRAM clamping 

IA32_PERF_CTL_ADDR=0x199
MSR_PKG_POWER_LIMIT_ADDR=0x610
MSR_PKG_POWER_LIMIT_DEFAULT=0x6845000148398
TURBO_BIT=$((~(1<<32)))

enable_turbo() {
    local core=$1
    local val=$(rdmsr -p $core $IA32_PERF_CTL_ADDR)
    local newval=$(printf "%x" $(($TURBO_BIT & 0x$val)))
    if [ $newval != $val ]; then
        wrmsr -p $core $IA32_PERF_CTL_ADDR 0x$newval
    fi
}

set_pkg_power_limit_default() {
    local core=$1
    local current=$(rdmsr -p $core $MSR_PKG_POWER_LIMIT_ADDR)
    if [ 0x$current != $MSR_PKG_POWER_LIMIT_DEFAULT ]; then
        wrmsr -p $core $MSR_PKG_POWER_LIMIT_ADDR  $MSR_PKG_POWER_LIMIT_DEFAULT
    fi
}

family=$(cat /proc/cpuinfo | grep family | cut -d ":" -f 2 | head -n 1 | sed 's/[ \t]*//g')
model=$(cat /proc/cpuinfo | grep model | sed -n '1~2p'| cut -d ":" -f 2 | head -n 1| sed 's/[ \t]*//g')

if [ $family -eq $SB_FAMILY ];then 
	if [ $model -eq $SB_MODEL_1 ] || [ $model -eq $SB_MODEL_2 ]; then
		for cpu in $(cd /dev/cpu && ls -1d [0-9]* | sort -n); do
        		enable_turbo $cpu
        		set_pkg_power_limit_default $cpu
		done
	fi
fi

