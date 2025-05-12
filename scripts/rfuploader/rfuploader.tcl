#!/usr/bin/tclsh

# *************************************************************************************************
#
#	Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
#	 
#	 
#	  Redistribution and use in source and binary forms, with or without 
#	  modification, are permitted provided that the following conditions 
#	  are met:
#	
#	    Redistributions of source code must retain the above copyright 
#	    notice, this list of conditions and the following disclaimer.
#	 
#	    Redistributions in binary form must reproduce the above copyright
#	    notice, this list of conditions and the following disclaimer in the 
#	    documentation and/or other materials provided with the   
#	    distribution.
#	 
#	    Neither the name of Texas Instruments Incorporated nor the names of
#	    its contributors may be used to endorse or promote products derived
#	    from this software without specific prior written permission.
#	
#	  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
#	  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
#	  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#	  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
#	  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
#	  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
#	  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#	  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#	  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
#	  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
#	  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# *************************************************************************************************
# ez430-Chronos Control Center TCL/Tk script
# *************************************************************************************************
# Rev 1.1  
# - added ini file load/save dialog to load/save key settings to Key Config pane
# - extended 12H / 24H format switch
# - added WBSL support
#
# Rev 1.0
# - initial version released to manufacturing
# *************************************************************************************************

set exit_prog 0

# load libraries -----------------------------------------------------
package require Tk

# Include BlueRobin BM-USBD1 driver
# This script file replaces the Windows DLL and adds the global handle "vcp" for the COM channel
source "driver-config.tcl"
source "eZ430-Chronos_driver.tcl"

# Open COM port
if { [BM_OpenCOM $com 115200 30 0 0] == 0 } {
  tk_dialog .dialog1 "Error" "Could not detect USB dongle. Press OK to close application." info 0 OK
  set com_available 0
  exit 0
} else  {
  set com_available 1
  flush $vcp
  # Reset hardware
  BM_Reset
  after 20
  # Flush channel
  for {set i 0} {$i < 10} {incr i} {  
    BM_GetStatus
    after 10
  }
}


# ----------------------------------------------------------------------------------------
# Global variables -----------------------------------------------------------------------

# Script revision number
set revision                1.1

# BlueRobin variables
set bluerobin_on            0
set heartrate               0
set speed                   0.0
set speed_limit_hi          25.0
set distance                0.0
set hr_sweep                0
set speed_sweep             0
set txid                    [expr 0xFFFF00 + round( rand() * 250)]
set msg                     0
set speed_is_mph            0
set speed_is_mph0           0

# SimpliciTi variables
set simpliciti_on           0
set simpliciti_sync_on      0
set simpliciti_acc_on       0
set simpliciti_ap_started   0
set x	                    0
set y                       0
set accel_x                 0
set accel_y                 0
set accel_z                 0
set accel_x_offset          0
set accel_y_offset          0
set accel_z_offset          0
set mouse_control           0
set move_x                  0
set move_y                  0
set wave_x                  { 0 50 600 50 }
set wave_y                  { 0 50 600 50 }
set wave_z                  { 0 50 600 50 }

# Key variables
set ini_file                "eZ430-Chronos-CC.ini"
set all_ini_files           { }
set button_event_text       "No button"
set button_event            0
set button_timeout          0
set event1                  { Arrow-Left Arrow-Right A B C D E F G H I J \
                              K L M N O P Q R S T U V W X Y Z F5 Space }
set event2                  { None Ctrl Alt Windows }
set pd_m1                   "Arrow-Left"
set pd_s1                   "Arrow-Right"
set pd_m2                   "F5"
set cb_m1_windows           0
set cb_m1_alt               0
set cb_m1_ctrl              0
set cb_m1_shift             0
set cb_s1_windows           0
set cb_s1_alt               0
set cb_s1_ctrl              0
set cb_s1_shift             0
set cb_m2_windows           0
set cb_m2_alt               0
set cb_m2_ctrl              0
set cb_m2_shift             0

# Sync global variables
set sync_time_hours_24      4
set sync_time_is_am         1
set sync_time_hours_12      4
set sync_time_minutes       30
set sync_time_seconds       0
set sync_date_year          2009
set sync_date_month         9
set sync_date_day           1
set sync_alarm_hours        6
set sync_alarm_minutes      30
set sync_altitude_24        500
set sync_altitude_12        1640
set sync_temperature_24     22
set sync_temperature_12     72
set sync_use_metric_units   1

