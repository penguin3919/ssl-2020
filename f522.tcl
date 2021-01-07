package require Tk


canvas .mycanvas1 -background white -width 500 -height 500  
pack .mycanvas1

###########################

set m 300

##line##


for {set i 0} {$i <=$m} {incr i} {
set xx($i) -1
}

for {set i 0} {$i <=$m} {incr i} {
set yy($i) -1
}

#puts "$xx(6)"

#set xx(0) 1
set xx(30) 1
set xx(60) 1
set xx(90) 1
set xx(240) 1
set xx(270) 1
set xx(300) 1

#set yy(0) 1
set yy(30) 1
set yy(60) 1
set yy(90) 1
set yy(240) 1
set yy(270) 1
set yy(300) 1


set xxn 0
set yyn 0

set xxx 0
set yyy 0
 

set num 0

for {set i 0} {$i<=$m} {incr i 3} {

if {$xx($i)==1} {
set xxx [expr ($xxx)<<9]

incr xxn
set xxx [expr ($xxx|$i)]
#set xxx [expr ($xxx|$i)<<9]
#
#puts "while i= $i , xxx=$xxx in $num."
#incr num
# 
}
if {$yy($i)==1} {
set yyy [expr ($yyy)<<9]
incr yyn
set yyy [expr ($yyy|$i)]
#set yyy [expr ($yyy|$i)<<9]

puts "while i= $i , yyy=$yyy in $num."
incr num

}

#if {$i==3} {puts "$yyy"}
			}
#puts " $xxx , $yyy"

set xyn [expr $xxn*$yyn]

for {set i 0} {$i <$xyn} {incr i} {
set xy($i) 0
}
#
set yyyt $yyy
#

set cnt 0



for {set i 0} {$i  < $xxn} {incr i} {
	set xxxt [expr $xxx&511]

	for {set k 0} {$k < $yyn} {incr k} {
		set xy($cnt) [expr $xy($cnt)|($yyy&511)]
		set xy($cnt) [expr $xy($cnt)<<9]
		set xy($cnt) [expr $xy($cnt)|$xxxt]
		incr cnt
			#puts "mid2 yyy: $yyy"
		set yyy [expr $yyy>>9]		
					}	
	set xxx [expr $xxx>>9]
	set yyy $yyyt
		#puts "mid xxx: $xxx"
}

for {set i 0} {$i <$xyn} {incr i} {
	puts "xy($i)= $xy($i)"
}

for {set i 0} {$i <$xyn} {incr i} {
	puts "x: [expr $xy($i)&511] , y: [expr ($xy($i)>>9)&511]"
}

puts "##dot##"

set xxyyn [expr $xyn-$yyn-1]
set xxyym [expr $xxyyn-$xxn+2]

#puts "xxyyn: $xxyyn"

for {set i 0} {$i <$xxyyn} {incr i} {
set xxyy($i) 0
}

set yyyt [expr $yyn-1]
set cntt 0

for {set i 0} {$i <$xxyyn} {incr i} {

if { $i==$yyyt} {set yyyt [expr $yyyt+$yyn]} else {

set xytt [expr $i+$yyn+1]
#set xxyy($cntt) [expr [(($xxyy($cntt)|$xy([expr $i+$yyn+1])<<18)|$xy($i)]]
#set xxyy($cntt) [expr ($xxyy($cntt)|$xy($xytt)<<18)|$xy($i)]

set xxyy($cntt) [expr (($xxyy($cntt)|$xy($i))<<18)|$xy($xytt)]
incr cntt 1
					}

}

for {set i 0} {$i <$xxyym} {incr i} {

puts "xxyy($i) : [expr $xxyy($i)&511] , [expr ($xxyy($i)>>9)&511], [expr ($xxyy($i)>>18)&511], [expr ($xxyy($i)>>27)&511]"

} 

for {set i 0} {$i <$xxyym} {incr i} {

#puts "xxyy($i) : [expr $xxyy($i)&511] , [expr ($xxyy($i)>>9)&511], [expr ($xxyy($i)>>18)&511], [expr ($xxyy($i)>>27)&511]"

.mycanvas1 create rectangle [expr $xxyy($i)&511] [expr ($xxyy($i)>>9)&511] [expr ($xxyy($i)>>18)&511] [expr ($xxyy($i)>>27)&511] -fill white -tag bear($i) -width 3

} 

#number
for {set i [expr $xxyym-1]} {$i >=0} {incr i -1} {
#.mycanvas1 create text [expr ($xxyy($i)&511)+10] [expr (($xxyy($i)>>9)&511)+10] -text "$i" -font {Helvetica -15 bold}

set m1 [expr ($xxyy($i)&511)]
set m2 [expr ($xxyy($i)>>9)&511] 
set m3 [expr ($xxyy($i)>>18)&511] 
set m4 [expr ($xxyy($i)>>27)&511]

set aa [expr ($m3-$m1)*($m4-$m2)]

.mycanvas1 create text [expr $m1+10] [expr $m2+10] -text "$i" -font {Helvetica -15 bold}

if {$aa==900} { .mycanvas1 itemconfigure bear($i) -fill {pale green}}


}


#.mycanvas1 itemconfigure bear(0) -fill {pale green}
.mycanvas1 itemconfigure bear(23) -fill {indian red}
.mycanvas1 itemconfigure bear(6) -fill {indian red}
#.mycanvas1 itemconfigure bear(8) -fill {pale green}
#.mycanvas1 itemconfigure bear(4) -fill white

#.mycanvas1 delete bear(1)


