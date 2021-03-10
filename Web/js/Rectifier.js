/*****************************************************************************
 * FILE         : Rectifier.js
 * DATE         : December 31 2018
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *****************************************************************************/

/*global FindSelectedBay*/

/*****************************************************************************!
 * Local Data 
 *****************************************************************************/
var editingRectifier = "";
var rectifierMajorAlarmNames = [
    "ACH", "ACL", "CBO", "CBO", "CNV", "DCU", "DTS", "EMS", "FMA", "INH",
    "NPD", "PHL", "PFF", "PHV", "PTS", "RHV", "SWO", "DNP", "DHV"
];

var rectifierMinorAlarmNames = [
    "FMI", "HAT", "TCL", "SNS"
];

var NODE_TYPE_RECTIFIER = 2;
var NODE_TYPE_DSM = 1;

/*****************************************************************************!
 * Function : RectifierInfo
 *****************************************************************************/
function RectifierInfo()
{
    this.ratedCurrent = 200;
    this.outputCurrent = 32;
    this.outputVoltage = 0;
    this.commStatus = false;
    this.startUpState = 0;
    this.serialNumber = nextSerialNumber++;
    this.productNumber = "0"; // "486534003"
    this.power = false;
    this.customerText = "";
    this.ratedInputVoltage = "220V";
    this.ratedOutputVoltage = "48V";
    this.nodeInfo = {};
    this.CANAddress = 0;
    this.alarms = {};
    this.alarms.ACH = false;
    this.alarms.ACL = false;
    this.alarms.CBO = false;
    this.alarms.CNV = false;
    this.alarms.DCU = false;
    this.alarms.DTS = false;
    this.alarms.EMS = false;
    this.alarms.FMA = false;
    this.alarms.INH = false;
    this.alarms.NPD = false;
    this.alarms.PHL = false;
    this.alarms.PFF = false;
    this.alarms.PHV = false;
    this.alarms.PTS = false;
    this.alarms.RHV = false;
    this.alarms.SWO = false;
    this.alarms.FMI = false;
    this.alarms.HAT = false;
    this.alarms.TCL = false;
    this.alarms.SNS = false;
    this.alarms.DNP = false;
    this.alarms.DHV = false;

    this.AnyMajorAlarms = function () {
        var anyAlarm =
            this.alarms.ACH  ||
            this.alarms.ACL  ||
            this.alarms.CBO  ||
            this.alarms.CBO  ||
            this.alarms.CNV  ||
            this.alarms.DCU  ||
            this.alarms.DTS  ||
            this.alarms.EMS  ||
            this.alarms.FMA  ||
            this.alarms.INH  ||
            this.alarms.NPD  ||
            this.alarms.PHL  ||
            this.alarms.PFF  ||
            this.alarms.PHV  ||
            this.alarms.PTS  ||
            this.alarms.RHV  ||
            this.alarms.DNP  ||
            this.alarms.DHV  ||
            this.alarms.SWO;
        return anyAlarm;
    }

    this.AnyMinorAlarms = function() {
        var anyAlarm =
            this.alarms.FMI  ||
            this.alarms.HAT  ||
            this.alarms.TCL  ||
            this.alarms.SNS;
        return anyAlarm;
    }
    
    this.ClearAlarms = function() {
        this.alarms.ACH = false;
        this.alarms.ACL = false;
        this.alarms.CBO = false;
        this.alarms.CBO = false;
        this.alarms.CNV = false;
        this.alarms.DCU = false;
        this.alarms.DTS = false;
        this.alarms.EMS = false;
        this.alarms.FMA = false;
        this.alarms.INH = false;
        this.alarms.NPD = false;
        this.alarms.PHL = false;
        this.alarms.PFF = false;
        this.alarms.PHV = false;
        this.alarms.PTS = false;
        this.alarms.RHV = false;
        this.alarms.SWO = false;
        this.alarms.DNP = false;
        this.alarms.FMI = false;
        this.alarms.HAT = false;
        this.alarms.TCL = false;
        this.alarms.SNS = false;
    };
        
}

/*****************************************************************************!
 * FUnction : RectifierEditRemoveCB
 *****************************************************************************/