# WBSL global variables
#set select_input_file       ""
if {$argc > 0} { 
    set select_input_file [lindex $argv 0]
} else {
    set select_input_file ""
}
set call_wbsl_timer         0
set call_wbsl_1             2
set call_wbsl_2             3
set wbsl_progress           0
set wbsl_on                 0
set wbsl_ap_started         0
set fsize                   0
set fp                      0
set rData                   [list]
set rData_index             0
set low_index               0
set list_count              0
set wbsl_opcode             0
set maxPayload              0
set ram_updater_downloaded  0
set wirelessUpdateStarted   0
set wbsl_timer_enabled      0
set wbsl_timer_counter      0
set wbsl_timer_flag         0
set wbsl_timer_timeout      0

# Function required by WBSL
proc ceil x  {expr {ceil($x)} }


# ----------------------------------------------------------------------------------------
# Function prototypes --------------------------------------------------------------------
proc get_spl_data {} {}
proc update_br_data {} {}
proc check_rx_serial {} {}
proc inc_heartrate {} {}
proc inc_speed {} {}
proc move_cursor {} {}
proc get_wbsl_status {} {}
proc wbsl_set_timer { timeout } {}
proc wbsl_reset_timer {} {}



# ----------------------------------------------------------------------------------------
# Graphical user interface setup ---------------------------------------------------------

# Some custom styles for graphical elements
ttk::setTheme clam
ttk::style configure custom.TCheckbutton -font "Helvetica 10"
ttk::style configure custom.TLabelframe -font "Helvetica 12 bold"

# Define basic window geometry
wm title . "eZ430-Chronos RF Uploader"
wm geometry . 700x490
wm resizable . 0 0
wm iconname . "ttknote"
ttk::frame .f
pack .f -fill both -expand 1
set w .f

# Map keys to internal functions 
bind . <Key-q> { exitpgm }

# Make the notebook and set up Ctrl+Tab traversal
ttk::notebook $w.note
pack $w.note -fill both -expand 1 -padx 2 -pady 3
ttk::notebook::enableTraversal $w.note


# ----------------------------------------------------------------------------------------
# Wireless Update pane -------------------------------------------------------------------
ttk::frame $w.note.wbsl -style custom.TFrame
$w.note add $w.note.wbsl -text "Wireless Update" -underline 0 -padding 2 
grid columnconfigure $w.note.wbsl {0 1} -weight 1 -uniform 1

ttk::label $w.note.wbsl.label0 -font "Helvetica 10 bold" -width 80 -wraplength 640 -justify center -text "Only use this update function with watch firmware that allows to invoke the Wireless Update on the watch again.\n\nOlder eZ430-Chronos kits require a manual software update of the watch and access point. See Chronoswiki."
grid $w.note.wbsl.label0 -row 0 -column 0 -sticky ew -columnspan 3 -pady 12 -padx 10

#ttk::labelframe $w.note.wbsl.lf -borderwidth 0 
ttk::label $w.note.wbsl.label1 -font "Helvetica 10" -text "Select the firmware file that you want to download to the watch:" 
ttk::entry $w.note.wbsl.entry0 -state readonly -textvariable select_input_file

grid $w.note.wbsl.label1 -row 1 -column 0 -sticky ew -columnspan 3 -pady 12 -padx 10
grid $w.note.wbsl.entry0 -row 2 -column 0 -sticky ew -columnspan 2 -padx 10

ttk::button $w.note.wbsl.btnBrowse -text "Browse..." -command { open_file } -width 16
grid $w.note.wbsl.btnBrowse -row 2 -column 2 -sticky ew -padx 10

ttk::button $w.note.wbsl.btnDwnld -text "Update Chronos Watch" -command { start_wbsl_ap } -width 16 -default "active"
grid $w.note.wbsl.btnDwnld -row 3 -column 0 -sticky ew -pady 20 -padx 8 -columnspan 3

# Progress bar
labelframe $w.note.wbsl.frame1p -borderwidth 0
ttk::label $w.note.wbsl.frame1p.lblProgress -text "Progress " -font "Helvetica 10 bold"
ttk::progressbar $w.note.wbsl.frame1p.progBar -orient horizontal -value 0 -variable wbsl_progress -mode determinate 
grid $w.note.wbsl.frame1p -row 4 -column 0 -sticky ew -pady 20 -padx 10 -columnspan 3
pack $w.note.wbsl.frame1p.lblProgress -side left 
pack $w.note.wbsl.frame1p.progBar -side left -fill x -expand 1 

#Dummy Labels to fill Space
ttk::label $w.note.wbsl.importantNote -width 80 -wraplength 640 -justify center -text "Important: If the wireless update fails during the firmware download to flash memory, the watch display will be blank and the watch will be in sleep mode. To restart the update, press the down button." -font "Helvetica 10 bold"
grid $w.note.wbsl.importantNote -row 5 -column 0 -sticky ew -columnspan 3 -pady 1 -padx 10

