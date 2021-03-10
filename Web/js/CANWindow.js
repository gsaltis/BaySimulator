/*****************************************************************************
 * FILE         : CANWindow.js
 * DATE         : December 31 2018
 * PROJECT      : 
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var CANWindowOpen = false;
var CANMonitorRunning = false;
var CANWindowEntryHeight = 19;

/*****************************************************************************!
 * Function : CANWindowToggle
 *****************************************************************************/
function CANWindowToggle() {
    var windowSwitch = document.getElementById("CANWindowSwitch");
    var windowSwitchOn = document.getElementById("CANWindowSwitchOn");
    var windowSwitchOff = document.getElementById("CANWindowSwitchOff");
    var mainWindow = document.getElementById("MainWindow");
    var canWindow = document.getElementById("CANWindow");
    
    console.log("CANWindowToggle", CANWindowOpen);
    if ( CANWindowOpen ) {
        console.log("Not Open")
        CANWindowOpen = false;
        windowSwitchOff.setAttribute("fill", "#666");
        windowSwitchOn.setAttribute("fill", "none");
        mainWindow.style.right="5px";
        canWindow.style.visibility = "hidden";
        document.getElementById("CANWindowMenuButtonStart").style.visibility = "hidden";
        document.getElementById("CANWindowMenuButtonPause").style.visibility = "hidden";
        CANMonitorRunning = false;
        SetMonitorCANProtocol(CANMonitorRunning);
    } else {
        console.log("Open")
        CANWindowOpen = true;
        windowSwitchOff.setAttribute("fill", "none");
        windowSwitchOn.setAttribute("fill", "#666");
        mainWindow.style.right="500px";
        canWindow.style.visibility = "visible";
        document.getElementById("CANWindowMenuButtonStart").style.visibility = "visible";
        document.getElementById("CANWindowMenuButtonPause").style.visibility = "hidden";
    }
    console.log(windowSwitch);
}

/*****************************************************************************!
 * Function : CANWindowAddEntry
 *****************************************************************************/
function CANWindowAddEntry(InCANInfo) {
    if ( CANMonitorRunning == false ) {
        return;
    }
    
    var canWindow = document.getElementById("CANWindowTable");
    var entry = document.createElement("div");
    entry.className = "CANWindowEntry";
    var n = canWindow.children.length;
    x = CANWindowEntryHeight * n;
    entry.style.top = x;
    canWindow.appendChild(entry);

    entry.innerHTML = InCANInfo.time + " ";
    if ( InCANInfo.direction == "IN" ) {
        entry.className += " CANWindowEntryIn";
        entry.innerHTML += "&nbsp;";
    } else {
        entry.className += " CANWindowEntryOut";
    }
    entry.innerHTML += InCANInfo.direction;
        
    entry.innerHTML += " " + InCANInfo.id + " ";

    k = 0;
    for (i = 0; i < InCANInfo.dataLength; i++) {
        entry.innerHTML += InCANInfo.data.substring(k, k + 2) + " ";
        k += 2;
    }
}
    
/*****************************************************************************!
 * Function : CANWindowAddStringEntry
 *****************************************************************************/
function CANWindowAddStringEntry(InCANInfo) {
    if ( CANMonitorRunning == false ) {
        return;
    }
    
    var canWindow = document.getElementById("CANWindowTable");
    var entry = document.createElement("div");
    entry.className = "CANWindowEntry";
    var n = canWindow.children.length;
    var x = CANWindowEntryHeight * n;
    entry.style.top = x;
    canWindow.appendChild(entry);

    var t = InCANInfo.packet;

    if ( t.substring(0, 4) == "Read" ) {    
        entry.className += " CANWindowEntryIn";
    } else {
        entry.className += " CANWindowEntryOut";
    }
    entry.innerHTML = t;
}
    
/*****************************************************************************!
 * Function : CANMonitorStart 
 *****************************************************************************/
function CANMonitorStart()
{
    CANMonitorRunning = true;
    document.getElementById("CANWindowMenuButtonPause").style.visibility="visible"
    document.getElementById("CANWindowMenuButtonStart").style.visibility="hidden"
    SetMonitorCANProtocol(CANMonitorRunning);
}

/*****************************************************************************!
 * Function : CANMonitorPause
 *****************************************************************************/
function CANMonitorPause()
{
    CANMonitorRunning = false;
    document.getElementById("CANWindowMenuButtonPause").style.visibility="hidden"
    document.getElementById("CANWindowMenuButtonStart").style.visibility="visible"
    SetMonitorCANProtocol(CANMonitorRunning);
}

/*****************************************************************************!
 * Function : CANMonitorStop
 *****************************************************************************/
function CANMonitorStop()
{
    CANMonitorRunning = false;
    SetMonitorCANProtocol(CANMonitorRunning);
    document.getElementById("CANWindowMenuButtonPause").style.visibility="hidden"
    document.getElementById("CANWindowMenuButtonStart").style.visibility="visible"
    CANMonitorClear();
}

/*****************************************************************************!
 * Function : CANMonitorClear
 *****************************************************************************/
function CANMonitorClear()
{
    var canWindow = document.getElementById("CANWindowTable");
    var n, i;
    
	n = canWindow.childNodes.length;
	for (i = 0; i < n; i++) {
		canWindow.removeChild(canWindow.childNodes[0]);
    }
}
