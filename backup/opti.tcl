#===========================================================
# Parameters for 802.11 nodes
#===========================================================
 #Antenna/MultiAntenna set Dista_ 3
#Antenna/OmniAntenna set Gt_ 1 ;# Transmit antenna gain
#Antenna/OmniAntenna set Gr_ 1 ;# Receive antenna gain
#Antenna/OmniAntenna set ht_ 1.5 ;# Tx antenna heigth
#Antenna/OmniAntenna set hr_ 1.5 ;# Rx antenna heigth
#Phy/WirelessPhy set L_ 1.0 ;# System Loss Factor
#Phy/WirelessPhy set freq_ 2.412e9 ;# Channel 1: 2.412 GHz
#Phy/WirelessPhy set Pt_ 0.100 ;# Transmit Power (15dBm)
#Phy/WirelessPhy set CPThresh_ 10.0 ;# Collision threshold
#Phy/WirelessPhy set CSThresh_ 3.1622777e-12 ;# Carrier Sense Power (-94dBm)
#Phy/WirelessPhy set RXThresh_ 1.20174e-05;# 4m
#Phy/WirelessPhy set RXThresh_ 1.56962e-05;# 3.5m
#Phy/WirelessPhy set RXThresh_ 2.00081e-05;# 3.1m
#Phy/WirelessPhy set RXThresh_ 2.13643e-05;# 3m
#Phy/WirelessPhy set RXThresh_ 3.07645e-05;# 2.5m
#Phy/WirelessPhy set RXThresh_ 4.80696e-05;# 2m
#Phy/WirelessPhy set RXThresh_ 8.5457e-05;# 1.5m
#Phy/WirelessPhy set RXThresh_ 0.000192278;# 1m

Mac/802_11 set basicRate_ 1Mb ;# set this to 0 if want to use bandwidth_ for

Mac/802_11 set dataRate_ 1Mb ;# both control and data pkts
#这个速度将直接制约吞吐量的大小，但同时需要考虑cbr包发送的速率，因为若cbr速率过大的话
#会导致大量缓存丢包(大部分数据没时间发)，如果cbr速率过小的话，则可能吞吐量满血(大部分时间没数据发).


#Phy/WirelessPhy set bandwidth_ 11Mb
###Mac/802_11 set CWMin_ 31 ;#
###Mac/802_11 set CWMax_ 1023 ;#
###Mac/802_11 set SlotTime_ 0.000020 ;# 20us
###Mac/802_11 set SIFS_ 0.000010 ;# 10us
###Mac/802_11 set PreambleLength_ 144 ;# 144 bit
###Mac/802_11 set PLCPHeaderLength_ 48 ;# 48 bits
###Mac/802_11 set PLCPDataRate_ 1.0e6 ;# 1Mbps
###Mac/802_11 set RTSThreshold_ 2500 ;# bytes: changed so thatRTS/CTS has been disabled ENRI
###Mac/802_11 set ShortRetryLPLCPimit_ 7 ;# retransmissions
#Mac/802_11 set LongRetryLimit_ 4 ;# retransmissions
###############################################################
Antenna/MultiAntenna set Width_ 50
#Phy/WirelessPhy set PeThresh_ 100   #not useful now
 set opt(chan) Channel/WirelessChannel ;# channel type
 set opt(prop) Propagation/OptiPropagation ;# radio-propagation model
#当前选择的传输模型与天线的配置有关。

# set opt(prop) Propagation/FreeSpace
 set opt(netif) Phy/WirelessPhy ;# network interface type
 set opt(mac) Mac/802_11 ;# MAC type
 set opt(ifq) Queue/DropTail/PriQueue ;# interface queue type
 set opt(ll) LL ;# link layer type
 set opt(ant) Antenna/OmniAntenna ;# antenna model
 set opt(ant2) Antenna/MultiAntenna ;
 set opt(ifqlen) 100 ;# max packet in ifq
 set opt(nn) 2 ;# number of mobilenodes
 set opt(adhocRouting) DSDV ;# routing protocol
 set opt(cp) "" ;# cp file not used
 set opt(sc) "" ;#/home/TFCjesus/scengen/scen/prueba" ;# node movement file.
 set opt(x) 1000 ;# x coordinate of topology
 set opt(y) 1000 ;# y coordinate of topology
 set opt(seed) 0.0 ;# random seed
 set opt(stop) 80 ;# time to stop simulation
 set num_wired_nodes 3
 set chan_1_ [new $opt(chan)]