# Frame for status display
labelframe $w.note.wbsl.frame0b -borderwidth 1 -background "Yellow"
ttk::label $w.note.wbsl.frame0b.lblStatus -text "Status:" -font "Helvetica 10 bold" -background "Yellow"
ttk::label $w.note.wbsl.frame0b.lblStatusText -text "Access Point is off." -font "Helvetica 10" -background "Yellow"
grid $w.note.wbsl.frame0b -row 6 -column 0 -pady 10 -padx 10 -sticky ew -columnspan 3
pack $w.note.wbsl.frame0b.lblStatus -side left -fill x 
pack $w.note.wbsl.frame0b.lblStatusText -side left -fill x 


# ----------------------------------------------------------------------------------------
# About pane -----------------------------------------------------------------------------
ttk::frame $w.note.about -style custom.TFrame 
$w.note add $w.note.about -text "About" -underline 0 -padding 2
grid rowconfigure $w.note.about 1 -weight 1 -uniform 1 
grid columnconfigure $w.note.about {0 1} -weight 1 -uniform 1

# SimpliciTI box
ttk::labelframe $w.note.about.s -borderwidth 1 
ttk::label $w.note.about.s.txt1 -font "Helvetica 12 bold" -justify "left" -width 4  -anchor center -style custom.TLabel -text "SimpliciTI\u2122"
ttk::label $w.note.about.s.txt2 -font "Helvetica 10" -width 80 -wraplength 550 -justify left -anchor n -style custom.TLabel \
-text "SimpliciTI\u2122 is a simple low-power RF network protocol aimed at small RF networks.\
\n\nSuch networks typically contain battery operated devices which require long battery life, low data rate and low duty cycle and have a limited number of nodes talking directly to each other or through an access point or range extenders. Access point and range extenders are not required but provide extra functionality such as store and forward messages.\
\n\nWith SimpliciTI\u2122 the MCU resource requirements are minimal which results in low system cost."
ttk::label $w.note.about.s.txt3 -font "Helvetica 10 bold" -wraplength 550 -justify left -anchor n -style custom.TLabel -text "Learn more about SimpliciTI\u2122 at http://www.ti.com/simpliciti"
grid $w.note.about.s -row 0 -column 0 -sticky new -pady 0 -columnspan 2
pack $w.note.about.s.txt1 -side top -fill x -pady 5 -padx 2m
pack $w.note.about.s.txt2 -side top -fill x -pady 0 -padx 2m
pack $w.note.about.s.txt3 -side top -fill x -pady 5 -padx 2m

# BlueRobin box
ttk::labelframe $w.note.about.b -borderwidth 1 
ttk::label $w.note.about.b.txt1 -font "Helvetica 12 bold italic" -foreground "Dark Blue" -justify "left" -width 4  -anchor center -text "BlueRobin\u2122" -style custom.TLabel
ttk::label $w.note.about.b.txt2 -font "Helvetica 10" -width 80 -wraplength 550 -justify left -anchor n -style custom.TLabel \
-text "The BlueRobin\u2122 protocol provides low data rate transmission for wireless body area sensor networks and team monitoring systems. Ultra-low power consumption, high reliability and low hardware costs are key elements of BlueRobin\u2122.\
\n\nBlueRobin\u2122 is successfully used in personal and multi-user heart rate monitoring systems, sports watches, chest straps, foot pods, cycle computers and other fitness equipment."
ttk::label $w.note.about.b.txt3 -font "Helvetica 10 bold" -wraplength 550 -justify left -anchor n -style custom.TLabel -text "Learn more about BlueRobin\u2122 at http://www.bm-innovations.com"
grid $w.note.about.b -row 1 -column 0 -sticky new -pady 5 -columnspan 2
pack $w.note.about.b.txt1 -side top -fill x -pady 5 -padx 2m
pack $w.note.about.b.txt2 -side top -fill x -pady 0 -padx 2m
pack $w.note.about.b.txt3 -side top -fill x -pady 5 -padx 2m


# ----------------------------------------------------------------------------------------
# Help pane ------------------------------------------------------------------------------
ttk::frame $w.note.help -style custom.TFrame 
$w.note add $w.note.help -text "Help" -underline 0 -padding 2
grid rowconfigure $w.note.help 1 -weight 1 -uniform 1 
grid columnconfigure $w.note.help {0 1} -weight 1 -uniform 1

