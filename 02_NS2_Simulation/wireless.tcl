if {$argc != 4} {
	puts "wireless.tcl program need three input as arguements; the first one for error rate and the other for bandwidth" 
	return;
}

# ======================================================================
# Define options
# ======================================================================
set opt(chan)           Channel/WirelessChannel  ;# channel type
set opt(prop)           Propagation/TwoRayGround ;# radio-propagation model
set opt(netif)          Phy/WirelessPhy          ;# network interface type
set opt(mac)            Mac/802_11               ;# MAC type
set opt(ifq)            Queue/DropTail/PriQueue  ;# interface queue type
set opt(ll)             LL                       ;# link layer type
set opt(ant)            Antenna/OmniAntenna      ;# antenna model
set opt(ifqlen)        		 50                  ;# max packet in ifq

Mac/802_11 set dataRate_ [lindex $argv 1]Mb     ;# set dataRate
Mac/802_11 set RTSThreshold_ 1000000000          ;# enable rts
Mac/802_11 set PER_ [lindex $argv 0]             ;# set error rate
Mac/802_11 set bandwidth_ [lindex $argv 1]Mb     ;# set bandwidth

set em_rate [lindex $argv 0]                     ;
set packet_size [lindex $argv 3]
Node/MobileNode set error_ [lindex $argv 0]      ;
Mac/802_11 set aarf_ false                       ;# 802.11 Auto Rate Fallback
Mac/802_11 set BER_ [lindex $argv 0]             ;
set my_err_rate [lindex $argv 0]                 ;


puts stdout "Simulation with: "
puts stdout "Error Rate : [lindex $argv 0]"
puts stdout "Bandwidth : [lindex $argv 1]"
puts stdout "Packet Size : [lindex $argv 3]"


set opt(bottomrow)      9                        ;# number of bottom-row nodes
set opt(spacing)        200                      ;# spacing between bottom-row nodes
set opt(mheight)        150                      ;# height of moving node above bottom-row nodes
set opt(brheight)       50                       ;# height of bottom-row nodes from bottom edge
set opt(x)              [expr ($opt(bottomrow)-1)*$opt(spacing)+1]    ;# x coordinate of topology
set opt(y)              300                      ;# y coordinate of topology

set opt(adhocRouting)   AODV                     ;# routing protocol
set opt(finish)         100                      ;# time to stop simulation

# the next value is the speed in meters/sec to move across the field
set opt(speed)          [expr 1.0*$opt(x)/$opt(finish)]

# create the simulator object
set ns [new Simulator]

# set up tracing
$ns use-newtrace
set tracefd  [open output/[lindex $argv 2].tr w]
set namtrace [open output/[lindex $argv 2].nam w]
$ns trace-all $tracefd
$ns namtrace-all-wireless $namtrace $opt(x) $opt(y) 
# create  and define the topography object and layout
set topo [new Topography]

$topo load_flatgrid $opt(x) $opt(y)

# create an instance of General Operations Director, which keeps track of nodes and
# node-to-node reachability. The parameter is the total number of nodes in the simulation.

create-god [expr $opt(bottomrow) + 1]

# general node configuration
set chan1 [new $opt(chan)]



$ns node-config -adhocRouting $opt(adhocRouting) \
                 -llType $opt(ll) \
                 -macType $opt(mac) \
                 -ifqType $opt(ifq) \
                 -ifqLen $opt(ifqlen) \
                 -antType $opt(ant) \
                 -propType $opt(prop) \
                 -phyType $opt(netif) \
                 -channel $chan1 \
                 -topoInstance $topo \
                 -wiredRouting OFF \
                 -agentTrace ON \
                 -routerTrace ON \
                 -macTrace ON \
		





#$ns interface-errormodel $em
# create the bottom-row nodes as a node array $rownode(), and the moving node as $mover

# for {set i 0} {$i < $opt(bottomrow)} {incr i} {
#     set rownode($i) [$ns node]
#     $rownode($i) set X_ [expr $i * $opt(spacing)]
#     $rownode($i) set Y_ $opt(brheight)
#     $rownode($i) set Z_ 0
# }


# set mover [$ns node]
# $mover set X_ 0
# $mover set Y_ [expr $opt(mheight) + $opt(brheight)]
# $mover set Z_ 0

# set moverdestX [expr $opt(x) - 1]

# $ns at 0 "$mover setdest $moverdestX [$mover set Y_] $opt(speed)"


#error module
$ns node-config -IncomingErrProc UniformErr \
                -OutgoingErrProc UniformErr \

