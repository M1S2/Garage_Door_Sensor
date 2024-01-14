<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.7.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="24" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="56" name="wert" color="7" fill="1" visible="no" active="no"/>
<layer number="57" name="tCAD" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
<layer number="100" name="Mechanical" color="7" fill="1" visible="no" active="yes"/>
<layer number="101" name="Gehäuse" color="7" fill="1" visible="no" active="yes"/>
<layer number="102" name="Mittellin" color="7" fill="1" visible="no" active="yes"/>
<layer number="103" name="tMap" color="7" fill="1" visible="yes" active="yes"/>
<layer number="104" name="Name" color="7" fill="1" visible="no" active="yes"/>
<layer number="105" name="tPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="106" name="bPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="107" name="Crop" color="7" fill="1" visible="yes" active="yes"/>
<layer number="108" name="tplace-old" color="7" fill="1" visible="yes" active="yes"/>
<layer number="109" name="ref-old" color="7" fill="1" visible="yes" active="yes"/>
<layer number="110" name="fp0" color="7" fill="1" visible="yes" active="yes"/>
<layer number="111" name="LPC17xx" color="7" fill="1" visible="yes" active="yes"/>
<layer number="112" name="tSilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="113" name="IDFDebug" color="7" fill="1" visible="yes" active="yes"/>
<layer number="116" name="Patch_BOT" color="7" fill="4" visible="yes" active="yes"/>
<layer number="121" name="_tsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="122" name="_bsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="123" name="tTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="124" name="bTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="125" name="_tNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="126" name="_bNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="127" name="_tValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="128" name="_bValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="131" name="tAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="132" name="bAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="144" name="Drill_legend" color="7" fill="1" visible="yes" active="yes"/>
<layer number="150" name="Notes" color="7" fill="1" visible="yes" active="yes"/>
<layer number="151" name="HeatSink" color="7" fill="1" visible="no" active="yes"/>
<layer number="152" name="_bDocu" color="7" fill="1" visible="yes" active="yes"/>
<layer number="153" name="FabDoc1" color="7" fill="1" visible="yes" active="yes"/>
<layer number="154" name="FabDoc2" color="7" fill="1" visible="yes" active="yes"/>
<layer number="155" name="FabDoc3" color="7" fill="1" visible="yes" active="yes"/>
<layer number="199" name="Contour" color="7" fill="1" visible="yes" active="yes"/>
<layer number="200" name="200bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="201" name="201bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="202" name="202bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="203" name="203bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="204" name="204bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="205" name="205bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="206" name="206bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="207" name="207bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="208" name="208bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="209" name="209bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="210" name="210bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="211" name="211bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="212" name="212bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="213" name="213bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="214" name="214bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="215" name="215bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="216" name="216bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="217" name="217bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="218" name="218bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="219" name="219bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="220" name="220bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="221" name="221bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="222" name="222bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="223" name="223bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="224" name="224bmp" color="7" fill="1" visible="no" active="no"/>
<layer number="225" name="225bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="226" name="226bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="227" name="227bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="228" name="228bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="229" name="229bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="230" name="230bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="231" name="231bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="248" name="Housing" color="7" fill="1" visible="yes" active="yes"/>
<layer number="249" name="Edge" color="7" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="7" fill="1" visible="no" active="yes"/>
<layer number="251" name="SMDround" color="7" fill="1" visible="no" active="yes"/>
<layer number="254" name="OrgLBR" color="7" fill="1" visible="no" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="OwnDevices" urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA">
<packages>
<package name="ESP-12F" urn="urn:adsk.eagle:footprint:42764228/1" library_version="24">
<wire x1="-8.5" y1="12.5" x2="8.5" y2="12.5" width="0.2" layer="21"/>
<wire x1="-8.5" y1="-11.5" x2="-8.5" y2="-12.5" width="0.2" layer="21"/>
<wire x1="-8.5" y1="-12.5" x2="-6" y2="-12.5" width="0.2" layer="21"/>
<wire x1="8.5" y1="-12.5" x2="6" y2="-12.5" width="0.2" layer="21"/>
<wire x1="8.5" y1="-12.5" x2="8.5" y2="-11.5" width="0.2" layer="21"/>
<wire x1="-8.5" y1="12.5" x2="-8.5" y2="4.5" width="0.2" layer="21"/>
<wire x1="8.5" y1="12.5" x2="8.5" y2="4.5" width="0.2" layer="21"/>
<wire x1="-8" y1="12" x2="8" y2="12" width="0.2" layer="48"/>
<wire x1="8" y1="12" x2="8" y2="-12" width="0.2" layer="48"/>
<wire x1="8" y1="-12" x2="-8" y2="-12" width="0.2" layer="48"/>
<wire x1="-8" y1="-12" x2="-8" y2="12" width="0.2" layer="48"/>
<text x="-8.26053125" y="13.3439" size="0.889596875" layer="25" ratio="11">&gt;NAME</text>
<text x="3.81645" y="13.3576" size="0.890503125" layer="27" ratio="11">&gt;VALUE</text>
<smd name="15" x="8" y="-10.52" dx="2" dy="1.2" layer="1"/>
<smd name="16" x="8" y="-8.52" dx="2" dy="1.2" layer="1"/>
<smd name="17" x="8" y="-6.52" dx="2" dy="1.2" layer="1"/>
<smd name="18" x="8" y="-4.52" dx="2" dy="1.2" layer="1"/>
<smd name="19" x="8" y="-2.52" dx="2" dy="1.2" layer="1"/>
<smd name="20" x="8" y="-0.52" dx="2" dy="1.2" layer="1"/>
<smd name="21" x="8" y="1.48" dx="2" dy="1.2" layer="1"/>
<smd name="22" x="8" y="3.48" dx="2" dy="1.2" layer="1"/>
<smd name="1" x="-8" y="3.48" dx="2" dy="1.2" layer="1"/>
<smd name="3" x="-8" y="-0.52" dx="2" dy="1.2" layer="1"/>
<smd name="2" x="-8" y="1.48" dx="2" dy="1.2" layer="1"/>
<smd name="4" x="-8" y="-2.52" dx="2" dy="1.2" layer="1"/>
<smd name="5" x="-8" y="-4.52" dx="2" dy="1.2" layer="1"/>
<smd name="6" x="-8" y="-6.52" dx="2" dy="1.2" layer="1"/>
<smd name="7" x="-8" y="-8.52" dx="2" dy="1.2" layer="1"/>
<smd name="8" x="-8" y="-10.52" dx="2" dy="1.2" layer="1"/>
<smd name="14" x="5" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
<smd name="13" x="3" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
<smd name="12" x="1" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
<smd name="11" x="-1" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
<smd name="10" x="-3" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
<smd name="9" x="-5" y="-12" dx="2" dy="1.2" layer="1" rot="R90"/>
</package>
<package name="B3F-10XX" urn="urn:adsk.eagle:footprint:21817510/2" library_version="30">
<description>&lt;b&gt;OMRON SWITCH&lt;/b&gt;</description>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.0508" layer="51"/>
<wire x1="1.27" y1="-1.27" x2="-1.27" y2="-1.27" width="0.0508" layer="51"/>
<wire x1="1.27" y1="-1.27" x2="1.27" y2="1.27" width="0.0508" layer="51"/>
<wire x1="-1.27" y1="1.27" x2="1.27" y2="1.27" width="0.0508" layer="51"/>
<wire x1="-3" y1="3.1" x2="3" y2="3.1" width="0.127" layer="21"/>
<wire x1="3" y1="3.1" x2="3" y2="-3.1" width="0.127" layer="21"/>
<wire x1="3" y1="-3.1" x2="-3" y2="-3.1" width="0.127" layer="21"/>
<wire x1="-3" y1="-3.1" x2="-3" y2="3.1" width="0.127" layer="21"/>
<circle x="0" y="0" radius="1.778" width="0.1524" layer="21"/>
<circle x="-2.3" y="-2.3" radius="0.508" width="0.1524" layer="51"/>
<circle x="2.3" y="-2.3" radius="0.508" width="0.1524" layer="51"/>
<circle x="2.3" y="2.3" radius="0.508" width="0.1524" layer="51"/>
<circle x="-2.3" y="2.3" radius="0.508" width="0.1524" layer="51"/>
<circle x="0" y="0" radius="0.635" width="0.0508" layer="51"/>
<circle x="0" y="0" radius="0.254" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="1.016" shape="long"/>
<pad name="2" x="2.54" y="0" drill="1.016" shape="long"/>
<text x="-3.048" y="3.683" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.048" y="-4.445" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-4.318" y="-0.889" size="1.27" layer="51" ratio="10">1</text>
<text x="3.683" y="-0.889" size="1.27" layer="51" ratio="10">2</text>
</package>
<package name="SWITCH_SPDT" urn="urn:adsk.eagle:footprint:41593437/2" library_version="32">
<wire x1="-6.4" y1="4" x2="6.4" y2="4" width="0.1524" layer="21"/>
<wire x1="6.4" y1="4" x2="6.4" y2="-4" width="0.1524" layer="21"/>
<wire x1="6.4" y1="-4" x2="-6.4" y2="-4" width="0.1524" layer="21"/>
<wire x1="-6.4" y1="-4" x2="-6.4" y2="4" width="0.1524" layer="21"/>
<pad name="P$2" x="0" y="0" drill="2" shape="long" rot="R90"/>
<pad name="P$3" x="4.25" y="0" drill="2" shape="long" rot="R90"/>
<pad name="P$1" x="-4.25" y="0" drill="2" shape="long" rot="R90"/>
<text x="-6.35" y="5.08" size="1.778" layer="25">&gt;NAME</text>
<text x="-6.35" y="-6.35" size="1.778" layer="27">&gt;VALUE</text>
</package>
<package name="MICROSWITCH_SPDT" urn="urn:adsk.eagle:footprint:42807262/1" library_version="32">
<pad name="P$1" x="-2.54" y="0" drill="0.6" shape="long" rot="R90"/>
<pad name="P$2" x="0" y="0" drill="0.6" shape="long" rot="R90"/>
<pad name="P$3" x="2.54" y="0" drill="0.6" shape="long" rot="R90"/>
<wire x1="-4.25" y1="1.75" x2="4.25" y2="1.75" width="0.1524" layer="21"/>
<wire x1="4.25" y1="1.75" x2="4.25" y2="-1.75" width="0.1524" layer="21"/>
<wire x1="4.25" y1="-1.75" x2="-4.25" y2="-1.75" width="0.1524" layer="21"/>
<wire x1="-4.25" y1="-1.75" x2="-4.25" y2="1.75" width="0.1524" layer="21"/>
<text x="-3.81" y="2.54" size="1.778" layer="25">&gt;NAME</text>
<text x="-3.81" y="-3.81" size="1.778" layer="27">&gt;VALUE</text>
</package>
<package name="JST-XH-02-PACKAGE-LONG-PAD" urn="urn:adsk.eagle:footprint:42772298/1" library_version="34">
<description>&lt;b&gt;JST XH Connector Long Pads (Package)&lt;/b&gt;&lt;p&gt;

Wire to board connector.