ttk::labelframe $w.note.help.frame -borderwidth 1
ttk::label $w.note.help.frame.head -font "Helvetica 12 bold" -justify "right" -width 4  -anchor center -style custom.TLabel -text "Help"
ttk::label $w.note.help.frame.txt1 -font "Helvetica 10" -width 80 -wraplength 500 -justify left -anchor n -style custom.TLabel \
-text "If you cannot communicate with the RF Access Point, please check the following points:\
\n\n1) Do you have another instance of the GUI open?\n\nIf so, please close it, since it may block the COM port.\
\n\n2) Is the RF Access Point mounted (will be done automatically by Linux)?\n\nIt must appear as '/dev/ttyACM0'. You might also want to run the 'lsusb' command. The RF Access Point should be listed as 'Bus xxx Device xxx: ID 0451:16a6 Texas Instruments, Inc.'. If not, disconnect the RF Access Point from the USB port and reconnect it."
pack $w.note.help.frame.head -side top -fill x -pady 10 -padx 5m
pack $w.note.help.frame.txt1 -side top -fill x -pady 10 -padx 5m
grid $w.note.help.frame -row 0 -column 0 -sticky ew -pady 0 -columnspan 2


# ----------------------------------------------------------------------------------------
# Generic SimpliciTI functions -----------------------------------------------------------

proc start_simpliciti_ap_acc { } {
  global w
  global simpliciti_on simpliciti_acc_on simpliciti_sync_on

  # AP already on?
  if { $simpliciti_on == 1 } { return } 

  set simpliciti_acc_on 1
  set simpliciti_sync_on 0
 
  start_simpliciti_ap
}


proc start_simpliciti_ap_sync { } {
  global w
  global simpliciti_on simpliciti_sync_on simpliciti_acc_on

  # AP already on?
  if { $simpliciti_on == 1 } { return } 

  set simpliciti_sync_on 1
  set simpliciti_acc_on 0
 
  start_simpliciti_ap

  after 1000
  catch { BM_GetStatus } status

  # Check RF Access Point status byte  
  if { $status == 3 } {
    updateStatusSPL "Access point started. Now start watch in sync mode."
  }

}



# Start RF Access Point
proc start_simpliciti_ap { } {
  global w
  global simpliciti_on bluerobin_on com_available
  global simpliciti_ap_started
  global wbsl_on

  # No com port?  
  if { $com_available == 0 } { return }
  
  # Wireless Update on?  
  if { $wbsl_on == 1 } { return }
  
  # In BlueRobin mode? -> Stop BlueRobin transmission
  if { $bluerobin_on == 1 } { 
    stop_bluerobin
    after 500
  } 

  updateStatusSPL "Starting access point."
  after 500

  # Link with SimpliciTI transmitter
  set result [ BM_SPL_Start ]
  if { $result == 0 } {
    updateStatusSPL "Failed to start access point."
    return
  }
  after 500
    
  # Set on flag after some waiting time  
  set simpliciti_on 1

  # Ignore dummy data from RF Access Point until it sends real values 
  set simpliciti_ap_started 0
  
  # Reconfigure control buttons
  $w.note.spl.frame0.btnStartStop configure -text "Stop Access Point" -command { stop_simpliciti_ap }
  $w.note.sync.f0.btn_start_ap configure -text "Stop Access Point" -command { stop_simpliciti_ap }
}


# Stop RF Access Point
proc stop_simpliciti_ap {} {
  global w
  global simpliciti_ap_started simpliciti_on simpliciti_acc_on simpliciti_sync_on bluerobin_on com_available
  global accel_x accel_y accel_z accel_x_offset accel_y_offset accel_z_offset

  # AP off?
  if { $simpliciti_on == 0 } { return } 

  # Clear on flag 
  set simpliciti_on 0
  set simpliciti_acc_on 0
  set simpliciti_sync_on 0
  
  # Send sync exit command (this will exit sync mode on watch side)
  BM_SYNC_SendCommand 0x07
  after 750
  
  # Stop SimpliciTI
  BM_SPL_Stop

  # Link is now off
  updateStatusSPL "Access point is off."

  # Clear values
  set accel_x  0
  set accel_y  0
  set accel_z  0  
  set accel_x_offset 0
  set accel_y_offset 0
  set accel_z_offset 0  
  set simpliciti_ap_started 0
  update
  
  # Reconfig button
  $w.note.spl.frame0.btnStartStop configure -text "Start Access Point" -command { start_simpliciti_ap_acc }
  $w.note.sync.f0.btn_start_ap configure -text "Start Access Point" -command { start_simpliciti_ap_sync }
}