#delete a:Pr b:SNR
#	exec rm AP_rcv_Pr STA1_rcv_Pr STA2_rcv_Pr SNR


 # create simulator instance
 set ns_ [new Simulator]
 # set up for hierarchical routing
 
$ns_ node-config -addressType hierarchical
AddrParams set domain_num_ 2 ;# number of domains
lappend cluster_num 1 1;# number of clusters in each domain
AddrParams set cluster_num_ $cluster_num
lappend eilastlevel 3 3;# number of nodes in each cluster
AddrParams set nodes_num_ $eilastlevel ;# of each domain
set tracefd [open handover.tr w]
set namtrace [open handover.nam w]
#$ns_ use-newtrace
$ns_ trace-all $tracefd
$ns_ namtrace-all-wireless $namtrace $opt(x) $opt(y)
# Create topography object
set topo [new Topography]
# define topology
$topo load_flatgrid $opt(x) $opt(y)
# create God
create-god [expr $opt(nn) + 1]

#create wired nodes
set temp {0.0.0 0.0.1 0.0.2} ;# hierarchical addresses
for {set i 0} {$i < $num_wired_nodes} {incr i} {
set W($i) [$ns_ node [lindex $temp $i]]
}
$W(0) set X_ 101.5
$W(0) set Y_ 101.5
$W(0) set Z_ 5.0
$ns_ at 0.01 "$W(0) label \"Switch\""
$W(1) set X_ 101.5
$W(1) set Y_ 105.0
$W(1) set Z_ 5.0
$ns_ at 0.01 "$W(1) label \"Host1\""
$W(2) set X_ 105.0
$W(2) set Y_ 101.5
$W(2) set Z_ 5.0
$ns_ at 0.01 "$W(2) label \"Host2\""


# Configure for ForeignAgent and HomeAgent nodes
$ns_ node-config -adhocRouting $opt(adhocRouting) \
-llType $opt(ll) \
-macType $opt(mac) \
-ifqType $opt(ifq) \
-ifqLen $opt(ifqlen) \
-antType $opt(ant2) \
-propType $opt(prop) \
-phyType $opt(netif) \
-channel $chan_1_\
-topoInstance $topo \
-wiredRouting ON \
-agentTrace OFF \
-routerTrace OFF \
-macTrace ON

set AP [$ns_ node 1.0.0] ;# disable random motion

$AP random-motion 0

#set mac_(2) [$AP2 getMac 0]
#$mac_(2) ap [$mac_(2) id]
#$mac_(2) set RTSThreshold_ 3000

$AP set X_ 100.0
$AP set Y_ 100.0
$AP set Z_ 5.0

$ns_ at 0.01 "$AP label \"AP\""

$ns_ node-config -wiredRouting OFF \
-antType $opt(ant)

set MN [$ns_ node 1.0.1]
set node_(6) $MN
#set mac_(6) [$node_(6) getMac 0]
#$mac_(6) set RTSThreshold_ 3000
#puts [mac_(6) set bandwidth_]

set MN2 [$ns_ node 1.0.2]
set node_(7) $MN2
#set mac_(7) [$node_(7) getMac 0]


set APaddress [AddrParams addr2id [$AP node-addr]];
$MN base-station $APaddress
$MN2 base-station $APaddress
#[$MN set regagent_] set home_agent_ $AP2address
#[$MN2 set regagent_] set home_agent_ $AP2address

#set HAaddress [AddrParams addr2id [$MN node-addr]];
#[#MN set regagent_] set home_agent_ $
$MN random-motion 0
$MN2 random-motion 0
$ns_ at 0.01 "$MN label \"MN1\""
$ns_ at 0.01 "$MN2 label \"MN2\""
#$mac_(6) ScanType ACTIVE

$MN set X_ 90
$MN set Y_ 96
$MN set Z_ 0
#简单移动
#$ns_ at 15.0 "$MN setdest 92 96 1"
#复杂运动
$ns_ at 1.0 "$MN setdest 104 96 1"
$ns_ at 15.0 "$MN setdest 104 104 1"
$ns_ at 23.0 "$MN setdest 96 104 1"
$ns_ at 31.0 "$MN setdest 104 96 1"
$ns_ at 43.0 "$MN setdest 104 104 1"
$ns_ at 51.0 "$MN setdest 96 96 1"
$ns_ at 63.0 "$MN setdest 90 96 1"
#$ns_ at 63 "$MN setdest 100 100 1"

$MN2 set X_ 104.0001
$MN2 set Y_ 96.0001
$MN2 set Z_ 0

#$ns_ at 23.0 "$MN2 setdest 104.0001 104.00001 1"