function
RectifierEditRemoveCB()
{
    var rect = editingRectifier
    var editBox = document.getElementById("RectifierEdit");
    var update = {};
    editBox.style.visibility = "hidden";
    document.getElementById("ModeBlock").style.visibility = "hidden";
    HideRectifier(rect.bay, rect.rectifierIndex);
    update.command = "remove";
    update.type = "rectifier";
    update.bayIndex = GetBayIndex(rect.bay);
    update.unitNumber = rect.rectifierIndex.toString();
    SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : AddRectifierCB
 *****************************************************************************/
function
AddRectifierCB()
{
    var bay, n, i;
    bay = FindSelectedBay();
    if ( bay == null ) {
        return;
    }
    for (i = 0; i < bay.rectifiers.length; i++) {
	if ( bay.children[i].elementType == "Rectifier" ) {
	    rect = bay.children[i];
	    if ( rect.children[1].slotSelected ) {
		console.log(i);
		break;
	    }
	}
    }
    n = bay.rectifiers.length;
    for (i = 0; i < n; i++ ) {
        if ( bay.rectifiers[i].style.visibility == "hidden" ) {
            DisplayRectifier(bay.rectifiers[i]);
            break;
        }
    }
}

/******************************************************************************!
 * Function : 
 ******************************************************************************/
function DisplayRectifier(InRectifier) {
    InRectifier.children[1].style.visibility = "hidden";
    InRectifier.children[0].style.visibility = "visible";
}

/*****************************************************************************!
 * Function : CreateRectifier
 *****************************************************************************/
function CreateRectifier(InBay, InRectifierIndex) {
    var                     rect;
    var                     r, y, n, r, c, i, r2;
    var                     path;
    var                     defs;
    
    n = InBay.rectifiers.length;
    y = BayHeight - (((RectifierHeight - 1) * RectifierCount) + (RectifierCount-1));
    y += ( n * (RectifierHeight - 1) - 1 + n);

    var info = new RectifierInfo();
    info.ClearAlarms()
    rect = document.createElementNS(SVGNS, "svg");
    rect.id = "Slot " + InRectifierIndex;
    rect.elementType = "Rectifier"
    rect.rectifierInfo = info;
    rect.style.position = "absolute";
    rect.style.width=BayWidth;
    rect.style.height=RectifierHeight
    rect.style.top = y;
    rect.style.left = "0px";
    rect.style.border = "solid 1px black;";
    rect.style.visibility = "hidden";
    rect.selected = false;
    rect.onclick = function(event) { RectifierSelectedCB(event); };
    rect.ondblclick = function(event) { RectifierSelectedDoubleClickCB(event); };
    rect.bay = InBay;
    rect.rectifierIndex = InRectifierIndex;

    group = document.createElementNS(SVGNS, "g");
    rect.appendChild(group);
    
    slot = document.createElementNS(SVGNS, "g");
    r2 = document.createElementNS(SVGNS, "rect");
    slot.appendChild(r2);
    slot.onclick = function(InEvent) { SelectEmptySlotCB(InEvent); }
    slot.ondblclick = function(event) { NewRectifierAddToSlotCB(event); };
    slot.slotSelected = false;
    r2.style.cursor = "hand";
    r2.setAttribute("x", 0);
    r2.setAttribute("y", 0);
    r2.setAttribute("width", BayWidth);
    r2.setAttribute("height", RectifierHeight);
    r2.setAttribute("stroke", "blue");
    r2.setAttribute("fill", "#00000060");
    r2.setAttribute("stroke-width", .25);
    rect.append(slot);
    t1 = document.createElementNS(SVGNS, "text");
    t1.setAttribute("x", 5);
    t1.setAttribute("y", 20);
    t1.setAttribute("font-size", 20);
    t1.setAttribute("fill", "#00000030");
    t1.setAttribute("stroke", "#00000080");
    t1.setAttribute("stroke-width", .75);
    t1.textContent = InRectifierIndex;
    slot.appendChild(t1);
    defs = document.createElementNS(SVGNS, "defs");
    powerGradient = document.createElementNS(SVGNS, "radialGradient");
    powerGradient.id = "PowerGradient";
    stop = document.createElementNS(SVGNS, "stop");
    stop.setAttribute("offset", "50%");
    stop.setAttribute("stop-color", "#484");
    powerGradient.appendChild(stop);
    stop = document.createElementNS(SVGNS, "stop");
    stop.setAttribute("offset", "50%");
    stop.setAttribute("stop-color", "#080");
    powerGradient.appendChild(stop);
    defs.appendChild(powerGradient);
    
    
    group.appendChild(defs);
    r = document.createElementNS(SVGNS, "rect");
    r.setAttribute("x", 0);
    r.setAttribute("y", 0);
    r.setAttribute("width", BayWidth);
    r.setAttribute("height", RectifierHeight);
    r.setAttribute("fill", "#ccc");
    r.setAttribute("stroke", "black");
    r.setAttribute("stroke-width", 1);
    r.id = "Base";

    r2 = document.createElementNS(SVGNS, "rect");
    r2.setAttribute("x", 15);
    r2.setAttribute("width", BayWidth - 30);
    r2.setAttribute("y", 3);
    r2.setAttribute("height", RectifierHeight - 6);
    r2.setAttribute("fill", "#BBB");

    group.appendChild(r);
    group.appendChild(r2);

    for (i = 0; i < 7; i++) {
        li = document.createElementNS(SVGNS, "line");
        li.setAttribute("x1", 15);
        li.setAttribute("x2", BayWidth - 15);
        li.setAttribute("y1", 3 + (i*3));
        li.setAttribute("y2", 3 + (i*3));
        li.setAttribute("stroke", "#666");
        group.appendChild(li);
    }
    li = document.createElementNS(SVGNS, "line");
    li.setAttribute("x1", 15);
    li.setAttribute("x2", 15);
    li.setAttribute("y1", 3);
    li.setAttribute("y2", RectifierHeight - 3);
    li.setAttribute("stroke", "#666");
    group.appendChild(li);
    
    li = document.createElementNS(SVGNS, "line");
    li.setAttribute("x1", BayWidth - 15);
    li.setAttribute("x2", BayWidth - 15);
    li.setAttribute("y1", 3);
    li.setAttribute("y2", RectifierHeight - 3);
    li.setAttribute("stroke", "#666");
    group.appendChild(li);

    for (i = 1; i < 6; i++ ){
        li = document.createElementNS(SVGNS, "line");
        li.setAttribute("x1", 15 + (i * 19));
        li.setAttribute("x2", 15 + (i * 19));
        li.setAttribute("y1", 3);
        li.setAttribute("y2", RectifierHeight - 3);
        li.setAttribute("stroke", "#DDD");
        li.setAttribute("stroke-width", "2");
        group.appendChild(li);
    }

    path = document.createElementNS(SVGNS, "path");
    path.setAttribute('d', "M62,1  82,1, 72,20 62,1");
    path.setAttribute("fill", "#DDD");
    group.appendChild(path);
    
    
    li = document.createElementNS(SVGNS, "line");
    li.setAttribute("x1", 72);
    li.setAttribute("x2", 72);
    li.setAttribute("y1", 3);
    li.setAttribute("y2", RectifierHeight - 3);
    li.setAttribute("stroke", "#DDD");
    li.setAttribute("stroke-width", "8");
    group.appendChild(li);

    CreateRectifierAlarmIndicator(rect, group);
    CreateRectifierPowerSwitch(rect, group);
    CreateRectifierCommIndicator(rect, group);
    InBay.rectifiers.push(rect);
    InBay.append(rect);


    CreateRectifierPowerGuage(rect, group);
    CreateRectifierLEDs(rect, group);
    SetRectifierOutputCurrent(rect, rect.rectifierInfo.outputCurrent);
}

/*****************************************************************************!
 * Function : NewRectifierAddToSlotCB
 *****************************************************************************/
function NewRectifierAddToSlotCB(InEvent) {
    var bay = FindSelectedBay();
    var index = InEvent.srcElement.parentElement.parentElement.rectifierIndex;
    DisplayRectifier(bay.rectifiers[index-1]);
}

/*****************************************************************************!
 * Function : SelectEmptySlotCB
 *****************************************************************************/
function SelectEmptySlotCB(InEvent) {
    slot = InEvent.srcElement;
    rectifier = slot.parentElement.parentElement;
    bay = rectifier.parentElement;

    for (i = 0; i < bay.children.length; i++) {
	s = bay.children[i];
	if (s.elementType == "Rectifier" ) {
	    s.children[1].children[0].setAttribute("fill", "#00000060");
	    s.children[1].slotSelected = false;
	}
    }
    slot.setAttribute("fill", "#00ff0020");
    slot.slotSelected = true;
}

/*****************************************************************************!
 * Function : CreateRectifierCommIndicator
 *****************************************************************************/
function CreateRectifierCommIndicator(InRectifier, InGroup) {
  var path = document.createElementNS(SVGNS, "path");
  path.setAttribute('d', 
                    "M2, 3 " + 
                    "13, 3 " + "13, 8 " + "8 13, " + "7 13, " + "2, 8 " + "2, 3");
  path.setAttribute("fill", "#00AA0040");
  path.setAttribute("stroke", "#000000040")
  path.setAttribute("stroke-width", ".5")
  path.id = "CommIndicator";
  InGroup.appendChild(path);
}

/*****************************************************************************!
 * Function : CreateRectifierAlarmIndicator
 *****************************************************************************/
function CreateRectifierAlarmIndicator(InRectifier, InGroup) {
    var c;
    var elementName = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-Alarm";
    var statusX = (BayWidth / 2);
    var statusY = (RectifierHeight / 4);

    
    c = document.createElementNS(SVGNS, "circle");
    c.setAttribute("cx", statusX);
    c.setAttribute("cy", statusY);
    c.setAttribute("r", 3);
    c.setAttribute("fill", "#DDD");
    c.setAttribute("stroke", "black");
    c.setAttribute("stroke-width", ".5");
    c.id = elementName;
    InGroup.append(c);
}

/*****************************************************************************!
 * Function : CreateRectifierPowerSwitch
 *****************************************************************************/
function
CreateRectifierPowerSwitch(InRectifier, InGroup) {
    var path, outline;
    path = document.createElementNS(SVGNS, "path");
    path.id = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-PowerSwitch";
    path.setAttribute('d', "M70,12 74,12 74,20 70,20");
    path.setAttribute("fill", "#666");
    path.style.cursor = "hand";

    path.rectifier = InRectifier;
    path.elementType = "PowerSwitch";
    outline = document.createElementNS(SVGNS, "path");
    outline.setAttribute('d', "M70,12 74,12 74,20 70,20 70,12");
    outline.setAttribute("fill", "none");
    outline.setAttribute("stroke", "#444");
    outline.setAttribute("stroke-width", ".5");
    outline.elementType = "PowerSwitch";
    outline.rectifier = InRectifier;
    path.onclick = function(InEvent) { ToggleRectifierPower(InEvent); }
    InGroup.appendChild(path);    
    InGroup.appendChild(outline);    
}

/*****************************************************************************!
 * Function : CreateRectifierPowerGuage
 *****************************************************************************/
function CreateRectifierPowerGuage(InRect, InGroup) {
    var                             gauge;
    var                             x, y, x2, y2, width, height;
    var                             path, outline;
    x = BayWidth - 13;
    y = 3;
    width = 4;
    height = 20;
    x2 = x + width;
    y2 = y + height;
    gauge = document.createElementNS(SVGNS, "rect");
    InGroup.appendChild(gauge);
    gauge.elementType = "PowerGuage";
    gauge.rectifier = InRect;
    gauge.setAttribute("x", x);
    gauge.id = "Bay-" + InRect.bay.bayName + "-Rectifier-" + InRect.rectifierIndex + "-PowerGuage";
    gauge.setAttribute("y", y);
    gauge.setAttribute("width", width);
    gauge.setAttribute("height", height);
    gauge.setAttribute("fill", "#888");
    gauge.setAttribute("stroke", "");

    outline = document.createElementNS(SVGNS, "path");
    path = "M" + x + "," + y + " ";
    path += x2 + "," + y + " ";
    path += x2 + "," + y2 + " ";
    path += x + "," + y2 + " ";
    path += x + "," + y ;
    outline.setAttribute('d', path);
    outline.setAttribute("fill", "none");
    outline.setAttribute("stroke", "#444");
    outline.setAttribute("stroke-width", ".5");
    outline.rectifier = InRect;
    outline.elementType = "PowerGuage";
    InGroup.appendChild(outline);
}

/*****************************************************************************!
 * Function : ToggleRectifierPower
 *****************************************************************************/
function ToggleRectifierPower(InEvent) {
    var elementName, rectifier;

    rectifier = InEvent.srcElement.parentElement.parentElement;
    if ( rectifier.rectifierInfo.power ) {
        rectifier.rectifierInfo.power = false;
        TurnOffRectifier(rectifier);
    } else {
        rectifier.rectifierInfo.power = true;
        TurnOnRectifier(rectifier);
    }
}

/*****************************************************************************!
 * Function : CreateRectifierLEDs
 *****************************************************************************/
function CreateRectifierLEDs(InRect, InGroup) {
    var                             led;

    led = document.createElementNS(SVGNS, "rect");
    InGroup.appendChild(led);
    led.setAttribute("x", BayWidth - 6);
    led.id = "ACLED";
    led.setAttribute("y", 3);
    led.setAttribute("width", 4);
    led.setAttribute("height", 4);
    led.setAttribute("fill", "#DDD");
    led.setAttribute("stroke", "");

    led = document.createElementNS(SVGNS, "rect");
    InGroup.appendChild(led);
    led.setAttribute("x", BayWidth - 6);
    led.id = "Alarm";
    led.setAttribute("y", 9);
    led.setAttribute("width", 4);
    led.setAttribute("height", 4);
    led.setAttribute("fill", "#DDD");
    led.setAttribute("stroke", "");

    led = document.createElementNS(SVGNS, "rect");
    InGroup.appendChild(led);
    led.setAttribute("x", BayWidth - 6);
    led.id = "AlarmMinor";
    led.setAttribute("y", 14);
    led.setAttribute("width", 4);
    led.setAttribute("height", 4);
    led.setAttribute("fill", "#DDD");
    led.setAttribute("stroke", "");

    led = document.createElementNS(SVGNS, "rect");
    InGroup.appendChild(led);
    led.setAttribute("x", BayWidth - 6);
    led.id = "AlarmMajor";
    led.setAttribute("y", 20);
    led.setAttribute("width", 4);
    led.setAttribute("height", 4);
    led.setAttribute("fill", "#DDD");
    led.setAttribute("stroke", "");
}

/*****************************************************************************!
 * Function : RectifierSelectedDoubleClickCB
 *****************************************************************************/
function RectifierSelectedDoubleClickCB(InEvent) {
    var                         element = InEvent.srcElement;
    var                         p = element;

    while ( p && p.bay == undefined ) {
        p = p.parentElement;
    }
    RectifierEditInit(p);
}

/*****************************************************************************!
 * Function : RectifierSelectedCB
 *****************************************************************************/
function RectifierSelectedCB(InEvent) {
    var                         element = InEvent.srcElement;
    var                         p = element;

    DeselectRectifiers();
    DeselectDSMs();
    while ( p && p.bay == undefined ) {
        p = p.parentElement;
    }

    if ( p ) {
        p.children[0].children[1].setAttribute("fill", " #013c70");
        DeselectBays();
        p.bay.className = "Bay BaySize BaySelected";
    }
    if ( InEvent.ctrlKey == true ) {
        RectifierEditInit(p);
    }
}

/*****************************************************************************!
 * Function : HideRectifier
 *****************************************************************************/
function HideRectifier(InBay, InRectifierIndex) {
    var rectifier = BayFindRectifier(InBay, InRectifierIndex);
    if ( null == rectifier ) {
        return;
    }
    rectifier.children[1].style.visibility = "visible";
    rectifier.children[0].style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : RectifierEditInit
 *****************************************************************************/
function RectifierEditInit(InRectifier) {
  var update = {};
  update.command = "get";
  update.type = "rectifier";
  update.bayIndex = GetBayIndex(InRectifier.bay);
  update.unitNumber = InRectifier.rectifierIndex;

  SendGeneralRequest(update);
}
  
/*****************************************************************************!
 * Function : RectifierEdit
 *****************************************************************************/
function RectifierEdit(InRectifier) {
    editingRectifier = InRectifier;
    document.getElementById("RectifierActive").checked = InRectifier.power;
    document.getElementById("RectifierEditTitle").innerHTML = "Rectifier : " + InRectifier.bay.bayName + ", SLOT " + InRectifier.rectifierIndex;
    document.getElementById("ModeBlock").style.visibility = "visible";
    var                     editBox = document.getElementById("RectifierEdit");
    var serialNumberInput = document.getElementById("RectifierSerialNumber");
    var rinfo = InRectifier.rectifierInfo;
    serialNumberInput.value = rinfo.serialNumber;
    document.getElementById("RectifierRatedCurrentSelect").value = rinfo.ratedCurrent;
    document.getElementById("RectifierRatedOutputVoltage").value = rinfo.ratedOutputVoltage;
    document.getElementById("RectifierRatedInputVoltage").value = rinfo.ratedInputVoltage;
    document.getElementById("RectifierProductNumber").value = rinfo.productNumber;
    document.getElementById("RectifierCustomerText").value = rinfo.customerText;
    document.getElementById("RectifierOutputCurrent").value = rinfo.outputCurrent;
    document.getElementById("RectifierCANAddress").innerHTML = rinfo.CANAddress;
    
    slider = document.getElementById("RectifierOutputCurrentSlider");
    slider.value = rinfo.outputCurrent;;
    slider.max = rinfo.ratedCurrent * 3 / 2;
    
    document.getElementById("RectifierActive").checked = rinfo.power;
    editBox.style.visibility = "visible";
    n = rectifierMajorAlarmNames.length;
    for (i = 0; i < n; i++) {
        alarmName = rectifierMajorAlarmNames[i];
        elementName = "Rectifier" + alarmName + "Alarm";
        document.getElementById(elementName).checked = rinfo.alarms[alarmName];
    }
    n = rectifierMinorAlarmNames.length;
    for (i = 0; i < n; i++) {
        alarmName = rectifierMinorAlarmNames[i];
        elementName = "Rectifier" + alarmName + "Alarm";
        document.getElementById(elementName).checked = rinfo.alarms[alarmName];
    }
}

/*****************************************************************************!
 * Function : RectifierSendAdd
 *****************************************************************************/
function RectifierSendAdd(InRectifier) {
    var                         update = new Object();

    update.type = "Rectifier";
    update.command = "add";
    update.unitNumber = InRectifier.rectifierIndex;
    update.ratedOutputCurrent = InRectifier.rectifierInfo.ratedCurrent;
    update.ratedInputVoltage = InRectifier.rectifierInfo.ratedInputVoltage;
    update.ratedOutputVoltage = InRectifier.rectifierInfo.ratedOutputVoltage;
    update.serialNumber = InRectifier.rectifierInfo.serialNumber;
    update.productNumber = InRectifier.rectifierInfo.productNumber;
    update.bayIndex = GetBayIndex(InRectifier.bay);
    update.customerText = InRectifier.rectifierInfo.customerText;
    SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : RectifierSendUpdate
 *****************************************************************************/
function RectifierSendUpdate(InRectifier) {
    var update = new Object();

    update.type                 = "Rectifier";
    update.command              = "update";
    update.unitNumber           = InRectifier.rectifierIndex;
    update.bayIndex             = InRectifier.bay.bayIndex;
    update.ratedCurrent         = InRectifier.rectifierInfo.ratedCurrent;
    update.outputCurrent        = InRectifier.rectifierInfo.outputCurrent;
    update.outputVoltage        = InRectifier.rectifierInfo.outputVoltage;
    update.ratedInputVoltage    = InRectifier.rectifierInfo.ratedInputVoltage;
    update.ratedOutputVoltage   = InRectifier.rectifierInfo.ratedOutputVoltage;
    update.customerText         = InRectifier.rectifierInfo.customerText;
    update.productNumber 	= InRectifier.rectifierInfo.productNumber;
    update.serialNumber		= InRectifier.rectifierInfo.serialNumber;

    update.ACH = InRectifier.rectifierInfo.alarms.ACH;
    update.ACL = InRectifier.rectifierInfo.alarms.ACL;
    update.CBO = InRectifier.rectifierInfo.alarms.CBO;
    update.CNV = InRectifier.rectifierInfo.alarms.CNV;
    update.DCU = InRectifier.rectifierInfo.alarms.DCU;
    update.DTS = InRectifier.rectifierInfo.alarms.DTS;
    update.EMS = InRectifier.rectifierInfo.alarms.EMS;
    update.FMA = InRectifier.rectifierInfo.alarms.FMA;
    update.INH = InRectifier.rectifierInfo.alarms.INH;
    update.NPD = InRectifier.rectifierInfo.alarms.NPD;
    update.PHL = InRectifier.rectifierInfo.alarms.PHL;
    update.PFF = InRectifier.rectifierInfo.alarms.PFF;
    update.PHV = InRectifier.rectifierInfo.alarms.PHV;
    update.PTS = InRectifier.rectifierInfo.alarms.PTS;
    update.RHV = InRectifier.rectifierInfo.alarms.RHV;
    update.SWO = InRectifier.rectifierInfo.alarms.SWO;
    update.DNP = InRectifier.rectifierInfo.alarms.DNP;
    update.FMI = InRectifier.rectifierInfo.alarms.FMI;
    update.HAT = InRectifier.rectifierInfo.alarms.HAT;
    update.TCL = InRectifier.rectifierInfo.alarms.TCL;
    update.SNS = InRectifier.rectifierInfo.alarms.SNS;
    update.DHV = InRectifier.rectifierInfo.alarms.DHV;
    SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : RectifierEditCancelCB
 *****************************************************************************/
function RectifierEditCancelCB() {
    var                     editBox = document.getElementById("RectifierEdit");
    editBox.style.visibility = "hidden";
    document.getElementById("ModeBlock").style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : RectifierEditOKCB
 *****************************************************************************/
function RectifierEditOKCB() {
    var                     editBox = document.getElementById("RectifierEdit");
    editBox.style.visibility = "hidden";
    document.getElementById("ModeBlock").style.visibility = "hidden";
    editingRectifier.rectifierInfo.customerText = document.getElementById("RectifierCustomerText").value;
    editingRectifier.rectifierInfo.outputCurrent = parseInt(document.getElementById("RectifierOutputCurrent").value);
    editingRectifier.rectifierInfo.productNumber = document.getElementById("RectifierProductNumber").value;
    editingRectifier.rectifierInfo.serialNumber = document.getElementById("RectifierSerialNumber").value;
    editingRectifier.rectifierInfo.ratedCurrent = parseInt(document.getElementById("RectifierRatedCurrentSelect").value);
    editingRectifier.rectifierInfo.ratedInputVoltage = document.getElementById("RectifierRatedInputVoltage").value;
    editingRectifier.rectifierInfo.ratedOutputVoltage = document.getElementById("RectifierRatedOutputVoltage").value;
    editingRectifier.rectifierInfo.power = document.getElementById("RectifierActive").checked;

    RectifierSetMajorAlarm(editingRectifier, false);
    RectifierSetMinorAlarm(editingRectifier, false);
    RectifierSetAlarm(editingRectifier, false);
    n = rectifierMajorAlarmNames.length;
    for (i = 0; i < n ;i++) {
        alarmName = rectifierMajorAlarmNames[i];
        elementName = "Rectifier" + alarmName + "Alarm";
        editingRectifier.rectifierInfo.alarms[alarmName] = document.getElementById(elementName).checked;
        if ( editingRectifier.rectifierInfo.alarms[alarmName] ) {
            RectifierSetMajorAlarm(editingRectifier, true);
            RectifierSetAlarm(editingRectifier, true);
        }
    }

    n = rectifierMinorAlarmNames.length;
    for (i = 0; i < n ;i++) {
        alarmName = rectifierMinorAlarmNames[i];
        elementName = "Rectifier" + alarmName + "Alarm";
        editingRectifier.rectifierInfo.alarms[alarmName] = document.getElementById(elementName).checked;
        if ( editingRectifier.rectifierInfo[alarmName] ) {
            RectifierSetMinorAlarm(editingRectifier, editingRectifier.rectifierInfo.alarms[alarmName]);
            RectifierSetAlarm(editingRectifier, true);
        }
    }
    if ( editingRectifier.rectifierInfo.power ) {
        TurnOnRectifier(editingRectifier);
    } else {
        TurnOffRectifier(editingRectifier);
    }
    RectifierUpdate(editingRectifier);
    SetRectifierOutputCurrent(editingRectifier, editingRectifier.rectifierInfo.outputCurrent);
    RectifierSendUpdate(editingRectifier);
}

/*****************************************************************************!
 * Function : RectifierUpdateStatus
 *****************************************************************************/
function RectifierUpdateStatus(InRectifier, InPower, InCommStatus, InCurrent, InStartUpState) { 
    InRectifier.rectifierInfo.commStatus = InCommStatus;
    InRectifier.rectifierInfo.outputCurrent = InCurrent;
    InRectifier.rectifierInfo.power = InPower;
    InRectifier.rectifierInfo.startUpState = InStartUpState;
    if ( InStartUpState == 3 ) {
	InRectifier.children[0].children[1].setAttribute("fill", "#CCC");
	InRectifier.children[0].children[2].setAttribute("fill", "#CCC");
    } else {
	InRectifier.children[0].children[1].setAttribute("fill", "#000000C0");
	InRectifier.children[0].children[2].setAttribute("fill", "#000000C0");
    }
    if ( InRectifier.rectifierInfo.power ) {
	TurnOnRectifier(InRectifier);
    } else {
	TurnOffRectifier(InRectifier);
    }
    RectifierUpdate(InRectifier);
    SetRectifierOutputCurrent(InRectifier, InRectifier.rectifierInfo.outputCurrent);
    SetRectifierLED(InRectifier, "CommIndicator", InCommStatus ? "#00AA0040" : "#C66");
}

/*****************************************************************************!
 * Function : RectifierUpdate
 *****************************************************************************/
function
RectifierUpdate(InRectifier) {
    var                         update = new Object();

    update.type = "Rectifier";
    update.unitNumber = InRectifier.rectifierIndex;
}

/*****************************************************************************!
 * function : DeselectRectifiers
 *****************************************************************************/
function DeselectRectifiers() {
    var                         i, j;
    
    for ( i = 0; i < Bays.length; i++ ) {
        for (j = 0; j < Bays[i].rectifiers.length; j++ ) {
            ch = Bays[i].rectifiers[j].children[0].children;
            for (k = 0; k < ch.length; k++) {
                if ( ch[k].id == "Base" ) {
                    ch[k].setAttribute("fill", "#DDD");
                }
            }
        }
    }
}

/*****************************************************************************!
 * Function : RectifierSetMajorAlarm
 *****************************************************************************/
function RectifierSetMajorAlarm(InRectifier, InValue) {
    color = InValue ? "#A00000" : "#DDD";
    SetRectifierLED(InRectifier, "AlarmMajor", color);
    SetRectifierAlarmIndicator(InRectifier);
}

/*****************************************************************************!
 * Function : SetRectifierAlarmIndicator
 *****************************************************************************/
function SetRectifierAlarmIndicator(InRectifier) {
    elementName = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-Alarm";
    if ( InRectifier.rectifierInfo.power == false ) {
        fill = "#DDD";
    } else if ( InRectifier.rectifierInfo.AnyMajorAlarms() ) {
        fill = "#A00";
    } else if ( InRectifier.rectifierInfo.AnyMinorAlarms() ) {
        fill = "#f84";
    } else {
        fill = "#AFA";
    }
    document.getElementById(elementName).setAttribute("fill", fill);
}
    
/*****************************************************************************!
 * Function : RectifierSetMinorAlarm
 *****************************************************************************/
function RectifierSetMinorAlarm(InRectifier, InValue) {
    color = InValue ? "#A00000" : "#DDD";
    SetRectifierLED(InRectifier, "AlarmMinor", color);
    SetRectifierAlarmIndicator(InRectifier);
}

/*****************************************************************************!
 * Function : RectifierSetAlarm
 *****************************************************************************/
function RectifierSetAlarm(InRectifier, InValue) {
    color = InValue ? "#A00000" : "#DDD";
    SetRectifierLED(InRectifier, "Alarm", color);
}

/*****************************************************************************!
 * Function : TurnOnRectifier
 *****************************************************************************/
function
TurnOnRectifier(InRectifier) {
    SetRectifierLED(InRectifier, "ACLED", "#006F00");
    if ( InRectifier.rectifierInfo.AnyMajorAlarms() ) {
        SetRectifierLED(InRectifier, "AlarmMajor", "#A00");
        SetRectifierLED(InRectifier, "Alarm", "#A00");
    }
    if ( InRectifier.rectifierInfo.AnyMinorAlarms() ) {
        SetRectifierLED(InRectifier, "AlarmMinor", "#A00");
        SetRectifierLED(InRectifier, "Alarm", "#A00");
    }
    elementName = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-PowerSwitch";
    element = document.getElementById(elementName);
    if ( element ) {
	element.setAttribute("fill", "#0F0");
    } else {
	console.log("Could not find " + elementName);
    }
    
    SetRectifierOutputCurrent(InRectifier, InRectifier.rectifierInfo.outputCurrent);
    SetRectifierAlarmIndicator(InRectifier);
    RectifierSendAdd(InRectifier);
    //RectifierSendUpdate(InRectifier);
    SetRectifierPower(InRectifier);
}

/*****************************************************************************!
 * Function : TurnOffRectifier
 *****************************************************************************/
function
TurnOffRectifier(InRectifier) {
    SetRectifierLED(InRectifier, "ACLED", "#DDD");
    SetRectifierLED(InRectifier, "AlarmMajor", "#DDD");
    SetRectifierLED(InRectifier, "AlarmMinor", "#DDD");
    SetRectifierLED(InRectifier, "Alarm", "#DDD");
    elementName = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-PowerSwitch";
    element = document.getElementById(elementName);
    element.setAttribute("fill", "#666");
    SetRectifierOutputCurrent(InRectifier, InRectifier.rectifierInfo.outputCurrent);
    SetRectifierAlarmIndicator(InRectifier);
    SetRectifierPower(InRectifier);
}

/*****************************************************************************!
 * Function : SetRectifierLED
 *****************************************************************************/
function SetRectifierLED(InRectifier, InLEDName, InColor) {
    var                         ch;
    var                         i, n;

    ch = InRectifier.children[0].children;
    n = ch.length;

    for (i = 0; i < n; i++) {
        element = ch[i];
        if ( element.id == InLEDName ) {
            element.setAttribute("fill", InColor);
        }
    }
}
    
/*****************************************************************************!
 * Function : SetRectifierOutputCurrent
 *****************************************************************************/
function SetRectifierOutputCurrent(InRectifier, InOutputCurrent) {
    var elementName;
    var guage, realCurrent, current, fill, height, y;
    
    elementName = "Bay-" + InRectifier.bay.bayName + "-Rectifier-" + InRectifier.rectifierIndex + "-PowerGuage";
    guage = document.getElementById(elementName);

    if ( InRectifier.rectifierInfo.power ) {
        realCurrent = current = parseInt(InOutputCurrent);
        ratedCurrent = parseInt(InRectifier.rectifierInfo.ratedCurrent);
        if ( current > ratedCurrent ) {
            current = ratedCurrent;
        }
        height = (20 * current) / ratedCurrent;
        if ( current > 0 ) {
            if ( realCurrent > ratedCurrent ) {
                fill = "#C00";
            } else {
                fill = "#0C0";
            }
        } else {
            fill = "#888";
        }
        y = 23 - height;
    } else {
        fill = "#888";
        height = 0;
        y = 23;
    }
    guage.setAttribute("fill", fill);
    guage.setAttribute("height", height);
    guage.setAttribute("y", y);
}

/*****************************************************************************!
 * Function : RectifierOutputCurrentSliderCB
 *****************************************************************************/
function RectifierOutputCurrentSliderCB(InEvent) {
    var element = document.getElementById("RectifierOutputCurrentSlider");
    document.getElementById("RectifierOutputCurrent").value = element.value;
}

/*****************************************************************************!
 * Function : RectifierRatedCurrentSelectCB
 *****************************************************************************/
function RectifierRatedCurrentSelectCB()
{
    var element = document.getElementById("RectifierRatedCurrentSelect");
    var element2 = document.getElementById("RectifierOutputCurrentSlider");
    value = element.value;
    element2.max = value * 3 / 2;
}    
   