# Generic file save dialog
proc file_save_dialog { w } {
  global ini_file
  
  # Define default file type
  set types {
  	{"eZ430-Chronos configuration"		{.ini}	}
  	{"All files"		*}
  }

  # Use standard Windows file dialog 
  set selected_type "eZ430-Chronos configuration"
  set ini_file [tk_getSaveFile -filetypes $types -parent $w -initialfile "eZ430-Chronos.ini" -defaultextension .ini]
}


        

# ----------------------------------------------------------------------------------------
# WBSL Update functions ------------------------------------------------------------------

# Prompt the user to select a file
proc open_file {} {
	global select_input_file
	global w
	set types {
		{{CC430 Firmware} {.txt}					}
	}
	set select_input_file [tk_getOpenFile -title "Select File" -filetypes $types] 
	
}


# Safely execute WBSL service functions (non-overlapping)
proc call_wbsl_funcs {} {
  global call_wbsl_timer call_wbsl_1 call_wbsl_2

  if { $call_wbsl_timer == $call_wbsl_1 } { 
    get_wbsl_packet_status 
    set call_wbsl_1 [expr $call_wbsl_timer + 2]
  } 
  if { $call_wbsl_timer == $call_wbsl_2 } { 
    get_wbsl_status
    set call_wbsl_2 [expr $call_wbsl_timer + 3]
  } 

  incr call_wbsl_timer
}


# Start the Wireless update procedure, and put RF Access Point in RX mode
proc start_wbsl_ap {} {
  global w
  global simpliciti_on bluerobin_on com_available
  global wbsl_on select_input_file
  global wbsl_ap_started
  global fsize
  global fp
  global rData
  global rData_index
  global low_index
  global list_count maxPayload 
  global ram_updater_downloaded
  global wirelessUpdateStarted

  # init needed variables
  set rData [list]
  set rData_index 0
  set low_index 0
  
  # No com port?  
  if { $com_available == 0} { return }
  
  # Testing REMOVE
  # set ram_updater_downloaded 1
  
  set ram_updater_file "ram_based_updater.txt"

  if { $ram_updater_downloaded == 0 } {
	  # Check that the user has selected a file    
	  if { [string length $select_input_file] == 0 } {
	  		tk_dialog .dialog1 "No file selected" {Please select a watch firmware file (*.txt) to download to the watch.} info 0 OK
	  		return
	  }
	  
	  # Check that the file selected by the user has the extension .txt 
	  if { [string first ".txt" $select_input_file] == -1 } {
	  	    tk_dialog .dialog1 "Invalid .txt File" {The file selected is not a .txt file.} info 0 OK
	  		return
	  }
  }

  # First off check that the file trying to be downloaded has the right format
  catch { file size $select_input_file } fsize
  
  # Check if the file exist
  if { [string first "no such file" $fsize] != -1 } {
  	tk_dialog .dialog1 "File doesnt exist" {The selected file doesnt exist, please verify the path.} info 0 OK
  	return
  }
  
  # Open the file for reading
  catch { open $select_input_file r } fp
  fconfigure $fp -translation binary

  # read the first character of the file, it should be an @ symbol
  set test_at [read $fp 1]

  if { $test_at != "@" } { 
    tk_dialog .dialog1 "Invalid .txt File" {The .txt file is NOT formatted correctly.} info 0 OK
    close $fp
    return
  }
  
  # read the complete file
  set rawdata [read $fp $fsize]
  close $fp
  # Remove spaces, tabs, endlines from the data
  regsub -all {[ \r\t\nq]*} $rawdata "" stripped_data
  set lines 0
  # Divide the file by the symbol @ so that in each list there is data to be written consecutively at the address indicated by the first 2 bytes
  set datainlist [split $stripped_data "@"]
  set list_count 0
  set byteCounter 0
  
  # For each line, convert the ASCII format in which is saved to Raw HEX format
  foreach lines $datainlist {
  	set lines [join $lines]
  	regsub -all {[ \r\t\nq]*} $lines "" line
  	if { [catch { binary format H* $line } line] } {
  	      tk_dialog .dialog1 "Invalid .txt File" {The .txt file is NOT formatted correctly.} info 0 OK
  	      return
      } 
      lappend rData $line
      incr list_count
   }
  
  # Check if the RAM_UPDATER is not yet on the watch so that we download this first
  if { $ram_updater_downloaded == 0 } {
  	  # init needed variables
      set rData [list]
      set rData_index 0
      set low_index 0
      
	  catch { file size $ram_updater_file} fsize
	  
	  # Check that the RAM Updater file is present on the GUI working directory
	  if { [string first "no such file" $fsize] != -1 } {
	    	tk_dialog .dialog1 "No Updater File" {The RAM Updater File is not present on the working directory. Filename should be:ram_based_updater.txt} info 0 OK
	     return
	  }
	  
	  catch { open $ram_updater_file r } fp
	  fconfigure $fp -translation binary
	  
	  set test_at [read $fp 1]
	 
	  if { $test_at != "@" } { 
	  	tk_dialog .dialog1 "Invalid .txt File" {The ram_based_updater.txt file is NOT formatted correctly.} info 0 OK
	  	close $fp
	  	return
	  }
	  
	  set rawdata [read $fp $fsize]
	  close $fp
	  # Remove spaces, tabs, endlines from the data
	  regsub -all {[ \r\t\nq]*} $rawdata "" stripped_data
	  
	  set datainlist [split $stripped_data "@"]
	  set list_count 0
	  set byteCounter 0
	  foreach lines $datainlist {
	  		set lines [join $lines]
	  		regsub -all {[ \r\t\nq]*} $lines "" line
	  		if { [catch { binary format H* $line } line] } {
	  	    	  tk_dialog .dialog1 "Invalid .txt File" {The ram_based_updater.txt file is NOT formatted correctly.} info 0 OK
	  	    	  return
	  	    } 
	  	    lappend rData $line
	  	    incr list_count
	  	}
  }
  # In AP mode?
  if { $simpliciti_on == 1 } { 
    stop_simpliciti_ap
    after 500
  } 
  
  # In BlueRobin mode?
  if { $bluerobin_on == 1 } { 
    stop_bluerobin
    after 500
  } 

  updateStatusWBSL "Starting Wireless Update."
  after 200

  # Link with WBSL transmitter
  BM_WBSL_Start
  after 100
  
  set result [ BM_WBSL_GetMaxPayload ]
  after 10
  set result [ BM_WBSL_GetMaxPayload ]
  if { $result < 2 } {
    updateStatusWBSL "$result Failed to start Wireless Update."
    return
  }

  set maxPayload $result
  
  # Calculate the number of packets needed to be sent
  
  #initialize the number of packets
  set fsize 0
  
  # sum up all the bytes to be sent
  foreach block $rData {
  	set byteCounter [string length $block]
  	set dByte [expr {double($byteCounter)}]
  	set dMax [expr {double($maxPayload)} ]
  	set temp [ceil [expr  $dByte / $dMax]]
  	set fsize [expr $fsize + $temp]
  }
    
  # Set on WBSL flag   
  set wbsl_on 1

  # Cancel out first received data
  set wbsl_ap_started 0
  
  # Reconfig buttons
  $w.note.wbsl.btnDwnld configure -text "Cancel Update" -command { stop_wbsl_ap }
  
}