proc UniformErr {} {
    global my_err_rate
    set err [new ErrorModel]
    $err unit packet
    $err set rate_ $my_err_rate
    return $err
}



set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]

$n0 set X_ 250.0
$n0 set Y_ 450.0
$n0 set Z_ 0.0

$n1 set X_ 175.0
$n1 set Y_ 250.0
$n1 set Z_ 0.0

$n2 set X_ 450.0
$n2 set Y_ 350.0
$n2 set Z_ 0.0

$n3 set X_ 250.0
$n3 set Y_ 50.0
$n3 set Z_ 0.0

$n4 set X_ 450.0
$n4 set Y_ 150.0
$n4 set Z_ 0.0

$n5 set X_ 650.0
$n5 set Y_ 150.0
$n5 set Z_ 0.0

$n6 set X_ 650.0
$n6 set Y_ 350.0
$n6 set Z_ 0.0

$n7 set X_ 850.0
$n7 set Y_ 350.0
$n7 set Z_ 0.0

$n8 set X_ 850.0
$n8 set Y_ 150.0
$n8 set Z_ 0.0




# setup UDP connection, using CBR traffic

# set udp [new Agent/UDP]
# set null [new Agent/Null]
# $ns attach-agent $rownode(0) $udp
# $ns attach-agent $mover $null
# $ns connect $udp $null
# set cbr1 [new Application/Traffic/CBR]
# $cbr1 set packetSize_ 512
# $cbr1 set rate_ 200kb
# $cbr1 attach-agent $udp
# $ns at 0 "$cbr1 start"
# $ns at $opt(finish) "$cbr1 stop"



#creation of agents
set tcp [new Agent/TCP]
set sink [new Agent/TCPSink]

$tcp set packetSize_ $packet_size
$ns attach-agent $n0 $tcp
$ns attach-agent $n7 $sink

$ns connect $tcp $sink

set ftp1 [new Application/FTP]

$ftp1 attach-agent $tcp
$ftp1 set rate_ 100Mb
$ftp1 set type_ CBR

$ns at 0 "$ftp1 start"

set tcp2 [new Agent/TCP]
set sink2 [new Agent/TCPSink]

$tcp2 set packetSize_ $packet_size
$ns attach-agent $n3 $tcp2
$ns attach-agent $n8 $sink2

$ns connect $tcp2 $sink2

set ftp2 [new Application/FTP]
$ftp2 attach-agent $tcp2
$ftp2 set rate_ 100Mb
$ftp2 set type_ CBR

$ns at 0 "$ftp2 start"

set tcp3 [new Agent/TCP]
set sink3 [new Agent/TCPSink]

$tcp3 set packetSize_ $packet_size
$ns attach-agent $n0 $tcp3
$ns attach-agent $n8 $sink3

$ns connect $tcp3 $sink3

set ftp3 [new Application/FTP]
$ftp3 attach-agent $tcp3
$ftp3 set rate_ 100Mb
$ftp3 set type_ CBR

$ns at 0 "$ftp3 start"

set tcp4 [new Agent/TCP]
set sink4 [new Agent/TCPSink]

$tcp4 set packetSize_ $packet_size
$ns attach-agent $n3 $tcp4
$ns attach-agent $n7 $sink4

$ns connect $tcp4 $sink4

set ftp4 [new Application/FTP]
$ftp4 attach-agent $tcp4
$ftp4 set rate_ 100Mb
$ftp4 set type_ CBR

$ns at 0 "$ftp4 start"

#$ns lossmodel $em $n0 $n1

# tell nam the initial node position (taken from node attributes)
# and size (supplied as a parameter)

# for {set i 0} {$i < $opt(bottomrow)} {incr i} {
#     $ns initial_node_pos $rownode($i) 10
# }

# $ns initial_node_pos $mover 20

$ns initial_node_pos $n0 50
$ns initial_node_pos $n1 50
$ns initial_node_pos $n2 50
$ns initial_node_pos $n3 50
$ns initial_node_pos $n4 50
$ns initial_node_pos $n5 50
$ns initial_node_pos $n6 50
$ns initial_node_pos $n7 50
$ns initial_node_pos $n8 50


# set the color of the mover node in nam
# $mover color blue
# $ns at 0.0 "$mover color blue"

$ns at $opt(finish) "finish"

proc finish {} {
    global ns tracefd namtrace
    $ns flush-trace
    close $tracefd
    close $namtrace
    puts stdout "Simulation Finished"
    puts stdout "####################" 
    exit 0
}

# begin simulation

$ns run