#方差门限确定所需测试数据
#$MN set X_ 90
#$MN set Y_ 90
#$MN set Z_ 0
#
#$ns_ at 1 "$MN setdest 110 110 1"

# create links between wired and BaseStation nodes
$ns_ duplex-link $W(0) $AP 2Mb 1ms DropTail
$ns_ duplex-link $W(1) $W(0) 2Mb 1ms DropTail
$ns_ duplex-link $W(2) $W(0) 2Mb 1ms DropTail
	
$ns_ duplex-link-op $W(0) $AP orient left-down
$ns_ duplex-link-op $W(1) $W(0) orient up
$ns_ duplex-link-op $W(2) $W(0) orient down

#UDP1
set udp [new Agent/UDP]
set sink [new Agent/LossMonitor]
$ns_ attach-agent $W(1) $udp
$ns_ attach-agent $MN $sink
$ns_ connect $udp $sink

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
#$cbr set packet_size_ 300
$cbr set rate_ 500Kb
#$cbr set ramdom_ false
#$cbr set packetSize_ 512
#$cbr set interval_ 0.01
$cbr set random_ 1
#$cbr set maxpkts_ 1000

#UDP2
set udp2 [new Agent/UDP]
set sink2 [new Agent/LossMonitor]
$ns_ attach-agent $W(2) $udp2
$ns_ attach-agent $MN2 $sink2
$ns_ connect $udp2 $sink2

set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp2
$cbr2 set type_ CBR
#$cbr2 set packet_size_ 300
$cbr2 set rate_ 500Kb
#$cbr2 set random_ false
#$cbr2 set packetSize_ 512
#$cbr2 set interval_ 0.01
$cbr2 set random_ 1
#$cbr2 set maxpkts_ 1000


for {set i 6} {$i < 8} {incr i} {
$ns_ initial_node_pos $node_($i) 5
}




#initialize flags
set f0 [open th.tr w]
set f1 [open lost.tr w]
set f2 [open th2.tr w]
set holdrate 0
set holdrate2 0

#figure out the UDP thoughtput
	proc record {} {
		global sink f0 f1 f2 holdrate holdrate2 ns_ sink2
	
		set time 0.5
		set bw0 [$sink set bytes_]
		set bw1 [$sink set nlost_]
		set bw2 [$sink2 set bytes_]
		set now [$ns_ now]
		
		puts $f0 "$now	[expr ($bw0+$holdrate)*8/(2*$time*1000000)]"
		puts $f1 "$now  [expr $bw1/$time]"
		puts $f2 "$now	[expr ($bw2+$holdrate2)*8/(2*$time*1000000)]"


		$sink set bytes_ 0
		$sink set nlost_ 0
		$sink2 set bytes_ 0

		set holdrate $bw0
		set holdrate2 $bw2

		$ns_ at [expr $now+$time] "record"
	}







	proc set_cbr {} {
		global ns_ MN

		set time 1
		set now [$ns_ now]

		set macMN [$MN getMac 0]
		set traceMN [$macMN down-target]
		set phyMN [$traceMN target]

		$phyMN set_cbr

		$ns_ at [expr $now+$time] "set_cbr"
	}

	proc set_ant {} {
		global MN ns_

		set macMN [$MN getMac 0]
		set traceMN [$macMN down-target]
		set phyMN [$traceMN target]
		
		$phyMN set_an_1 1
		$phyMN set_an_2 1
		$phyMN set_an_3 0
		$phyMN set_an_4 0
	}


$ns_ at 0.0 "record"
#$ns_ at 0.0 "set_cbr"
#$ns_ at 0.0 "set_ant"


#change
$ns_ at 0.1 "$cbr start"
$ns_ at $opt(stop).0 "$cbr stop"

#$ns_ at 0.2 "$cbr2 start"
#$ns_ at $opt(stop).0 "$cbr2 stop"

$ns_ at $opt(stop).0 "$node_(6) reset";
$ns_ at $opt(stop).0 "$node_(7) reset";
$ns_ at $opt(stop).0 "$AP reset";

$ns_ at $opt(stop).0002 "puts \"NS EXITING...\" ; $ns_ halt"
$ns_ at $opt(stop).0001 "stop"

proc stop {} {
global ns_ tracefd namtrace f0 f1 f2
close $tracefd
close $namtrace
close $f0
close $f1
close $f2
exec xgraph th.tr th2.tr  -geometry 800*400 &
#exec xgraph th2.tr  -geometry 800*400 &
#exec xgraph lost.tr -geometry 800*400 &
#exec nam handover.nam &
exit 0
}

puts "Starting Simulation..."
$ns_ run

