# Stop the wireless update procedure
proc stop_wbsl_ap {} {
  global w
  global simpliciti_on bluerobin_on com_available wbsl_on
  global ram_updater_downloaded
  global wirelessUpdateStarted

  # AP off?
  if { $wbsl_on == 0 } { return }
  
  # Clear on flags  
  set wbsl_on 0
  set ram_updater_downloaded 0

  after 500
 
  BM_WBSL_Stop

  # Show that link is inactive
  updateStatusWBSL "Wireless Update is off."

  update
  
  # Initialize the variable that tell us when the update procedure has been initiated by the Watch
  set wirelessUpdateStarted 0
  # Reconfig button and re-enable it in case the update procedure was started and it was disabled during the procedure
  $w.note.wbsl.btnDwnld configure -text "Update Chronos Watch" -command { start_wbsl_ap } -state enabled
  
}

proc get_wbsl_packet_status {} {
  global w
  global wbsl_on 
  global wbsl_ap_started
  global fsize
  global fp
  global rData   
  global rData_index
  global low_index
  global list_count maxPayload
  global wbsl_opcode
  global ram_updater_downloaded
  global vcp_reply
  
  set status 0
  
  # return if WBSL is not active  
  if { $wbsl_on == 0 } { return }
 
  # Check packet status
  set status [ BM_WBSL_GetPacketStatus ]
 
  if { $status == 1 } {
    # WBSL_DISABLED Not started by watch
    return
  } elseif { $status == 2 } {
  	# WBSL Is processing a packet
        return	
  } elseif { $status == 4 } {
	# Send the size of the file
	set packets [expr {int($fsize)} ]
	# send opcode 0 which is a info packet, which contains the total packets to be sent
	catch { BM_WBSL_SendData 0 $packets } status 
    # The next packet will contain an address
	set wbsl_opcode 1
  } elseif { $status == 8 } {
	# Send the next data packet
		
	if { $rData_index <  $list_count } {
		# Choose the appropriate block of data
		set data_block [lindex $rData $rData_index]
		# Get the size of the block of data, to know if we have sent all of the data in this block and move to the next
		set block_size [string length $data_block]
		# Read MaxPayload Bytes from the list
		set c_data [string range $data_block $low_index [expr $low_index + [expr $maxPayload - 1]]]
		
		# Send the read bytes to the dongle
		set status [BM_WBSL_SendData $wbsl_opcode $c_data] 
		
		#update the low index
		set low_index [expr $low_index + $maxPayload]
		
		# Next packet is a normal data packet
		set wbsl_opcode 2
		
		if { $low_index >= $block_size } { 
			incr rData_index
			set 	low_index 0
			# Next packet will include an address at the beginning of the packet
			set wbsl_opcode 1
		}
	 }
  } else {
  	# ERROR only the previous options should be returned
    if { $ram_updater_downloaded == 0 } {
  		tk_dialog .dialog1 "Error in communication" {There was an error in the communication between the RF Access Point and the watch during the download to RAM. The watch should have reset itself. Please retry the update the same way as before.} info 0 OK
    } else {
      tk_dialog .dialog1 "Error in communication" {There was an error in the communication between the RF Access Point and the watch during the download to Flash. The watch is in a sleep mode now. Please press the "Update Chronos Watch" first and then press the down button on the watch to restart the update.} info 0 OK
    }
  	after 200
  	stop_wbsl_ap 
  	return
  }
}