Pitch: 2,54 mm, (0.100")&lt;p&gt;
Number of pins: &lt;b&gt;2&lt;/b&gt;&lt;b&gt;&lt;P&gt;

&lt;b&gt;Created by Rembrandt Electronics&lt;/b&gt;&lt;p&gt;
&lt;b&gt;www.rembrandtelectronics.com&lt;/b&gt;&lt;p&gt;</description>
<wire x1="3.7" y1="-2.3575" x2="3.7" y2="3.3925" width="0.254" layer="21"/>
<wire x1="3.7" y1="3.3925" x2="-3.7" y2="3.3925" width="0.254" layer="21"/>
<wire x1="-3.7" y1="3.3925" x2="-3.7" y2="-2.3575" width="0.254" layer="21"/>
<wire x1="-3.7" y1="-2.3575" x2="3.7" y2="-2.3575" width="0.254" layer="21"/>
<wire x1="-1.3" y1="-2.3" x2="-1.3" y2="-1.8" width="0.2" layer="21"/>
<wire x1="1.3" y1="-2.3" x2="1.3" y2="-1.8" width="0.2" layer="21"/>
<wire x1="-1.3" y1="-1.8" x2="1.3" y2="-1.8" width="0.2" layer="21"/>
<pad name="2" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="1" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-4.255" y="-2.04" size="1.016" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="-3.3025" y="3.8925" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<text x="-2.8675" y="-1.4875" size="1.016" layer="51" ratio="10">1</text>
</package>
<package name="JST-XH-02-EDGE" urn="urn:adsk.eagle:footprint:42837630/1" library_version="34">
<smd name="P$1" x="-1.27" y="0" dx="4" dy="1.27" layer="1" rot="R90"/>
<smd name="P$2" x="1.27" y="0" dx="4" dy="1.27" layer="1" rot="R90"/>
<wire x1="-3.7" y1="2" x2="3.7" y2="2" width="0.1524" layer="21"/>
<wire x1="-3.7" y1="2" x2="-3.7" y2="9" width="0.1524" layer="21"/>
<wire x1="3.7" y1="2" x2="3.7" y2="9" width="0.1524" layer="21"/>
<wire x1="-3.7" y1="9" x2="-2" y2="9" width="0.1524" layer="21"/>
<wire x1="-2" y1="9" x2="2" y2="9" width="0.1524" layer="21"/>
<wire x1="2" y1="9" x2="3.7" y2="9" width="0.1524" layer="21"/>
<wire x1="-2" y1="9" x2="-2" y2="5" width="0.1524" layer="21"/>
<wire x1="-2" y1="5" x2="2" y2="5" width="0.1524" layer="21"/>
<wire x1="2" y1="5" x2="2" y2="9" width="0.1524" layer="21"/>
<text x="3.04" y="0.445" size="1.016" layer="25" ratio="10">&gt;NAME</text>
<text x="3.0475" y="-1.1875" size="1.016" layer="27" ratio="10">&gt;VALUE</text>
<text x="-2.8675" y="-0.2175" size="1.016" layer="51" ratio="10">1</text>
</package>
</packages>
<packages3d>
<package3d name="ESP-12F" urn="urn:adsk.eagle:package:42764230/1" type="model">
<packageinstances>
<packageinstance name="ESP-12F"/>
</packageinstances>
</package3d>
<package3d name="B3F-10XX" urn="urn:adsk.eagle:package:21817514/3" type="model">
<description>&lt;b&gt;OMRON SWITCH&lt;/b&gt;</description>
<packageinstances>
<packageinstance name="B3F-10XX"/>
</packageinstances>
</package3d>
<package3d name="SWITCH_SPDT" urn="urn:adsk.eagle:package:41593439/2" type="model">
<packageinstances>
<packageinstance name="SWITCH_SPDT"/>
</packageinstances>
</package3d>
<package3d name="MICROSWITCH_SPDT" urn="urn:adsk.eagle:package:42807267/1" type="model">
<packageinstances>
<packageinstance name="MICROSWITCH_SPDT"/>
</packageinstances>
</package3d>
<package3d name="JST-XH-02" urn="urn:adsk.eagle:package:42772299/1" type="model">
<description>&lt;b&gt;JST XH Connector Long Pads (Package)&lt;/b&gt;&lt;p&gt;

Wire to board connector.

Pitch: 2,54 mm, (0.100")&lt;p&gt;
Number of pins: &lt;b&gt;2&lt;/b&gt;&lt;b&gt;&lt;P&gt;

&lt;b&gt;Created by Rembrandt Electronics&lt;/b&gt;&lt;p&gt;
&lt;b&gt;www.rembrandtelectronics.com&lt;/b&gt;&lt;p&gt;</description>
<packageinstances>
<packageinstance name="JST-XH-02-PACKAGE-LONG-PAD"/>
</packageinstances>
</package3d>
<package3d name="JST-XH-02-EDGE" urn="urn:adsk.eagle:package:42837631/1" type="model">
<packageinstances>
<packageinstance name="JST-XH-02-EDGE"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="ESP-12F" urn="urn:adsk.eagle:symbol:42764227/1" library_version="24">
<wire x1="-12.7" y1="20.32" x2="-12.7" y2="-20.32" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-20.32" x2="12.7" y2="-20.32" width="0.254" layer="94"/>
<wire x1="12.7" y1="-20.32" x2="12.7" y2="20.32" width="0.254" layer="94"/>
<wire x1="12.7" y1="20.32" x2="-12.7" y2="20.32" width="0.254" layer="94"/>
<text x="-12.7254" y="22.9057" size="1.272540625" layer="95" ratio="11">&gt;NAME</text>
<text x="-12.7237" y="-22.9026" size="1.27236875" layer="96" ratio="11">&gt;VALUE</text>
<pin name="RST" x="-17.78" y="17.78" length="middle"/>
<pin name="ADC" x="17.78" y="-2.54" length="middle" rot="R180"/>
<pin name="EN" x="-17.78" y="15.24" length="middle"/>
<pin name="GPIO16" x="-17.78" y="-17.78" length="middle"/>
<pin name="GPIO14" x="-17.78" y="-12.7" length="middle"/>
<pin name="GPIO12" x="-17.78" y="-7.62" length="middle"/>
<pin name="GPIO13" x="-17.78" y="-10.16" length="middle"/>
<pin name="VCC" x="-17.78" y="10.16" length="middle"/>
<pin name="CS0" x="17.78" y="-17.78" length="middle" rot="R180"/>
<pin name="MISO" x="17.78" y="-10.16" length="middle" rot="R180"/>
<pin name="IO9" x="-17.78" y="0" length="middle"/>
<pin name="IO10" x="-17.78" y="-2.54" length="middle"/>
<pin name="MOSI" x="17.78" y="-12.7" length="middle" rot="R180"/>
<pin name="SCLK" x="17.78" y="-15.24" length="middle" rot="R180"/>
<pin name="GND" x="-17.78" y="5.08" length="middle"/>
<pin name="GPIO15" x="-17.78" y="-15.24" length="middle"/>
<pin name="GPIO2" x="17.78" y="7.62" length="middle" rot="R180"/>
<pin name="GPIO0" x="17.78" y="10.16" length="middle" rot="R180"/>
<pin name="GPIO4" x="17.78" y="5.08" length="middle" rot="R180"/>
<pin name="GPIO5" x="17.78" y="2.54" length="middle" rot="R180"/>
<pin name="RXD" x="17.78" y="15.24" length="middle" rot="R180"/>
<pin name="TXD" x="17.78" y="17.78" length="middle" rot="R180"/>
</symbol>
<symbol name="TS2" urn="urn:adsk.eagle:symbol:21817504/1" library_version="30">
<wire x1="0" y1="1.905" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="-4.445" y1="1.905" x2="-3.175" y2="1.905" width="0.254" layer="94"/>
<wire x1="-4.445" y1="-1.905" x2="-3.175" y2="-1.905" width="0.254" layer="94"/>
<wire x1="-4.445" y1="1.905" x2="-4.445" y2="0" width="0.254" layer="94"/>
<wire x1="-4.445" y1="0" x2="-4.445" y2="-1.905" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.905" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="0" x2="-0.635" y2="0" width="0.1524" layer="94"/>
<wire x1="-4.445" y1="0" x2="-3.175" y2="0" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="1.905" width="0.254" layer="94"/>
<circle x="0" y="-2.54" radius="0.127" width="0.4064" layer="94"/>
<circle x="0" y="2.54" radius="0.127" width="0.4064" layer="94"/>
<text x="-6.35" y="-2.54" size="1.778" layer="95" rot="R90">&gt;NAME</text>
<text x="-3.81" y="3.175" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="P" x="0" y="-5.08" visible="pad" length="short" direction="pas" swaplevel="2" rot="R90"/>
<pin name="S" x="0" y="5.08" visible="pad" length="short" direction="pas" swaplevel="1" rot="R270"/>
</symbol>
<symbol name="SWITCH_SPDT" urn="urn:adsk.eagle:symbol:41593436/2" library_version="32">
<pin name="P$1" x="-6.35" y="2.54" visible="off" length="short" direction="pas"/>
<pin name="P$3" x="-6.35" y="-2.54" visible="off" length="short" direction="pas"/>
<pin name="P$2" x="5.08" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<wire x1="-1.27" y1="2.54" x2="-1.27" y2="1.27" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-1.27" y2="-1.27" width="0.4064" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-3.81" y2="2.54" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-3.81" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.665009375" y2="-1.62206875" width="0.4064" layer="94"/>
<text x="-2.54" y="7.62" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
</symbol>
<symbol name="M" urn="urn:adsk.eagle:symbol:42772296/1" library_version="34">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
<symbol name="MV" urn="urn:adsk.eagle:symbol:42772297/1" library_version="34">
<wire x1="1.27" y1="0" x2="0" y2="0" width="0.6096" layer="94"/>
<text x="2.54" y="-0.762" size="1.524" layer="95">&gt;NAME</text>
<text x="-0.762" y="1.397" size="1.778" layer="96">&gt;VALUE</text>
<pin name="S" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ESP-12F" urn="urn:adsk.eagle:component:42764232/1" prefix="U" library_version="24">
<description>317060163 &lt;a href="https://pricing.snapeda.com/parts/ESP-12F/AI-Thinker/view-part?ref=eda"&gt;Check availability&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="ESP-12F" x="0" y="0"/>
</gates>
<devices>
<device name="" package="ESP-12F">
<connects>
<connect gate="G$1" pin="ADC" pad="2"/>
<connect gate="G$1" pin="CS0" pad="9"/>
<connect gate="G$1" pin="EN" pad="3"/>
<connect gate="G$1" pin="GND" pad="15"/>
<connect gate="G$1" pin="GPIO0" pad="18"/>
<connect gate="G$1" pin="GPIO12" pad="6"/>
<connect gate="G$1" pin="GPIO13" pad="7"/>
<connect gate="G$1" pin="GPIO14" pad="5"/>
<connect gate="G$1" pin="GPIO15" pad="16"/>
<connect gate="G$1" pin="GPIO16" pad="4"/>
<connect gate="G$1" pin="GPIO2" pad="17"/>
<connect gate="G$1" pin="GPIO4" pad="19"/>
<connect gate="G$1" pin="GPIO5" pad="20"/>
<connect gate="G$1" pin="IO10" pad="12"/>
<connect gate="G$1" pin="IO9" pad="11"/>
<connect gate="G$1" pin="MISO" pad="10"/>
<connect gate="G$1" pin="MOSI" pad="13"/>
<connect gate="G$1" pin="RST" pad="1"/>
<connect gate="G$1" pin="RXD" pad="21"/>
<connect gate="G$1" pin="SCLK" pad="14"/>
<connect gate="G$1" pin="TXD" pad="22"/>
<connect gate="G$1" pin="VCC" pad="8"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:42764230/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Not in stock"/>
<attribute name="CHECK_PRICES" value="https://www.snapeda.com/parts/ESP-12F/AI-Thinker/view-part/?ref=eda"/>
<attribute name="DESCRIPTION" value=" WiFi Module "/>
<attribute name="MF" value="AI-Thinker"/>
<attribute name="MP" value="ESP-12F"/>
<attribute name="PACKAGE" value="Package "/>
<attribute name="PRICE" value="None"/>
<attribute name="SNAPEDA_LINK" value="https://www.snapeda.com/parts/ESP-12F/AI-Thinker/view-part/?ref=snap"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="KURZHUBTASTER" urn="urn:adsk.eagle:component:21817518/3" prefix="S" uservalue="yes" library_version="30">
<description>&lt;b&gt;OMRON SWITCH&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="TS2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="B3F-10XX">
<connects>
<connect gate="1" pin="P" pad="1"/>
<connect gate="1" pin="S" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:21817514/3"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SWITCH_SPDT" urn="urn:adsk.eagle:component:41593442/3" library_version="32">
<description>https://www.roboter-bausatz.de/p/kippschalter-spdt-3a-250v-mts-102-ein-ein?number=RBS15499
&lt;br&gt;&lt;br&gt;
Kleiner Kippschalter für DIY-Elektronik, RC-Modellbau oder Auto. Durch die Schalterstellungen Ein-Ein (Wechselschalter) können mit diesem Kippschalter zwei Stromkreise geschaltet werden. In jeder Position wird ein Stromkreis geöffnet und der andere gleichzeitig geschlossen. Der Schalter benötigt zum Einbau eine Frontplatte oder eine 6mm Bohrung.
&lt;br&gt;&lt;br&gt;
Details&lt;br&gt;
Schaltleistung: 3A/250V&lt;br&gt;
Kontaktwiderstand: &lt; 20 mOhm&lt;br&gt;
Isolationswiderstand: &gt; 1000 MOhm&lt;br&gt;
Durchschlagfestigkeit: 1500V~ (1 min.)&lt;br&gt;
Betriebstemperatur: -25~+85°C&lt;br&gt;
Schaltzyklen: 10.000&lt;br&gt;
MTS-102&lt;br&gt;
Schaltzustand: EIN - EIN&lt;br&gt;
Typ: SPDT&lt;br&gt;
Pins: 3</description>
<gates>
<gate name="G$1" symbol="SWITCH_SPDT" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SWITCH_SPDT">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:41593439/2"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="MICRO" package="MICROSWITCH_SPDT">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:42807267/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JST-XH-02-PIN" urn="urn:adsk.eagle:component:42772301/2" prefix="X" library_version="34">
<description>&lt;b&gt;JST XH Connector 2 Pin&lt;/b&gt;&lt;p&gt;

Wire to board connector.

Pitch: 2,54 mm, (0.100")&lt;p&gt;
Number of pins: &lt;b&gt;2&lt;/b&gt;&lt;b&gt;&lt;P&gt;

&lt;b&gt;Created by Rembrandt Electronics&lt;/b&gt;&lt;p&gt;
&lt;b&gt;www.rembrandtelectronics.com&lt;/b&gt;&lt;p&gt;</description>
<gates>
<gate name="-2" symbol="M" x="2.54" y="15.24" addlevel="always" swaplevel="1"/>
<gate name="-1" symbol="MV" x="2.54" y="17.78" addlevel="always" swaplevel="1"/>
</gates>
<devices>
<device name="-LONG-PAD" package="JST-XH-02-PACKAGE-LONG-PAD">
<connects>
<connect gate="-1" pin="S" pad="1"/>
<connect gate="-2" pin="S" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:42772299/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="-EDGE" package="JST-XH-02-EDGE">
<connects>
<connect gate="-1" pin="S" pad="P$1"/>
<connect gate="-2" pin="S" pad="P$2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:42837631/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="frames" urn="urn:adsk.eagle:library:229">
<description>&lt;b&gt;Frames for Sheet and Layout&lt;/b&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="DINA4_L" urn="urn:adsk.eagle:symbol:13867/1" library_version="1">
<frame x1="0" y1="0" x2="264.16" y2="180.34" columns="4" rows="4" layer="94" border-left="no" border-top="no" border-right="no" border-bottom="no"/>
</symbol>
<symbol name="DOCFIELD" urn="urn:adsk.eagle:symbol:13864/1" library_version="1">
<wire x1="0" y1="0" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="87.63" y2="15.24" width="0.1016" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="71.12" y2="5.08" width="0.1016" layer="94"/>
<wire x1="0" y1="5.08" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="101.6" y1="15.24" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="71.12" y2="0" width="0.1016" layer="94"/>
<wire x1="71.12" y1="5.08" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="71.12" y1="0" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="87.63" y2="5.08" width="0.1016" layer="94"/>
<wire x1="87.63" y1="15.24" x2="0" y2="15.24" width="0.1016" layer="94"/>
<wire x1="87.63" y1="5.08" x2="101.6" y2="5.08" width="0.1016" layer="94"/>
<wire x1="101.6" y1="5.08" x2="101.6" y2="0" width="0.1016" layer="94"/>
<wire x1="0" y1="15.24" x2="0" y2="22.86" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="35.56" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="101.6" y2="22.86" width="0.1016" layer="94"/>
<wire x1="0" y1="22.86" x2="0" y2="35.56" width="0.1016" layer="94"/>
<wire x1="101.6" y1="22.86" x2="101.6" y2="15.24" width="0.1016" layer="94"/>
<text x="1.27" y="1.27" size="2.54" layer="94">Date:</text>
<text x="12.7" y="1.27" size="2.54" layer="94">&gt;LAST_DATE_TIME</text>
<text x="72.39" y="1.27" size="2.54" layer="94">Sheet:</text>
<text x="86.36" y="1.27" size="2.54" layer="94">&gt;SHEET</text>
<text x="88.9" y="11.43" size="2.54" layer="94">REV:</text>
<text x="1.27" y="19.05" size="2.54" layer="94">TITLE:</text>
<text x="1.27" y="11.43" size="2.54" layer="94">Document Number:</text>
<text x="17.78" y="19.05" size="2.54" layer="94">&gt;DRAWING_NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="DINA4_L" urn="urn:adsk.eagle:component:13919/1" prefix="FRAME" uservalue="yes" library_version="1">
<description>&lt;b&gt;FRAME&lt;/b&gt;&lt;p&gt;
DIN A4, landscape with extra doc field</description>
<gates>
<gate name="G$1" symbol="DINA4_L" x="0" y="0"/>
<gate name="G$2" symbol="DOCFIELD" x="162.56" y="0" addlevel="must"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Connector">
<description>&lt;b&gt;Pin Headers,Terminal blocks, D-Sub, Backplane, FFC/FPC, Socket</description>
<packages>
<package name="2X03" urn="urn:adsk.eagle:footprint:22348/1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.81" y1="-1.905" x2="-3.175" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-2.54" x2="-1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="-0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-2.54" x2="1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-1.905" x2="-3.81" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="1.905" x2="-3.175" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="2.54" x2="-1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-1.27" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="1.905" x2="-0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="2.54" x2="0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="2.54" x2="1.27" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="1.905" x2="-1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.905" x2="1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-2.54" x2="0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-2.54" x2="-1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-2.54" x2="3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.27" y1="1.905" x2="1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.54" x2="3.175" y2="2.54" width="0.1524" layer="21"/>
<wire x1="3.175" y1="2.54" x2="3.81" y2="1.905" width="0.1524" layer="21"/>
<wire x1="3.81" y1="1.905" x2="3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="3.175" y2="-2.54" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-2.54" y="1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="0" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="0" y="1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="2.54" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="2.54" y="1.27" drill="1.016" shape="octagon"/>
<text x="-3.81" y="3.175" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-4.445" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-2.794" y1="-1.524" x2="-2.286" y2="-1.016" layer="51"/>
<rectangle x1="-2.794" y1="1.016" x2="-2.286" y2="1.524" layer="51"/>
<rectangle x1="-0.254" y1="1.016" x2="0.254" y2="1.524" layer="51"/>
<rectangle x1="-0.254" y1="-1.524" x2="0.254" y2="-1.016" layer="51"/>
<rectangle x1="2.286" y1="1.016" x2="2.794" y2="1.524" layer="51"/>
<rectangle x1="2.286" y1="-1.524" x2="2.794" y2="-1.016" layer="51"/>
</package>
<package name="2X03/90" urn="urn:adsk.eagle:footprint:22349/1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.81" y1="-1.905" x2="-1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="6.985" x2="-2.54" y2="1.27" width="0.762" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="6.985" x2="0" y2="1.27" width="0.762" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-1.905" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="6.985" x2="2.54" y2="1.27" width="0.762" layer="21"/>
<pad name="2" x="-2.54" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="4" x="0" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="6" x="2.54" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="1" x="-2.54" y="-6.35" drill="1.016" shape="octagon"/>
<pad name="3" x="0" y="-6.35" drill="1.016" shape="octagon"/>
<pad name="5" x="2.54" y="-6.35" drill="1.016" shape="octagon"/>
<text x="-4.445" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="5.715" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.921" y1="0.635" x2="-2.159" y2="1.143" layer="21"/>
<rectangle x1="-0.381" y1="0.635" x2="0.381" y2="1.143" layer="21"/>
<rectangle x1="2.159" y1="0.635" x2="2.921" y2="1.143" layer="21"/>
<rectangle x1="-2.921" y1="-2.921" x2="-2.159" y2="-1.905" layer="21"/>
<rectangle x1="-0.381" y1="-2.921" x2="0.381" y2="-1.905" layer="21"/>
<rectangle x1="-2.921" y1="-5.461" x2="-2.159" y2="-4.699" layer="21"/>
<rectangle x1="-2.921" y1="-4.699" x2="-2.159" y2="-2.921" layer="51"/>
<rectangle x1="-0.381" y1="-4.699" x2="0.381" y2="-2.921" layer="51"/>
<rectangle x1="-0.381" y1="-5.461" x2="0.381" y2="-4.699" layer="21"/>
<rectangle x1="2.159" y1="-2.921" x2="2.921" y2="-1.905" layer="21"/>
<rectangle x1="2.159" y1="-5.461" x2="2.921" y2="-4.699" layer="21"/>
<rectangle x1="2.159" y1="-4.699" x2="2.921" y2="-2.921" layer="51"/>
</package>
</packages>
<packages3d>
<package3d name="2X03" urn="urn:adsk.eagle:package:22462/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X03"/>
</packageinstances>
</package3d>
<package3d name="2X03/90" urn="urn:adsk.eagle:package:22464/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X03/90"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="PINH2X3" urn="urn:adsk.eagle:symbol:22347/1">
<wire x1="-6.35" y1="-5.08" x2="8.89" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="8.89" y1="-5.08" x2="8.89" y2="5.08" width="0.4064" layer="94"/>
<wire x1="8.89" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-5.08" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="5.08" y="2.54" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
<pin name="3" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="4" x="5.08" y="0" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
<pin name="5" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="6" x="5.08" y="-2.54" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-2X3" urn="urn:adsk.eagle:component:16494865/4" prefix="JP">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINH2X3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="2X03">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
<connect gate="A" pin="6" pad="6"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22462/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Headers" constant="no"/>
<attribute name="DESCRIPTION" value="Header, Straight, 2x3 Position" constant="no"/>
<attribute name="MANUFACTURER" value="Generic" constant="no"/>
<attribute name="MPN" value="Generic" constant="no"/>
<attribute name="OPERATING_TEMPERATURE" value="Generic" constant="no"/>
<attribute name="PART_STATUS" value="Active" constant="no"/>
<attribute name="PITCH" value="0.100&quot; (2.54mm)" constant="no"/>
<attribute name="ROHS_COMPLIANCE" value="Generic" constant="no"/>
<attribute name="SERIES" value="Generic" constant="no"/>
<attribute name="SUB-CATEGORY" value="Headers, Male Pins" constant="no"/>
<attribute name="TYPE" value="Board to Board or Cable, Unshrouded, Through Hole, Straight" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="2X03/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
<connect gate="A" pin="6" pad="6"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22464/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Headers" constant="no"/>
<attribute name="DESCRIPTION" value="Header, Right Angle, 2x3 Position" constant="no"/>
<attribute name="MANUFACTURER" value="Generic" constant="no"/>
<attribute name="MPN" value="Generic" constant="no"/>
<attribute name="OPERATING_TEMPERATURE" value="Generic" constant="no"/>
<attribute name="PART_STATUS" value="Active" constant="no"/>
<attribute name="PITCH" value="0.100&quot; (2.54mm)" constant="no"/>
<attribute name="ROHS_COMPLIANCE" value="Generic" constant="no"/>
<attribute name="SERIES" value="Generic" constant="no"/>
<attribute name="SUB-CATEGORY" value="Headers, Male Pins" constant="no"/>
<attribute name="TYPE" value="Board to Board or Cable, Unshrouded, Through Hole, Right Angle" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Power_Symbols">
<description>&lt;B&gt;Supply &amp; Ground symbols</description>
<packages>
</packages>
<symbols>
<symbol name="GND-BAR" urn="urn:adsk.eagle:symbol:16502372/2">
<description>Ground (GND) Bar</description>
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="0" y="-1.905" size="1.778" layer="96" align="center">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="+3.3V" urn="urn:adsk.eagle:symbol:18498252/2">
<description>3.3 Volt (3V3) Bar</description>
<wire x1="1.905" y1="2.54" x2="-1.905" y2="2.54" width="0.254" layer="94"/>
<text x="-0.127" y="3.048" size="1.778" layer="96" align="bottom-center">&gt;VALUE</text>
<pin name="+3.3V" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="+5V" urn="urn:adsk.eagle:symbol:18498226/2">
<description>5 Volt (5V0) Bar</description>
<wire x1="1.905" y1="2.54" x2="-1.905" y2="2.54" width="0.254" layer="94"/>
<text x="-0.127" y="3.048" size="1.778" layer="96" align="bottom-center">&gt;VALUE</text>
<pin name="+5V" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND-BAR" urn="urn:adsk.eagle:component:16502419/5" prefix="SUPPLY" uservalue="yes">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt; - Ground (GND) Bar</description>
<gates>
<gate name="G$1" symbol="GND-BAR" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Supply" constant="no"/>
<attribute name="VALUE" value="GND" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+3.3V" urn="urn:adsk.eagle:component:16502431/7" prefix="SUPPLY" uservalue="yes">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;  3.3 Volt (3V3) Bar</description>
<gates>
<gate name="G$1" symbol="+3.3V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Supply" constant="no"/>
<attribute name="VALUE" value="+3.3V" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+5V" urn="urn:adsk.eagle:component:16502432/7" prefix="SUPPLY" uservalue="yes">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;  5 Volt (5V0) Bar</description>
<gates>
<gate name="G$1" symbol="+5V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name="">
<attribute name="CATEGORY" value="Supply" constant="no"/>
<attribute name="VALUE" value="+5V" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="IC_Power-Management">
<description>&lt;B&gt;Voltage Regulators, Drivers, Switching controllers</description>
<packages>
<package name="SOT230P700X170-4" urn="urn:adsk.eagle:footprint:16494935/1">
<description>4-SOT223, 2.30 mm pitch, 7.00 mm span, 6.50 X 3.50 X 1.70 mm body
&lt;p&gt;4-pin SOT223 package with 2.30 mm pitch, 7.00 mm span with body size 6.50 X 3.50 X 1.70 mm&lt;/p&gt;</description>
<circle x="-2.9276" y="3.253" radius="0.25" width="0" layer="21"/>
<wire x1="-1.85" y1="3.003" x2="-1.85" y2="3.35" width="0.12" layer="21"/>
<wire x1="-1.85" y1="3.35" x2="1.85" y2="3.35" width="0.12" layer="21"/>
<wire x1="1.85" y1="3.35" x2="1.85" y2="1.8486" width="0.12" layer="21"/>
<wire x1="-1.85" y1="-3.003" x2="-1.85" y2="-3.35" width="0.12" layer="21"/>
<wire x1="-1.85" y1="-3.35" x2="1.85" y2="-3.35" width="0.12" layer="21"/>
<wire x1="1.85" y1="-3.35" x2="1.85" y2="-1.8486" width="0.12" layer="21"/>
<wire x1="1.85" y1="-3.35" x2="-1.85" y2="-3.35" width="0.12" layer="51"/>
<wire x1="-1.85" y1="-3.35" x2="-1.85" y2="3.35" width="0.12" layer="51"/>
<wire x1="-1.85" y1="3.35" x2="1.85" y2="3.35" width="0.12" layer="51"/>
<wire x1="1.85" y1="3.35" x2="1.85" y2="-3.35" width="0.12" layer="51"/>
<smd name="1" x="-3.075" y="2.3" dx="1.8603" dy="0.8979" layer="1"/>
<smd name="2" x="-3.075" y="0" dx="1.8603" dy="0.8979" layer="1"/>
<smd name="3" x="-3.075" y="-2.3" dx="1.8603" dy="0.8979" layer="1"/>
<smd name="4" x="3.075" y="0" dx="1.8603" dy="3.1891" layer="1"/>
<text x="0" y="4.138" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-3.985" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="SOT230P700X170-4" urn="urn:adsk.eagle:package:16494942/1" type="model">
<description>4-SOT223, 2.30 mm pitch, 7.00 mm span, 6.50 X 3.50 X 1.70 mm body
&lt;p&gt;4-pin SOT223 package with 2.30 mm pitch, 7.00 mm span with body size 6.50 X 3.50 X 1.70 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="SOT230P700X170-4"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="NCP1117ST33T3G" urn="urn:adsk.eagle:symbol:16494928/2">
<wire x1="12.7" y1="5.08" x2="12.7" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="12.7" y1="-5.08" x2="-12.7" y2="-5.08" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="-5.08" x2="-12.7" y2="5.08" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="5.08" x2="12.7" y2="5.08" width="0.1524" layer="94"/>
<text x="0" y="7.62" size="1.78025" layer="95" align="center">&gt;NAME</text>
<text x="2.54" y="-8.89" size="1.78033125" layer="96">&gt;VALUE</text>
<pin name="IN" x="-15.24" y="0" length="short" direction="in"/>
<pin name="OUT" x="15.24" y="0" length="short" direction="out" rot="R180"/>
<pin name="GND" x="0" y="-7.62" length="short" direction="pas" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="NCP1117ST33T3G" urn="urn:adsk.eagle:component:16494946/3" prefix="VR">
<description>1.0 A Low-Dropout Positive Fixed and Adjustable Voltage Regulators
&lt;br&gt;&lt;a href="https://www.onsemi.com/pub/Collateral/NCP1117-D.PDF"&gt;Datasheet&lt;/a&gt;&lt;br&gt;&lt;br&gt;</description>
<gates>
<gate name="G$1" symbol="NCP1117ST33T3G" x="0" y="0"/>
</gates>
<devices>
<device name="SOT-223" package="SOT230P700X170-4">
<connects>
<connect gate="G$1" pin="GND" pad="1"/>
<connect gate="G$1" pin="IN" pad="3"/>
<connect gate="G$1" pin="OUT" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16494942/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="IC_Power-Management" constant="no"/>
<attribute name="DESCRIPTION" value="" constant="no"/>
<attribute name="MANUFACTURER" value="ON Semiconductor" constant="no"/>
<attribute name="MPN" value="NCP1117ST33T3G" constant="no"/>
<attribute name="OPERATING_TEMP" value="0 to +125 °C" constant="no"/>
<attribute name="PART_STATUS" value="Active" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="RoHS3 Compliant" constant="no"/>
<attribute name="SERIES" value="NCP1117, NCV1117" constant="no"/>
<attribute name="SUB-CATEGORY" value="Voltage Regulator" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="Linear" constant="no"/>
<attribute name="VALUE" value="NCP1117ST33T3G" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="LED">
<description>&lt;B&gt;LED parts CHIP-Flat Top, Round Top</description>
<packages>
<package name="WS2812B" urn="urn:adsk.eagle:footprint:29365667/1">
<description>&lt;b&gt;WS2812B&lt;/b&gt;&lt;br&gt;</description>
<text x="-0.26" y="3.72" size="1.27" layer="25" align="center">&gt;NAME</text>
<text x="0" y="-3.81" size="1.27" layer="27" align="center">&gt;VALUE</text>
<wire x1="-2.5" y1="2.5" x2="2.5" y2="2.5" width="0.12" layer="51"/>
<wire x1="2.5" y1="2.5" x2="2.5" y2="-2.5" width="0.12" layer="51"/>
<wire x1="2.5" y1="-2.5" x2="-2.5" y2="-2.5" width="0.12" layer="51"/>
<wire x1="-2.5" y1="-2.5" x2="-2.5" y2="2.5" width="0.12" layer="51"/>
<wire x1="2.5" y1="-2.5" x2="-2.5" y2="-2.5" width="0.12" layer="21"/>
<wire x1="-2.5" y1="-2.5" x2="-2.5" y2="-2.3876" width="0.12" layer="21"/>
<wire x1="-2.5" y1="2.5" x2="2.5" y2="2.5" width="0.12" layer="21"/>
<wire x1="-2.5" y1="-0.7112" x2="-2.5" y2="0.7112" width="0.12" layer="21"/>
<wire x1="2.5" y1="-0.7112" x2="2.5" y2="0.7112" width="0.12" layer="21"/>
<wire x1="2.5" y1="-2.5" x2="2.5" y2="-2.3876" width="0.12" layer="21"/>
<wire x1="-2.5" y1="2.3768" x2="-2.5" y2="2.4892" width="0.12" layer="21"/>
<wire x1="2.5038" y1="2.3768" x2="2.5038" y2="2.4892" width="0.12" layer="21"/>
<circle x="0" y="0" radius="1.7" width="0.12" layer="21"/>
<circle x="-3.7206" y="1.65" radius="0.25" width="0" layer="21"/>
<smd name="3" x="2.45" y="-1.65" dx="1.5" dy="0.9" layer="1" rot="R180"/>
<smd name="4" x="2.45" y="1.65" dx="1.5" dy="0.9" layer="1" rot="R180"/>
<smd name="2" x="-2.45" y="-1.65" dx="1.5" dy="0.9" layer="1" rot="R180"/>
<smd name="1" x="-2.45" y="1.65" dx="1.5" dy="0.9" layer="1" rot="R180"/>
</package>
</packages>
<packages3d>
<package3d name="WS2812B" urn="urn:adsk.eagle:package:29365669/4" type="model">
<description>&lt;b&gt;WS2812B&lt;/b&gt;&lt;br&gt;</description>
<packageinstances>
<packageinstance name="WS2812B"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="WS2812B" urn="urn:adsk.eagle:symbol:29365664/1">
<wire x1="-10.16" y1="5.08" x2="10.16" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="10.16" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="-10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-10.16" y1="5.08" x2="-10.16" y2="-5.08" width="0.254" layer="94"/>
<text x="0" y="7.62" size="1.778" layer="95" align="center">&gt;NAME</text>
<text x="0" y="-7.62" size="1.778" layer="96" align="center">&gt;VALUE</text>
<pin name="DOUT" x="12.7" y="-2.54" length="short" direction="out" rot="R180"/>
<pin name="DIN" x="12.7" y="2.54" length="short" direction="in" rot="R180"/>
<pin name="VDD" x="-12.7" y="2.54" length="short" direction="pwr"/>
<pin name="VSS" x="-12.7" y="-2.54" length="short" direction="pwr"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="WS2812B" urn="urn:adsk.eagle:component:29365672/2" prefix="D">
<description>Intelligent control LED integrated light source&lt;p&gt;

&lt;a href="http://cdn.sparkfun.com/datasheets/BreakoutBoards/WS2812B.pdf"&gt; Datasheet &lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="WS2812B" x="0" y="0"/>
</gates>
<devices>
<device name="" package="WS2812B">
<connects>
<connect gate="G$1" pin="DIN" pad="4"/>
<connect gate="G$1" pin="DOUT" pad="2"/>
<connect gate="G$1" pin="VDD" pad="1"/>
<connect gate="G$1" pin="VSS" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:29365669/4"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="CATEGORY" value="LED" constant="no"/>
<attribute name="COLOR" value="RGB" constant="no"/>
<attribute name="DESCRIPTION" value="LED integrated light source" constant="no"/>
<attribute name="MANUFACTURER" value="Worldsemi" constant="no"/>
<attribute name="MPN" value="WS2812B" constant="no"/>
<attribute name="OPERATING_TEMP" value="-25 ℃ to +80℃(TJ)" constant="no"/>
<attribute name="PART_STATUS" value="Active" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB_CATEGORY" value="RGB" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Capacitor">
<description>&lt;B&gt;Capacitors - Fixed, Variable, Trimmers</description>
<packages>
<package name="CAPC1005X60" urn="urn:adsk.eagle:footprint:16290849/2">
<description>Chip, 1.00 X 0.50 X 0.60 mm body
&lt;p&gt;Chip package with body size 1.00 X 0.50 X 0.60 mm&lt;/p&gt;</description>
<wire x1="0.55" y1="0.6286" x2="-0.55" y2="0.6286" width="0.127" layer="21"/>
<wire x1="0.55" y1="-0.6286" x2="-0.55" y2="-0.6286" width="0.127" layer="21"/>
<wire x1="0.55" y1="-0.3" x2="-0.55" y2="-0.3" width="0.12" layer="51"/>
<wire x1="-0.55" y1="-0.3" x2="-0.55" y2="0.3" width="0.12" layer="51"/>
<wire x1="-0.55" y1="0.3" x2="0.55" y2="0.3" width="0.12" layer="51"/>
<wire x1="0.55" y1="0.3" x2="0.55" y2="-0.3" width="0.12" layer="51"/>
<smd name="1" x="-0.4846" y="0" dx="0.56" dy="0.6291" layer="1"/>
<smd name="2" x="0.4846" y="0" dx="0.56" dy="0.6291" layer="1"/>
<text x="0" y="1.2636" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.2636" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC1110X102" urn="urn:adsk.eagle:footprint:16290845/2">
<description>Chip, 1.17 X 1.02 X 1.02 mm body
&lt;p&gt;Chip package with body size 1.17 X 1.02 X 1.02 mm&lt;/p&gt;</description>
<wire x1="0.66" y1="0.9552" x2="-0.66" y2="0.9552" width="0.127" layer="21"/>
<wire x1="0.66" y1="-0.9552" x2="-0.66" y2="-0.9552" width="0.127" layer="21"/>
<wire x1="0.66" y1="-0.635" x2="-0.66" y2="-0.635" width="0.12" layer="51"/>
<wire x1="-0.66" y1="-0.635" x2="-0.66" y2="0.635" width="0.12" layer="51"/>
<wire x1="-0.66" y1="0.635" x2="0.66" y2="0.635" width="0.12" layer="51"/>
<wire x1="0.66" y1="0.635" x2="0.66" y2="-0.635" width="0.12" layer="51"/>
<smd name="1" x="-0.5388" y="0" dx="0.6626" dy="1.2823" layer="1"/>
<smd name="2" x="0.5388" y="0" dx="0.6626" dy="1.2823" layer="1"/>
<text x="0" y="1.5902" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.5902" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC1608X85" urn="urn:adsk.eagle:footprint:16290847/2">
<description>Chip, 1.60 X 0.80 X 0.85 mm body
&lt;p&gt;Chip package with body size 1.60 X 0.80 X 0.85 mm&lt;/p&gt;</description>
<wire x1="0.875" y1="0.7991" x2="-0.875" y2="0.7991" width="0.127" layer="21"/>
<wire x1="0.875" y1="-0.7991" x2="-0.875" y2="-0.7991" width="0.127" layer="21"/>
<wire x1="0.875" y1="-0.475" x2="-0.875" y2="-0.475" width="0.12" layer="51"/>
<wire x1="-0.875" y1="-0.475" x2="-0.875" y2="0.475" width="0.12" layer="51"/>
<wire x1="-0.875" y1="0.475" x2="0.875" y2="0.475" width="0.12" layer="51"/>
<wire x1="0.875" y1="0.475" x2="0.875" y2="-0.475" width="0.12" layer="51"/>
<smd name="1" x="-0.7746" y="0" dx="0.9209" dy="0.9702" layer="1"/>
<smd name="2" x="0.7746" y="0" dx="0.9209" dy="0.9702" layer="1"/>
<text x="0" y="1.4341" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.4341" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC2012X110" urn="urn:adsk.eagle:footprint:16290848/2">
<description>Chip, 2.00 X 1.25 X 1.10 mm body
&lt;p&gt;Chip package with body size 2.00 X 1.25 X 1.10 mm&lt;/p&gt;</description>
<wire x1="1.1" y1="1.0467" x2="-1.1" y2="1.0467" width="0.127" layer="21"/>
<wire x1="1.1" y1="-1.0467" x2="-1.1" y2="-1.0467" width="0.127" layer="21"/>
<wire x1="1.1" y1="-0.725" x2="-1.1" y2="-0.725" width="0.12" layer="51"/>
<wire x1="-1.1" y1="-0.725" x2="-1.1" y2="0.725" width="0.12" layer="51"/>
<wire x1="-1.1" y1="0.725" x2="1.1" y2="0.725" width="0.12" layer="51"/>
<wire x1="1.1" y1="0.725" x2="1.1" y2="-0.725" width="0.12" layer="51"/>
<smd name="1" x="-0.8754" y="0" dx="1.1646" dy="1.4653" layer="1"/>
<smd name="2" x="0.8754" y="0" dx="1.1646" dy="1.4653" layer="1"/>
<text x="0" y="1.6817" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.6817" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC3216X135" urn="urn:adsk.eagle:footprint:16290836/2">
<description>Chip, 3.20 X 1.60 X 1.35 mm body
&lt;p&gt;Chip package with body size 3.20 X 1.60 X 1.35 mm&lt;/p&gt;</description>
<wire x1="1.7" y1="1.2217" x2="-1.7" y2="1.2217" width="0.127" layer="21"/>
<wire x1="1.7" y1="-1.2217" x2="-1.7" y2="-1.2217" width="0.127" layer="21"/>
<wire x1="1.7" y1="-0.9" x2="-1.7" y2="-0.9" width="0.12" layer="51"/>
<wire x1="-1.7" y1="-0.9" x2="-1.7" y2="0.9" width="0.12" layer="51"/>
<wire x1="-1.7" y1="0.9" x2="1.7" y2="0.9" width="0.12" layer="51"/>
<wire x1="1.7" y1="0.9" x2="1.7" y2="-0.9" width="0.12" layer="51"/>
<smd name="1" x="-1.4754" y="0" dx="1.1646" dy="1.8153" layer="1"/>
<smd name="2" x="1.4754" y="0" dx="1.1646" dy="1.8153" layer="1"/>
<text x="0" y="1.8567" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.8567" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC3225X135" urn="urn:adsk.eagle:footprint:16290843/2">
<description>Chip, 3.20 X 2.50 X 1.35 mm body
&lt;p&gt;Chip package with body size 3.20 X 2.50 X 1.35 mm&lt;/p&gt;</description>
<wire x1="1.7" y1="1.6717" x2="-1.7" y2="1.6717" width="0.127" layer="21"/>
<wire x1="1.7" y1="-1.6717" x2="-1.7" y2="-1.6717" width="0.12" layer="21"/>
<wire x1="1.7" y1="-1.35" x2="-1.7" y2="-1.35" width="0.12" layer="51"/>
<wire x1="-1.7" y1="-1.35" x2="-1.7" y2="1.35" width="0.12" layer="51"/>
<wire x1="-1.7" y1="1.35" x2="1.7" y2="1.35" width="0.12" layer="51"/>
<wire x1="1.7" y1="1.35" x2="1.7" y2="-1.35" width="0.12" layer="51"/>
<smd name="1" x="-1.4754" y="0" dx="1.1646" dy="2.7153" layer="1"/>
<smd name="2" x="1.4754" y="0" dx="1.1646" dy="2.7153" layer="1"/>
<text x="0" y="2.3067" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.3067" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC4532X135" urn="urn:adsk.eagle:footprint:16290841/2">
<description>Chip, 4.50 X 3.20 X 1.35 mm body
&lt;p&gt;Chip package with body size 4.50 X 3.20 X 1.35 mm&lt;/p&gt;</description>
<wire x1="2.4" y1="2.0217" x2="-2.4" y2="2.0217" width="0.127" layer="21"/>
<wire x1="2.4" y1="-2.0217" x2="-2.4" y2="-2.0217" width="0.127" layer="21"/>
<wire x1="2.4" y1="-1.7" x2="-2.4" y2="-1.7" width="0.12" layer="51"/>
<wire x1="-2.4" y1="-1.7" x2="-2.4" y2="1.7" width="0.12" layer="51"/>
<wire x1="-2.4" y1="1.7" x2="2.4" y2="1.7" width="0.12" layer="51"/>
<wire x1="2.4" y1="1.7" x2="2.4" y2="-1.7" width="0.12" layer="51"/>
<smd name="1" x="-2.0565" y="0" dx="1.3973" dy="3.4153" layer="1"/>
<smd name="2" x="2.0565" y="0" dx="1.3973" dy="3.4153" layer="1"/>
<text x="0" y="2.6567" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.6567" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPM3216X180" urn="urn:adsk.eagle:footprint:16290835/2">
<description>Molded Body, 3.20 X 1.60 X 1.80 mm body
&lt;p&gt;Molded Body package with body size 3.20 X 1.60 X 1.80 mm&lt;/p&gt;</description>
<wire x1="-1.7" y1="0.9084" x2="1.7" y2="0.9084" width="0.127" layer="21"/>
<wire x1="-1.7" y1="-0.9084" x2="1.7" y2="-0.9084" width="0.127" layer="21"/>
<wire x1="1.7" y1="-0.9" x2="-1.7" y2="-0.9" width="0.12" layer="51"/>
<wire x1="-1.7" y1="-0.9" x2="-1.7" y2="0.9" width="0.12" layer="51"/>
<wire x1="-1.7" y1="0.9" x2="1.7" y2="0.9" width="0.12" layer="51"/>
<wire x1="1.7" y1="0.9" x2="1.7" y2="-0.9" width="0.12" layer="51"/>
<smd name="1" x="-1.3099" y="0" dx="1.7955" dy="1.1887" layer="1"/>
<smd name="2" x="1.3099" y="0" dx="1.7955" dy="1.1887" layer="1"/>
<text x="0" y="1.5434" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.5434" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPM3528X210" urn="urn:adsk.eagle:footprint:16290844/2">
<description>Molded Body, 3.50 X 2.80 X 2.10 mm body
&lt;p&gt;Molded Body package with body size 3.50 X 2.80 X 2.10 mm&lt;/p&gt;</description>
<wire x1="-1.85" y1="1.5" x2="1.85" y2="1.5" width="0.127" layer="21"/>
<wire x1="-1.85" y1="-1.5" x2="1.85" y2="-1.5" width="0.127" layer="21"/>
<wire x1="1.85" y1="-1.5" x2="-1.85" y2="-1.5" width="0.12" layer="51"/>
<wire x1="-1.85" y1="-1.5" x2="-1.85" y2="1.5" width="0.12" layer="51"/>
<wire x1="-1.85" y1="1.5" x2="1.85" y2="1.5" width="0.12" layer="51"/>
<wire x1="1.85" y1="1.5" x2="1.85" y2="-1.5" width="0.12" layer="51"/>
<smd name="1" x="-1.4599" y="0" dx="1.7955" dy="2.2036" layer="1"/>
<smd name="2" x="1.4599" y="0" dx="1.7955" dy="2.2036" layer="1"/>
<text x="0" y="2.135" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.135" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPM6032X280" urn="urn:adsk.eagle:footprint:16290839/2">
<description>Molded Body, 6.00 X 3.20 X 2.80 mm body
&lt;p&gt;Molded Body package with body size 6.00 X 3.20 X 2.80 mm&lt;/p&gt;</description>
<wire x1="-3.15" y1="1.75" x2="3.15" y2="1.75" width="0.127" layer="21"/>
<wire x1="-3.15" y1="-1.75" x2="3.15" y2="-1.75" width="0.127" layer="21"/>
<wire x1="3.15" y1="-1.75" x2="-3.15" y2="-1.75" width="0.12" layer="51"/>
<wire x1="-3.15" y1="-1.75" x2="-3.15" y2="1.75" width="0.12" layer="51"/>
<wire x1="-3.15" y1="1.75" x2="3.15" y2="1.75" width="0.12" layer="51"/>
<wire x1="3.15" y1="1.75" x2="3.15" y2="-1.75" width="0.12" layer="51"/>
<smd name="1" x="-2.4712" y="0" dx="2.368" dy="2.2036" layer="1"/>
<smd name="2" x="2.4712" y="0" dx="2.368" dy="2.2036" layer="1"/>
<text x="0" y="2.385" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.385" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPM7343X310" urn="urn:adsk.eagle:footprint:16290840/2">
<description>Molded Body, 7.30 X 4.30 X 3.10 mm body
&lt;p&gt;Molded Body package with body size 7.30 X 4.30 X 3.10 mm&lt;/p&gt;</description>
<wire x1="-3.8" y1="2.3" x2="3.8" y2="2.3" width="0.127" layer="21"/>
<wire x1="-3.8" y1="-2.3" x2="3.8" y2="-2.3" width="0.127" layer="21"/>
<wire x1="3.8" y1="-2.3" x2="-3.8" y2="-2.3" width="0.12" layer="51"/>
<wire x1="-3.8" y1="-2.3" x2="-3.8" y2="2.3" width="0.12" layer="51"/>
<wire x1="-3.8" y1="2.3" x2="3.8" y2="2.3" width="0.12" layer="51"/>
<wire x1="3.8" y1="2.3" x2="3.8" y2="-2.3" width="0.12" layer="51"/>
<smd name="1" x="-3.1212" y="0" dx="2.368" dy="2.4036" layer="1"/>
<smd name="2" x="3.1212" y="0" dx="2.368" dy="2.4036" layer="1"/>
<text x="0" y="2.935" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.935" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPC4564X110" urn="urn:adsk.eagle:footprint:16290837/2">
<description>Chip, 4.50 X 6.40 X 1.10 mm body
&lt;p&gt;Chip package with body size 4.50 X 6.40 X 1.10 mm&lt;/p&gt;</description>
<wire x1="2.4" y1="3.7179" x2="-2.4" y2="3.7179" width="0.127" layer="21"/>
<wire x1="2.4" y1="-3.7179" x2="-2.4" y2="-3.7179" width="0.127" layer="21"/>
<wire x1="2.4" y1="-3.4" x2="-2.4" y2="-3.4" width="0.12" layer="51"/>
<wire x1="-2.4" y1="-3.4" x2="-2.4" y2="3.4" width="0.12" layer="51"/>
<wire x1="-2.4" y1="3.4" x2="2.4" y2="3.4" width="0.12" layer="51"/>
<wire x1="2.4" y1="3.4" x2="2.4" y2="-3.4" width="0.12" layer="51"/>
<smd name="1" x="-2.0565" y="0" dx="1.3973" dy="6.8078" layer="1"/>
<smd name="2" x="2.0565" y="0" dx="1.3973" dy="6.8078" layer="1"/>
<text x="0" y="4.3529" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-4.3529" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPRD550W60D1025H1250B" urn="urn:adsk.eagle:footprint:16290829/2">
<description>Radial Non-Polarized Capacitor, 5.50 mm pitch, 10.25 mm body diameter, 12.50 mm body height
&lt;p&gt;Radial Non-Polarized Capacitor package with 5.50 mm pitch (lead spacing), 0.60 mm lead diameter, 10.25 mm body diameter and 12.50 mm body height&lt;/p&gt;</description>
<circle x="0" y="0" radius="5.25" width="0.127" layer="21"/>
<circle x="0" y="0" radius="5.25" width="0.12" layer="51"/>
<pad name="1" x="-2.75" y="0" drill="0.8" diameter="1.4"/>
<pad name="2" x="2.75" y="0" drill="0.8" diameter="1.4"/>
<text x="0" y="5.885" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-5.885" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
<package name="CAPRD2261W240D5080H5555B" urn="urn:adsk.eagle:footprint:16290850/2">
<description>Radial Non-Polarized Capacitor, 22.61 mm pitch, 50.80 mm body diameter, 55.55 mm body height
&lt;p&gt;Radial Non-Polarized Capacitor package with 22.61 mm pitch (lead spacing), 2.40 mm lead diameter, 50.80 mm body diameter and 55.55 mm body height&lt;/p&gt;</description>
<circle x="0" y="0" radius="25.79" width="0.127" layer="21"/>
<circle x="0" y="0" radius="25.79" width="0.12" layer="51"/>
<pad name="1" x="-11.305" y="0" drill="2.6" diameter="3.9"/>
<pad name="2" x="11.305" y="0" drill="2.6" diameter="3.9"/>
<text x="0" y="26.425" size="1.27" layer="25" align="bottom-center">&gt;NAME</text>
<text x="0" y="-26.425" size="1.27" layer="27" align="top-center">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="CAPC1005X60" urn="urn:adsk.eagle:package:16290895/2" type="model">
<description>Chip, 1.00 X 0.50 X 0.60 mm body
&lt;p&gt;Chip package with body size 1.00 X 0.50 X 0.60 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC1005X60"/>
</packageinstances>
</package3d>
<package3d name="CAPC1110X102" urn="urn:adsk.eagle:package:16290904/2" type="model">
<description>Chip, 1.17 X 1.02 X 1.02 mm body
&lt;p&gt;Chip package with body size 1.17 X 1.02 X 1.02 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC1110X102"/>
</packageinstances>
</package3d>
<package3d name="CAPC1608X85" urn="urn:adsk.eagle:package:16290898/2" type="model">
<description>Chip, 1.60 X 0.80 X 0.85 mm body
&lt;p&gt;Chip package with body size 1.60 X 0.80 X 0.85 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC1608X85"/>
</packageinstances>
</package3d>
<package3d name="CAPC2012X110" urn="urn:adsk.eagle:package:16290897/2" type="model">
<description>Chip, 2.00 X 1.25 X 1.10 mm body
&lt;p&gt;Chip package with body size 2.00 X 1.25 X 1.10 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC2012X110"/>
</packageinstances>
</package3d>
<package3d name="CAPC3216X135" urn="urn:adsk.eagle:package:16290893/2" type="model">
<description>Chip, 3.20 X 1.60 X 1.35 mm body
&lt;p&gt;Chip package with body size 3.20 X 1.60 X 1.35 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC3216X135"/>
</packageinstances>
</package3d>
<package3d name="CAPC3225X135" urn="urn:adsk.eagle:package:16290903/2" type="model">
<description>Chip, 3.20 X 2.50 X 1.35 mm body
&lt;p&gt;Chip package with body size 3.20 X 2.50 X 1.35 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC3225X135"/>
</packageinstances>
</package3d>
<package3d name="CAPC4532X135" urn="urn:adsk.eagle:package:16290900/2" type="model">
<description>Chip, 4.50 X 3.20 X 1.35 mm body
&lt;p&gt;Chip package with body size 4.50 X 3.20 X 1.35 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC4532X135"/>
</packageinstances>
</package3d>
<package3d name="CAPM3216X180" urn="urn:adsk.eagle:package:16290894/2" type="model">
<description>Molded Body, 3.20 X 1.60 X 1.80 mm body
&lt;p&gt;Molded Body package with body size 3.20 X 1.60 X 1.80 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPM3216X180"/>
</packageinstances>
</package3d>
<package3d name="CAPM3528X210" urn="urn:adsk.eagle:package:16290902/2" type="model">
<description>Molded Body, 3.50 X 2.80 X 2.10 mm body
&lt;p&gt;Molded Body package with body size 3.50 X 2.80 X 2.10 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPM3528X210"/>
</packageinstances>
</package3d>
<package3d name="CAPM6032X280" urn="urn:adsk.eagle:package:16290896/2" type="model">
<description>Molded Body, 6.00 X 3.20 X 2.80 mm body
&lt;p&gt;Molded Body package with body size 6.00 X 3.20 X 2.80 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPM6032X280"/>
</packageinstances>
</package3d>
<package3d name="CAPM7343X310" urn="urn:adsk.eagle:package:16290891/2" type="model">
<description>Molded Body, 7.30 X 4.30 X 3.10 mm body
&lt;p&gt;Molded Body package with body size 7.30 X 4.30 X 3.10 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPM7343X310"/>
</packageinstances>
</package3d>
<package3d name="CAPC4564X110L" urn="urn:adsk.eagle:package:16290887/3" type="model">
<description>Chip, 4.50 X 6.40 X 1.10 mm body
&lt;p&gt;Chip package with body size 4.50 X 6.40 X 1.10 mm&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPC4564X110"/>
</packageinstances>
</package3d>
<package3d name="CAPRD550W60D1025H1250B" urn="urn:adsk.eagle:package:16290858/2" type="model">
<description>Radial Non-Polarized Capacitor, 5.50 mm pitch, 10.25 mm body diameter, 12.50 mm body height
&lt;p&gt;Radial Non-Polarized Capacitor package with 5.50 mm pitch (lead spacing), 0.60 mm lead diameter, 10.25 mm body diameter and 12.50 mm body height&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPRD550W60D1025H1250B"/>
</packageinstances>
</package3d>
<package3d name="CAPRD2261W240D5080H5555B" urn="urn:adsk.eagle:package:16290864/2" type="model">
<description>Radial Non-Polarized Capacitor, 22.61 mm pitch, 50.80 mm body diameter, 55.55 mm body height
&lt;p&gt;Radial Non-Polarized Capacitor package with 22.61 mm pitch (lead spacing), 2.40 mm lead diameter, 50.80 mm body diameter and 55.55 mm body height&lt;/p&gt;</description>
<packageinstances>
<packageinstance name="CAPRD2261W240D5080H5555B"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="C" urn="urn:adsk.eagle:symbol:16290820/1">
<description>Capacitor</description>
<rectangle x1="-2.032" y1="-2.032" x2="2.032" y2="-1.524" layer="94"/>
<rectangle x1="-2.032" y1="-1.016" x2="2.032" y2="-0.508" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-2.032" width="0.1524" layer="94"/>
<pin name="1" x="0" y="2.54" visible="off" length="short" direction="pas" swaplevel="1" rot="R270"/>
<pin name="2" x="0" y="-5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R90"/>
<text x="2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-2.54" size="1.778" layer="97">&gt;SPICEMODEL</text>
<text x="2.54" y="0" size="1.778" layer="96">&gt;VALUE</text>
<text x="2.54" y="-5.08" size="1.778" layer="97">&gt;SPICEEXTRA</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="C" urn="urn:adsk.eagle:component:16290909/7" prefix="C" uservalue="yes">
<description>&lt;B&gt;Capacitor - Generic</description>
<gates>
<gate name="G$1" symbol="C" x="0" y="0"/>
</gates>
<devices>
<device name="CHIP-0402(1005-METRIC)" package="CAPC1005X60">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290895/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-0504(1310-METRIC)" package="CAPC1110X102">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290904/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-0603(1608-METRIC)" package="CAPC1608X85">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290898/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-0805(2012-METRIC)" package="CAPC2012X110">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290897/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-1206(3216-METRIC)" package="CAPC3216X135">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290893/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-1210(3225-METRIC)" package="CAPC3225X135">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290903/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-1812(4532-METRIC)" package="CAPC4532X135">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290900/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="TANTALUM-1206(3216-METRIC)" package="CAPM3216X180">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290894/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="TANTALUM-1411(3528-METRIC)" package="CAPM3528X210">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290902/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="TANTALUM-2412(6032-METRIC)" package="CAPM6032X280">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290896/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="TANTALUM-2917(7343-METRIC)" package="CAPM7343X310">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290891/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="CHIP-1825(4564-METRIC)" package="CAPC4564X110">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290887/3"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="RADIAL-12.5MM-DIA" package="CAPRD550W60D1025H1250B">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290858/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
<device name="RADIAL-55.5MM-DIA" package="CAPRD2261W240D5080H5555B">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:16290864/2"/>
</package3dinstances>
<technologies>
<technology name="_">
<attribute name="CATEGORY" value="Capacitor" constant="no"/>
<attribute name="MANUFACTURER" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OPERATING_TEMP" value="" constant="no"/>
<attribute name="PART_STATUS" value="" constant="no"/>
<attribute name="ROHS_COMPLIANT" value="" constant="no"/>
<attribute name="SERIES" value="" constant="no"/>
<attribute name="SUB-CATEGORY" value="" constant="no"/>
<attribute name="THERMALLOSS" value="" constant="no"/>
<attribute name="TYPE" value="" constant="no"/>
<attribute name="VOLTAGE_RATING" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
<spice>
<pinmapping spiceprefix="C">
<pinmap gate="G$1" pin="1" pinorder="1"/>
<pinmap gate="G$1" pin="2" pinorder="2"/>
</pinmapping>
</spice>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="ESP-12F" device="" package3d_urn="urn:adsk.eagle:package:42764230/1"/>
<part name="FRAME1" library="frames" library_urn="urn:adsk.eagle:library:229" deviceset="DINA4_L" device=""/>
<part name="SUPPLY2" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY11" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="JP1" library="Connector" deviceset="PINHD-2X3" device="" package3d_urn="urn:adsk.eagle:package:22462/2"/>
<part name="SUPPLY13" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="VR1" library="IC_Power-Management" deviceset="NCP1117ST33T3G" device="SOT-223" package3d_urn="urn:adsk.eagle:package:16494942/1" value="NCP1117ST33T3G"/>
<part name="SUPPLY14" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY4" library="Power_Symbols" deviceset="+3.3V" device="" value="+3.3V"/>
<part name="SUPPLY5" library="Power_Symbols" deviceset="+3.3V" device="" value="+3.3V"/>
<part name="SUPPLY6" library="Power_Symbols" deviceset="+3.3V" device="" value="+3.3V"/>
<part name="S1" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="KURZHUBTASTER" device="" package3d_urn="urn:adsk.eagle:package:21817514/3"/>
<part name="S2" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="KURZHUBTASTER" device="" package3d_urn="urn:adsk.eagle:package:21817514/3"/>
<part name="D1" library="LED" deviceset="WS2812B" device="" package3d_urn="urn:adsk.eagle:package:29365669/4"/>
<part name="D2" library="LED" deviceset="WS2812B" device="" package3d_urn="urn:adsk.eagle:package:29365669/4"/>
<part name="D3" library="LED" deviceset="WS2812B" device="" package3d_urn="urn:adsk.eagle:package:29365669/4"/>
<part name="SUPPLY7" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY8" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY10" library="Power_Symbols" deviceset="+5V" device="" value="+5V"/>
<part name="SUPPLY1" library="Power_Symbols" deviceset="+5V" device="" value="+5V"/>
<part name="C1" library="Capacitor" deviceset="C" device="CHIP-0805(2012-METRIC)" package3d_urn="urn:adsk.eagle:package:16290897/2" technology="_" value="100n"/>
<part name="C2" library="Capacitor" deviceset="C" device="CHIP-0805(2012-METRIC)" package3d_urn="urn:adsk.eagle:package:16290897/2" technology="_" value="100n"/>
<part name="SUPPLY9" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY12" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY15" library="Power_Symbols" deviceset="GND-BAR" device="" value="GND"/>
<part name="SUPPLY3" library="Power_Symbols" deviceset="+3.3V" device="" value="+3.3V"/>
<part name="SW1" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="SWITCH_SPDT" device="MICRO" package3d_urn="urn:adsk.eagle:package:42807267/1"/>
<part name="S3" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="KURZHUBTASTER" device="" package3d_urn="urn:adsk.eagle:package:21817514/3"/>
<part name="C3" library="Capacitor" deviceset="C" device="CHIP-0805(2012-METRIC)" package3d_urn="urn:adsk.eagle:package:16290897/2" technology="_" value="10u"/>
<part name="X1" library="OwnDevices" library_urn="urn:adsk.wipprod:fs.file:vf.a4uZ8vxKQaeHmtfASYy_rA" deviceset="JST-XH-02-PIN" device="-EDGE" package3d_urn="urn:adsk.eagle:package:42837631/1" value="JST-XH-02"/>
</parts>
<sheets>
<sheet>
<plain>
<text x="7.62" y="167.64" size="2.54" layer="97">Power Supply Input (e.g. 5V)</text>
<text x="205.74" y="134.62" size="2.54" layer="97">Programming Header</text>
<text x="208.28" y="99.06" size="2.54" layer="97">MCU ESP-12F</text>
<text x="78.74" y="167.64" size="2.54" layer="97">Voltage Regulation to 3,3V</text>
<text x="10.16" y="119.38" size="2.54" layer="97">Buttons</text>
<text x="78.74" y="119.38" size="2.54" layer="97">LEDs</text>
<text x="114.3" y="60.96" size="2.54" layer="97">WiFi</text>
<text x="114.3" y="101.6" size="2.54" layer="97">Door 1 Status</text>
<text x="114.3" y="81.28" size="2.54" layer="97">Door 2 Status</text>
<text x="165.1" y="27.94" size="3.81" layer="94">Garage_Door_Sensor</text>
<text x="233.68" y="27.94" size="2.54" layer="94">Milestone M1</text>
</plain>
<instances>
<instance part="U1" gate="G$1" x="218.44" y="71.12" smashed="yes">
<attribute name="NAME" x="205.7146" y="94.0257" size="1.272540625" layer="95" ratio="11"/>
<attribute name="VALUE" x="205.7163" y="48.2174" size="1.27236875" layer="96" ratio="11"/>
</instance>
<instance part="FRAME1" gate="G$1" x="0" y="0" smashed="yes"/>
<instance part="FRAME1" gate="G$2" x="162.56" y="0" smashed="yes">
<attribute name="LAST_DATE_TIME" x="175.26" y="1.27" size="2.54" layer="94"/>
<attribute name="SHEET" x="248.92" y="1.27" size="2.54" layer="94"/>
<attribute name="DRAWING_NAME" x="180.34" y="19.05" size="2.54" layer="94"/>
</instance>
<instance part="SUPPLY2" gate="G$1" x="25.4" y="142.24" smashed="yes">
<attribute name="VALUE" x="25.4" y="140.335" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY11" gate="G$1" x="190.5" y="73.66" smashed="yes">
<attribute name="VALUE" x="190.5" y="71.755" size="1.778" layer="96" align="center"/>
</instance>
<instance part="JP1" gate="A" x="218.44" y="124.46" smashed="yes">
<attribute name="NAME" x="212.09" y="130.175" size="1.778" layer="95"/>
<attribute name="VALUE" x="212.09" y="116.84" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY13" gate="G$1" x="187.96" y="124.46" smashed="yes">
<attribute name="VALUE" x="187.96" y="122.555" size="1.778" layer="96" align="center"/>
</instance>
<instance part="VR1" gate="G$1" x="93.98" y="157.48" smashed="yes">
<attribute name="NAME" x="93.98" y="165.1" size="1.78025" layer="95" align="center"/>
<attribute name="VALUE" x="96.52" y="148.59" size="1.78033125" layer="96"/>
</instance>
<instance part="SUPPLY14" gate="G$1" x="93.98" y="142.24" smashed="yes">
<attribute name="VALUE" x="93.98" y="140.335" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY4" gate="G$1" x="134.62" y="160.02" smashed="yes">
<attribute name="VALUE" x="134.493" y="163.068" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="SUPPLY5" gate="G$1" x="233.68" y="127" smashed="yes">
<attribute name="VALUE" x="233.553" y="130.048" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="SUPPLY6" gate="G$1" x="190.5" y="81.28" smashed="yes">
<attribute name="VALUE" x="190.373" y="84.328" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="S1" gate="1" x="22.86" y="106.68" smashed="yes" rot="R270">
<attribute name="NAME" x="20.32" y="113.03" size="1.778" layer="95"/>
<attribute name="VALUE" x="26.035" y="110.49" size="1.778" layer="96"/>
</instance>
<instance part="S2" gate="1" x="22.86" y="93.98" smashed="yes" rot="R270">
<attribute name="NAME" x="20.32" y="100.33" size="1.778" layer="95"/>
<attribute name="VALUE" x="26.035" y="97.79" size="1.778" layer="96"/>
</instance>
<instance part="D1" gate="G$1" x="96.52" y="101.6" smashed="yes">
<attribute name="NAME" x="96.52" y="109.22" size="1.778" layer="95" align="center"/>
<attribute name="VALUE" x="96.52" y="93.98" size="1.778" layer="96" align="center"/>
</instance>
<instance part="D2" gate="G$1" x="96.52" y="81.28" smashed="yes">
<attribute name="NAME" x="96.52" y="88.9" size="1.778" layer="95" align="center"/>
<attribute name="VALUE" x="96.52" y="73.66" size="1.778" layer="96" align="center"/>
</instance>
<instance part="D3" gate="G$1" x="96.52" y="60.96" smashed="yes">
<attribute name="NAME" x="96.52" y="68.58" size="1.778" layer="95" align="center"/>
<attribute name="VALUE" x="96.52" y="53.34" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY7" gate="G$1" x="15.24" y="73.66" smashed="yes">
<attribute name="VALUE" x="15.24" y="71.755" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY8" gate="G$1" x="73.66" y="50.8" smashed="yes">
<attribute name="VALUE" x="73.66" y="48.895" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY10" gate="G$1" x="71.12" y="160.02" smashed="yes">
<attribute name="VALUE" x="70.993" y="163.068" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="SUPPLY1" gate="G$1" x="50.8" y="149.86" smashed="yes">
<attribute name="VALUE" x="50.673" y="152.908" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="C1" gate="G$1" x="71.12" y="149.86" smashed="yes">
<attribute name="NAME" x="73.66" y="149.86" size="1.778" layer="95"/>
<attribute name="VALUE" x="73.66" y="147.32" size="1.778" layer="96"/>
</instance>
<instance part="C2" gate="G$1" x="121.92" y="149.86" smashed="yes">
<attribute name="NAME" x="124.46" y="149.86" size="1.778" layer="95"/>
<attribute name="VALUE" x="124.46" y="147.32" size="1.778" layer="96"/>
</instance>
<instance part="SUPPLY9" gate="G$1" x="71.12" y="142.24" smashed="yes">
<attribute name="VALUE" x="71.12" y="140.335" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY12" gate="G$1" x="121.92" y="142.24" smashed="yes">
<attribute name="VALUE" x="121.92" y="140.335" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY15" gate="G$1" x="134.62" y="142.24" smashed="yes">
<attribute name="VALUE" x="134.62" y="140.335" size="1.778" layer="96" align="center"/>
</instance>
<instance part="SUPPLY3" gate="G$1" x="78.74" y="106.68" smashed="yes">
<attribute name="VALUE" x="78.613" y="109.728" size="1.778" layer="96" align="bottom-center"/>
</instance>
<instance part="SW1" gate="G$1" x="43.18" y="147.32" smashed="yes">
<attribute name="NAME" x="40.64" y="152.4" size="1.778" layer="95"/>
<attribute name="VALUE" x="30.48" y="139.7" size="1.778" layer="96"/>
</instance>
<instance part="S3" gate="1" x="22.86" y="81.28" smashed="yes" rot="R270">
<attribute name="NAME" x="20.32" y="87.63" size="1.778" layer="95"/>
<attribute name="VALUE" x="26.035" y="85.09" size="1.778" layer="96"/>
</instance>
<instance part="C3" gate="G$1" x="134.62" y="149.86" smashed="yes">
<attribute name="NAME" x="137.16" y="149.86" size="1.778" layer="95"/>
<attribute name="VALUE" x="137.16" y="147.32" size="1.778" layer="96"/>
</instance>
<instance part="X1" gate="-2" x="15.24" y="147.32" smashed="yes" rot="R180">
<attribute name="NAME" x="12.7" y="148.082" size="1.524" layer="95" rot="R180"/>
</instance>
<instance part="X1" gate="-1" x="15.24" y="149.86" smashed="yes" rot="R180">
<attribute name="NAME" x="12.7" y="150.622" size="1.524" layer="95" rot="R180"/>
<attribute name="VALUE" x="9.398" y="151.257" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="SUPPLY2" gate="G$1" pin="GND"/>
<wire x1="25.4" y1="144.78" x2="25.4" y2="147.32" width="0.1524" layer="91"/>
<wire x1="25.4" y1="147.32" x2="17.78" y2="147.32" width="0.1524" layer="91"/>
<pinref part="X1" gate="-2" pin="S"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GND"/>
<wire x1="200.66" y1="76.2" x2="198.12" y2="76.2" width="0.1524" layer="91"/>
<pinref part="SUPPLY11" gate="G$1" pin="GND"/>
<wire x1="198.12" y1="76.2" x2="190.5" y2="76.2" width="0.1524" layer="91"/>
<wire x1="198.12" y1="55.88" x2="198.12" y2="76.2" width="0.1524" layer="91"/>
<junction x="198.12" y="76.2"/>
<pinref part="U1" gate="G$1" pin="GPIO15"/>
<wire x1="200.66" y1="55.88" x2="198.12" y2="55.88" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="SUPPLY14" gate="G$1" pin="GND"/>
<wire x1="93.98" y1="144.78" x2="93.98" y2="149.86" width="0.1524" layer="91"/>
<pinref part="VR1" gate="G$1" pin="GND"/>
</segment>
<segment>
<pinref part="S1" gate="1" pin="P"/>
<wire x1="17.78" y1="106.68" x2="15.24" y2="106.68" width="0.1524" layer="91"/>
<wire x1="15.24" y1="106.68" x2="15.24" y2="93.98" width="0.1524" layer="91"/>
<pinref part="SUPPLY7" gate="G$1" pin="GND"/>
<pinref part="S2" gate="1" pin="P"/>
<wire x1="15.24" y1="93.98" x2="15.24" y2="81.28" width="0.1524" layer="91"/>
<wire x1="15.24" y1="81.28" x2="15.24" y2="76.2" width="0.1524" layer="91"/>
<wire x1="17.78" y1="93.98" x2="15.24" y2="93.98" width="0.1524" layer="91"/>
<junction x="15.24" y="93.98"/>
<pinref part="S3" gate="1" pin="P"/>
<wire x1="17.78" y1="81.28" x2="15.24" y2="81.28" width="0.1524" layer="91"/>
<junction x="15.24" y="81.28"/>
</segment>
<segment>
<pinref part="D1" gate="G$1" pin="VSS"/>
<wire x1="83.82" y1="99.06" x2="73.66" y2="99.06" width="0.1524" layer="91"/>
<wire x1="73.66" y1="99.06" x2="73.66" y2="78.74" width="0.1524" layer="91"/>
<pinref part="SUPPLY8" gate="G$1" pin="GND"/>
<pinref part="D3" gate="G$1" pin="VSS"/>
<wire x1="73.66" y1="78.74" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<wire x1="73.66" y1="58.42" x2="73.66" y2="53.34" width="0.1524" layer="91"/>
<wire x1="83.82" y1="58.42" x2="73.66" y2="58.42" width="0.1524" layer="91"/>
<pinref part="D2" gate="G$1" pin="VSS"/>
<wire x1="83.82" y1="78.74" x2="73.66" y2="78.74" width="0.1524" layer="91"/>
<junction x="73.66" y="78.74"/>
<junction x="73.66" y="58.42"/>
</segment>
<segment>
<pinref part="C1" gate="G$1" pin="2"/>
<pinref part="SUPPLY9" gate="G$1" pin="GND"/>
</segment>
<segment>
<pinref part="C2" gate="G$1" pin="2"/>
<pinref part="SUPPLY12" gate="G$1" pin="GND"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="1"/>
<wire x1="215.9" y1="127" x2="187.96" y2="127" width="0.1524" layer="91"/>
<pinref part="SUPPLY13" gate="G$1" pin="GND"/>
</segment>
<segment>
<pinref part="SUPPLY15" gate="G$1" pin="GND"/>
<pinref part="C3" gate="G$1" pin="2"/>
</segment>
</net>
<net name="PROG_RX" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="TXD"/>
<wire x1="236.22" y1="88.9" x2="238.76" y2="88.9" width="0.1524" layer="91"/>
<label x="238.76" y="88.9" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="6"/>
<wire x1="223.52" y1="121.92" x2="231.14" y2="121.92" width="0.1524" layer="91"/>
<label x="231.14" y="121.92" size="1.27" layer="95" xref="yes"/>
</segment>
</net>
<net name="PROG_TX" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="RXD"/>
<wire x1="236.22" y1="86.36" x2="238.76" y2="86.36" width="0.1524" layer="91"/>
<label x="238.76" y="86.36" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="5"/>
<wire x1="215.9" y1="121.92" x2="208.28" y2="121.92" width="0.1524" layer="91"/>
<label x="208.28" y="121.92" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
<net name="PROG_RST" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="RST"/>
<wire x1="200.66" y1="88.9" x2="193.04" y2="88.9" width="0.1524" layer="91"/>
<label x="193.04" y="88.9" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="4"/>
<wire x1="223.52" y1="124.46" x2="231.14" y2="124.46" width="0.1524" layer="91"/>
<label x="231.14" y="124.46" size="1.27" layer="95" xref="yes"/>
</segment>
</net>
<net name="PROG_GPIO0" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="GPIO0"/>
<wire x1="236.22" y1="81.28" x2="238.76" y2="81.28" width="0.1524" layer="91"/>
<label x="238.76" y="81.28" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="3"/>
<wire x1="215.9" y1="124.46" x2="208.28" y2="124.46" width="0.1524" layer="91"/>
<label x="208.28" y="124.46" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
<net name="+3.3V" class="0">
<segment>
<pinref part="VR1" gate="G$1" pin="OUT"/>
<wire x1="109.22" y1="157.48" x2="121.92" y2="157.48" width="0.1524" layer="91"/>
<wire x1="134.62" y1="157.48" x2="134.62" y2="160.02" width="0.1524" layer="91"/>
<pinref part="SUPPLY4" gate="G$1" pin="+3.3V"/>
<pinref part="C2" gate="G$1" pin="1"/>
<wire x1="121.92" y1="152.4" x2="121.92" y2="157.48" width="0.1524" layer="91"/>
<wire x1="134.62" y1="157.48" x2="121.92" y2="157.48" width="0.1524" layer="91"/>
<junction x="121.92" y="157.48"/>
<pinref part="C3" gate="G$1" pin="1"/>
<wire x1="134.62" y1="152.4" x2="134.62" y2="157.48" width="0.1524" layer="91"/>
<junction x="134.62" y="157.48"/>
</segment>
<segment>
<pinref part="SUPPLY6" gate="G$1" pin="+3.3V"/>
<wire x1="190.5" y1="81.28" x2="193.04" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="VCC"/>
<wire x1="200.66" y1="81.28" x2="198.12" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="EN"/>
<wire x1="198.12" y1="81.28" x2="193.04" y2="81.28" width="0.1524" layer="91"/>
<wire x1="200.66" y1="86.36" x2="198.12" y2="86.36" width="0.1524" layer="91"/>
<wire x1="198.12" y1="86.36" x2="198.12" y2="81.28" width="0.1524" layer="91"/>
<junction x="198.12" y="81.28"/>
</segment>
<segment>
<wire x1="78.74" y1="106.68" x2="78.74" y2="104.14" width="0.1524" layer="91"/>
<wire x1="78.74" y1="104.14" x2="83.82" y2="104.14" width="0.1524" layer="91"/>
<pinref part="D1" gate="G$1" pin="VDD"/>
<pinref part="SUPPLY3" gate="G$1" pin="+3.3V"/>
<pinref part="D2" gate="G$1" pin="VDD"/>
<wire x1="83.82" y1="83.82" x2="78.74" y2="83.82" width="0.1524" layer="91"/>
<wire x1="78.74" y1="83.82" x2="78.74" y2="86.36" width="0.1524" layer="91"/>
<pinref part="D3" gate="G$1" pin="VDD"/>
<wire x1="78.74" y1="86.36" x2="78.74" y2="104.14" width="0.1524" layer="91"/>
<wire x1="83.82" y1="63.5" x2="78.74" y2="63.5" width="0.1524" layer="91"/>
<wire x1="78.74" y1="63.5" x2="78.74" y2="83.82" width="0.1524" layer="91"/>
<junction x="78.74" y="104.14"/>
<junction x="78.74" y="83.82"/>
</segment>
<segment>
<pinref part="JP1" gate="A" pin="2"/>
<wire x1="223.52" y1="127" x2="233.68" y2="127" width="0.1524" layer="91"/>
<pinref part="SUPPLY5" gate="G$1" pin="+3.3V"/>
</segment>
</net>
<net name="BTN_SHOW_STATUS" class="0">
<segment>
<pinref part="S1" gate="1" pin="S"/>
<wire x1="27.94" y1="106.68" x2="30.48" y2="106.68" width="0.1524" layer="91"/>
<label x="30.48" y="106.68" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GPIO12"/>
<wire x1="200.66" y1="63.5" x2="193.04" y2="63.5" width="0.1524" layer="91"/>
<label x="193.04" y="63.5" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
<net name="BTN_WIFI" class="0">
<segment>
<pinref part="S2" gate="1" pin="S"/>
<wire x1="27.94" y1="93.98" x2="30.48" y2="93.98" width="0.1524" layer="91"/>
<label x="30.48" y="93.98" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GPIO13"/>
<wire x1="200.66" y1="60.96" x2="193.04" y2="60.96" width="0.1524" layer="91"/>
<label x="193.04" y="60.96" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="DOUT"/>
<wire x1="109.22" y1="99.06" x2="111.76" y2="99.06" width="0.1524" layer="91"/>
<wire x1="111.76" y1="99.06" x2="111.76" y2="83.82" width="0.1524" layer="91"/>
<wire x1="111.76" y1="83.82" x2="109.22" y2="83.82" width="0.1524" layer="91"/>
<pinref part="D2" gate="G$1" pin="DIN"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="D2" gate="G$1" pin="DOUT"/>
<wire x1="109.22" y1="78.74" x2="111.76" y2="78.74" width="0.1524" layer="91"/>
<wire x1="111.76" y1="78.74" x2="111.76" y2="63.5" width="0.1524" layer="91"/>
<wire x1="111.76" y1="63.5" x2="109.22" y2="63.5" width="0.1524" layer="91"/>
<pinref part="D3" gate="G$1" pin="DIN"/>
</segment>
</net>
<net name="LEDS_DIN" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="DIN"/>
<wire x1="109.22" y1="104.14" x2="111.76" y2="104.14" width="0.1524" layer="91"/>
<wire x1="111.76" y1="104.14" x2="111.76" y2="116.84" width="0.1524" layer="91"/>
<wire x1="111.76" y1="116.84" x2="109.22" y2="116.84" width="0.1524" layer="91"/>
<label x="109.22" y="116.84" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GPIO14"/>
<wire x1="200.66" y1="58.42" x2="193.04" y2="58.42" width="0.1524" layer="91"/>
<label x="193.04" y="58.42" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
<net name="+5V" class="0">
<segment>
<wire x1="71.12" y1="160.02" x2="71.12" y2="157.48" width="0.1524" layer="91"/>
<wire x1="71.12" y1="157.48" x2="78.74" y2="157.48" width="0.1524" layer="91"/>
<pinref part="VR1" gate="G$1" pin="IN"/>
<pinref part="SUPPLY10" gate="G$1" pin="+5V"/>
<pinref part="C1" gate="G$1" pin="1"/>
<wire x1="71.12" y1="152.4" x2="71.12" y2="157.48" width="0.1524" layer="91"/>
<junction x="71.12" y="157.48"/>
</segment>
<segment>
<pinref part="SW1" gate="G$1" pin="P$2"/>
<wire x1="48.26" y1="147.32" x2="50.8" y2="147.32" width="0.1524" layer="91"/>
<wire x1="50.8" y1="147.32" x2="50.8" y2="149.86" width="0.1524" layer="91"/>
<pinref part="SUPPLY1" gate="G$1" pin="+5V"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<wire x1="17.78" y1="149.86" x2="36.83" y2="149.86" width="0.1524" layer="91"/>
<pinref part="SW1" gate="G$1" pin="P$1"/>
<pinref part="X1" gate="-1" pin="S"/>
</segment>
</net>
<net name="BTN_RESET" class="0">
<segment>
<pinref part="S3" gate="1" pin="S"/>
<wire x1="27.94" y1="81.28" x2="30.48" y2="81.28" width="0.1524" layer="91"/>
<label x="30.48" y="81.28" size="1.27" layer="95" xref="yes"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GPIO16"/>
<wire x1="200.66" y1="53.34" x2="193.04" y2="53.34" width="0.1524" layer="91"/>
<label x="193.04" y="53.34" size="1.27" layer="95" rot="R180" xref="yes"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
<note version="8.4" severity="warning">
Since Version 8.4, EAGLE supports properties for SPICE simulation. 
Probes in schematics and SPICE mapping objects found in parts and library devices
will not be understood with this version. Update EAGLE to the latest version
for full support of SPICE simulation. 
</note>
</compatibility>
</eagle>