# Get the global status of the AP, check if the state in which the GUI is, matches the state of the AP
proc get_wbsl_status {} {
  global w vcp
  global wbsl_on
  global wbsl_ap_started wbsl_progress
  global ram_updater_downloaded
  global wirelessUpdateStarted
  global wbsl_timer_flag
  global vcp_reply fh
  
  set status 0
  
  # return if WBSL is not active  
  if { $wbsl_on == 0 } { return }

  # Check if the flag has been set, which means the communication was lost while trying to link to download the update image
 if { $wbsl_timer_flag == 1 } {
	     tk_dialog .dialog1 "Error in communication" {There was an error in the communication between the AP and the Watch while trying to start the download to Flash. The watch should have reset itself. Please retry the update the same way as before.} info 0 OK
	     wbsl_reset_timer	     
	     after 300
	     stop_wbsl_ap
	     return
    }

  # Update status box  
  set status [ BM_GetStatus1 ]

  if { $status == 9 } {
    # just starting AP
    updateStatusWBSL "Starting access point."
    return
  # Check if there was an error during the communication between the AP and the watch
  } elseif { $status == 11 || $status == 12 } {
  	
  	if { $ram_updater_downloaded == 0 } {
  		tk_dialog .dialog1 "Error in communication" {There was an error in the communication between the RF Access Point and the watch during the download to RAM. The watch should have reset itself. Please retry the update the same way as before.} info 0 OK
    } else {
        tk_dialog .dialog1 "Error in communication" {There was an error in the communication between the RF Access Point and the watch during the download to Flash. The watch is in sleep mode now. Please press the "Update Chronos Watch" first and then press the down button on the watch to restart the update.} info 0 OK
    }
	after 300
	stop_wbsl_ap 
	
  } elseif { $status == 10 } {
    
    # Read WBSL data from dongle
    set data [ BM_WBSL_GetStatus ]
   # if { $data == "" }  { return }
    
    if { $wbsl_ap_started == 0} {
    	if { $ram_updater_downloaded == 0 } {
      updateStatusWBSL "Access point started. Now start watch in rFbSL mode."
after 2000
     } else {
     	updateStatusWBSL "Starting to download update image to watch."
     	# We will now try to link with the watch to start downloading the Update Image, we need a timer in case the communication is lost
     	# while trying to link, since for the linking to start, the Dongle normally waits until the watch initiates the procedure.
     	wbsl_set_timer 1
     	update
     }
      set wbsl_ap_started 1
      return
    } else {
   
      # Check if data is valid
      if { $data < 0 } {
        return
      } 
     
     set wbsl_progress $data
     
     if { $wbsl_progress != 0 } {
     	   
     	   if { $wirelessUpdateStarted == 0 } {
     	   	
     	   	  set wirelessUpdateStarted 1
     	      # Reconfig buttons
  		    $w.note.wbsl.btnDwnld configure -state disabled
  	     }
     	   
	     if { $ram_updater_downloaded == 1 } {
	     	 # The download to FLASH has started, we don't need the timer to keep running
	     	 wbsl_reset_timer
	     	 update
	    	  updateStatusWBSL "Downloading update image to watch. Progress: [format %d $wbsl_progress]%"	
           
		    if { $wbsl_progress >= 100 } { 
		  	    updateStatusWBSL "Image has been successfully downloaded to the watch"	
		  	    after 1500
		  	    stop_wbsl_ap
		  	   }
		  	
	     } else {
	     	 updateStatusWBSL "Downloading the RAM Based Updater. Progress: [format %d $wbsl_progress]%"	

#puts $fh "\nDownloading the RAM Based Updater.\n"

	     	  if { $wbsl_progress >= 100 } { 
		  	    updateStatusWBSL "RAM Based Updater downloaded. Starting download of update image."	
		  	    set ram_updater_downloaded 1
		  	    BM_WBSL_Stop
		  	    set wbsl_on 0
		  	    start_wbsl_ap
		  	   }
	     }
     }
      return
      }
   } 
}

# Stop and reset the timer variables
proc wbsl_reset_timer { } {
 	global wbsl_timer_enabled
 	global wbsl_timer_counter
 	global wbsl_timer_flag
 	global wbsl_timer_timeout
 	
 	set  wbsl_timer_counter  0
 	set  wbsl_timer_flag     0
 	set  wbsl_timer_timeout  0
 	set  wbsl_timer_enabled  0
 }

# Set the timeout variable and start the timer
proc wbsl_set_timer { timeout } {
 	global wbsl_timer_enabled
 	global wbsl_timer_counter
 	global wbsl_timer_flag
 	global wbsl_timer_timeout
 	
 	set  wbsl_timer_counter  0
 	set  wbsl_timer_flag     0
 	set  wbsl_timer_timeout  $timeout
 	set  wbsl_timer_enabled  1
 }

# Called every 2.5 seconds, acts as the timer, it only counts if it's enabled
proc wbsl_simple_timer {} {
 	global wbsl_timer_enabled
 	global wbsl_timer_counter
 	global wbsl_timer_flag
 	global wbsl_timer_timeout
 	
 	if { $wbsl_timer_enabled == 0 } { 
 	   return	
 	}
    	set wbsl_timer_counter [expr $wbsl_timer_counter+1]
    	if { $wbsl_timer_counter > $wbsl_timer_timeout } {
    	    	set wbsl_timer_flag 1
          set wbsl_timer_enabled 0
    }
    
}



# ----------------------------------------------------------------------------------------
# System functions -----------------------------------------------------------------------

# Create Windows key events
proc button_set { btn } {
  global pd_m1 pd_m2 pd_s1
  global cb_m1_windows cb_m1_alt cb_m1_ctrl cb_m1_shift
  global cb_s1_windows cb_s1_alt cb_s1_ctrl cb_s1_shift
  global cb_m2_windows cb_m2_alt cb_m2_ctrl cb_m2_shift
  
  # Button select
  switch $btn {
    "M1"  { set pd          $pd_m1
            set cb_windows  $cb_m1_windows
            set cb_alt      $cb_m1_alt
            set cb_ctrl     $cb_m1_ctrl
            set cb_shift    $cb_m1_shift }
    "M2"  { set pd          $pd_m2
            set cb_windows  $cb_m2_windows
            set cb_alt      $cb_m2_alt
            set cb_ctrl     $cb_m2_ctrl
            set cb_shift    $cb_m2_shift }
    "S1"  { set pd          $pd_s1
            set cb_windows  $cb_s1_windows
            set cb_alt      $cb_s1_alt
            set cb_ctrl     $cb_s1_ctrl
            set cb_shift    $cb_s1_shift }
    default { return }
  }
  
  # Convert key to key symbol
  set keysymbol 0
  if { [string length $pd] == 1 } {
    set keysymbol $pd
  } else {
    # Convert special keys
    switch $pd {
      "Space"         { set keysymbol "space" }
      "Arrow-Left"    { set keysymbol "Left" } 
      "Arrow-Right"   { set keysymbol "Right" }
      "F5"            { set keysymbol "F5" }
    }
  }

  # Simulate complex key event
  BM_SetKey $keysymbol $cb_windows $cb_alt $cb_ctrl $cb_shift
}




# ----------------------------------------------------------------------------------------
# Status output functions ----------------------------------------------------------------

proc updateStatusWBSL { msg } {
  global w
  $w.note.wbsl.frame0b.lblStatusText configure -text $msg
  update
}



# ----------------------------------------------------------------------------------------
# Start / stop application ---------------------------------------------------------------

# Exit application
proc exitpgm {} {
  exit 0
}

# Exit program
if { $exit_prog == 1 } { exitpgm }



# ----------------------------------------------------------------------------------------
# Periodic functions  --------------------------------------------------------------------

proc every {ms body} {eval $body; after $ms [info level 0]}
 every 25   { get_spl_data }
every 10   { call_wbsl_funcs }
every 2500 { wbsl_simple_timer }

