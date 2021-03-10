/*****************************************************************************
 * FILE         : DSM.js
 * DATE         : January 2 2019
 * PROJECT      : 
 * COPYRIGHT    : Copyright (c) 2019 Vertiv Company
 *****************************************************************************/

/*global SVGNS*/
/*global CreateHorizontalContact*/
/*global CreateVerticalContact*/
/*global nextSerialNumber*/
/*global Bays*/
/*global GetBayIndex*/
/*global SendGeneralRequest*/
/*global nextSerialNumber*/
/*global selectedContact*/

/*****************************************************************************!
 * Local Vars
 *****************************************************************************/
var DSMColor                = "#016c40";
var DSMUnselectedBackground = "linear-gradient(135deg, #016c40, #08995e)";
var DSMSelectedBackground   = "linear-gradient(225deg, #013c70, #08998e)";
var DSMProductNumber = "";

var DSMHorizontalContactPositions = [
  { x:10, y:40 },
  { x:20, y:40 },
  { x:30, y:40 },
  { x:40, y:40 },
  { x:50, y:40 },
  { x:60, y:40 },
  { x:75, y:40 },
  { x:85, y:40 },
  { x:95, y:40 },
  { x:105, y:40 },
  { x:115, y:40 },
  { x:125, y:40 }
];

var DSMVerticalContactPositions = [
  { x:10, y:30 },
  { x:10, y:40 },
  { x:10, y:50 },
  { x:10, y:60 },
  { x:10, y:70 },
  { x:10, y:80 },

  { x:10, y:100 },
  { x:10, y:110 },
  { x:10, y:120 },
  { x:10, y:130 },
  { x:10, y:140 },
  { x:10, y:150 },

  { x:40, y:30 },
  { x:40, y:40 },
  { x:40, y:50 },
  { x:40, y:60 },
  { x:40, y:70 },
  { x:40, y:80 }
];

/*****************************************************************************!
 * Function : CreateDSM
 *****************************************************************************/
function CreateDSM(InIndex, InDistOnly)
{
  var                         dsm = document.createElementNS(SVGNS, "svg");
    var                         i;
    dsm.id = "DSM " + InIndex;
  dsm.contacts = [];
  dsm.info = new DSMInfo();
	if ( InDistOnly ) {
		dsm.info.maxPointsPerBus = 12;
		dsm.info.productNumber = DSMDistOnlyProductNumber;
	} else {
		dsm.info.maxPointsPerBus = 24;
		dsm.info.productNumber = DSMProductNumber;
	}
  dsm.style.position = "absolute";
  dsm.style.left = "0";
  dsm.style.width = "142";
  dsm.style.height = "102";
  dsm.style.top = "30";
  dsm.style.background = DSMUnselectedBackground;
  dsm.style.border = "solid 1px black";
  dsm.elementType = "DSM";
  dsm.selected = false;
  for (i = 0 ; i < DSMHorizontalContactPositions.length; i++) {
    CreateHorizontalContact(dsm, i);
  }
  DSMInitialize(dsm, InIndex);
  return dsm
}

/*****************************************************************************!
 * Type : DSMInfo
 *****************************************************************************/
function DSMInfo()
{
  var i;
  this.serialNumber = nextSerialNumber++;
  this.productNumber = DSMProductNumber;
  this.maxPointsPerBus = 12;

  this.TDB = false;   // Two Distribution bus
  this.OSP = true;    // One Shunt Per Bus
  this.G50 = false;   // Customer shunts are 50mV
  this.TBD = true;    // 2 Byte Data
  this.GS2 = false;   // Shunt 2 gain customer specified
  this.GS1 = false;   // Shunt 1 gain customer specified
  this.TOP = true;    // Top Distribution;
  this.BLI = false;   // Battery LVD Installed;
  this.LLI = false;   // Load Share Imbalance;

  this.bus1mv = 0;
  this.bus2mv = 0;

  this.NRY = false;   // Not reporting yet
  this.IOC = false;   // Installed options changed
  this.DNP = false;   // Duplicate node present
  this.SOC = false;   // Shunt over current
  this.UFB = false;   // Unknown Fuse/Breaker
  this.FBA = false;   // Fuse/Breaker Alarm
  this.DCF = false;   // Disconnected controller fail
  this.LVD = false;   // Low Voltage Disconnect
  this.ABF = false;   // A/B Power Feed
  this.SHR = false;   // Shunt removed
  this.ABC = false;   // A/B Jumper change
  this.NDR = false;   // No Data Received
  this.CTL = false;   // Controller fail

  this.BH4 = false;
  this.BH3 = false;
  this.BH2 = false;
  this.BH1 = false;
  this.BT4 = false;
  this.BT3 = false;
  this.BT2 = false;
  this.BT1 = false;
  this.S2B = false;
  this.S2L = false;
  this.measuredCurrent = [];
  for ( i = 0; i < this.maxPointsPerBus; i++) {
    this.measuredCurrent[i] = 0;
  }
  this.shuntBits = [];
  for (i = 0; i < 24; i++) {
    this.shuntBits[i] = 0;
  }

  this.fuseBreakerInstallStatus = [];
  for (i = 0; i < 24; i++) {
    this.fuseBreakerInstallStatus[i] = 0;
  }
}

/*****************************************************************************!
 * Function : DSMSendAdd
 *****************************************************************************/
function DSMSendAdd(InDSM) {
  var update              = new Object();
  var dsmIndex            = InDSM.dsmIndex;
  
  update.type = "DSM";
  update.command = "add";
  update.maxPointsPerBus = InDSM.info.maxPointsPerBus.toString();
  update.serialNumber = InDSM.info.serialNumber.toString();
  update.productNumber = InDSM.info.productNumber.toString();
  update.unitNumber = dsmIndex.toString();
  update.bayIndex = GetBayIndex(InDSM.bay).toString();
  console.log(update);
  SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : DSMSendUpdate
 *****************************************************************************/
function DSMSendUpdate(InDSM) {
  var                         update = new Object();
  var                         contact, i, s, rating;
  
  update.type = "DSM";
  update.command = "update";
  update.maxPointsPerBus = InDSM.info.maxPointsPerBus;
  update.serialNumber = InDSM.info.serialNumber;
  update.bayIndex = GetBayIndex(InDSM.bay);
  update.productNumber = InDSM.info.productNumber;
  update.unitNumber = InDSM.dsmIndex;
  update.contacts = [];
  for ( i = 0; i < InDSM.contacts.length; i++) {
    contact = new Object();
    contact.current = parseInt(InDSM.contacts[i].current, 10);
    contact.count = InDSM.contacts[i].breakerCount == undefined ? 0  : InDSM.contacts[i].breakerCount;
    contact.state = InDSM.contacts[i].contactState;
    contact.alarm = InDSM.contacts[i].alarm;
    rating = InDSM.contacts[i].breakerRating;
    if ( rating ) {
      s = rating.split(' ');
      contact.amps = parseInt(s[0], 10);
      contact.type = s[2];
    } else {
      contact.amps = 0;
      contact.type = "";
    }
    update.contacts.push(contact);
  }
  SendGeneralRequest(update);
  console.log(update);
}

/*****************************************************************************!
 * Function : CreateVerticalDSM
 *****************************************************************************/
function CreateVerticalDSM(InIndex) {
  var                         dsm = document.createElement("svg");
  var i;
  dsm.className = "DSMVertical";
  dsm.elementType = "DSM";
  dsm.info = new DSMInfo();
  dsm.contacts = [];

  for (i = 0 ; i < DSMHorizontalContactPositions.length; i++) {
    CreateVerticalContact(dsm, i);
  }

  DSMInitialize(dsm, InIndex);
  return dsm;
}

/*****************************************************************************!
 * Function : DSMInitialize
 *****************************************************************************/
function DSMInitialize(InDSM, InIndex) {
  InDSM.onclick = function(event) { DSMSelectedCB(event); };
  InDSM.ondblclick = function(event) { DSMSelectedDoubleClickCB(event); };
  InDSM.dsmIndex = InIndex;
  InDSM.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : DSMSelectedCB
 *****************************************************************************/
function DSMSelectedCB(InEvent) {
  var element;
  DeselectRectifiers();

  element = InEvent.srcElement;
  if ( element.elementType == "contact" ) {
    element = element.parentElement;
  }
  if ( element.selected ) {
    return;
  }
  DeselectDSMs();
  SelectDSM(element);
}

/*****************************************************************************!
 * Function : SelectDSM
 *****************************************************************************/
function SelectDSM(InDSM) {
  InDSM.selected = true;
  InDSM.style.background = DSMSelectedBackground;
}

/*****************************************************************************!
 * Function : DSMSelectedDoubleClickCB
 *****************************************************************************/
function DSMSelectedDoubleClickCB(InEvent) {
  DSMEdit(InEvent.srcElement);
}

/*****************************************************************************!
 * Function : DSMEdit
 *****************************************************************************/
function DSMEdit(InDSM)
{
  var                         editBox = document.getElementById("DSMEdit");
  
  document.getElementById("DSMEditTitle").innerHTML = "DSM : " + InDSM.bay.bayName + ", MODULE " + InDSM.dsmIndex;
  document.getElementById("ModeBlock").style.visibility = "visible";

  document.getElementById("DSMSerialNumber").value = InDSM.info.serialNumber;
  document.getElementById("DSMProductNumber").value = InDSM.info.productNumber;
  editBox.style.visibility = "visible";
	occupiedCount = 0;
	
  for (i = 0; i < InDSM.contacts.length; i++) {
		contact = InDSM.contacts[i];
		contactIndex = i + 1;
		contactValueName = "DSMEditContactValue" + contactIndex;
		contactName = "DSMEditContact" + contactIndex;
		contactLabel = document.getElementById(contactName);
		contactValue = document.getElementById(contactValueName);
		if ( contact.occupied ) {
			contactLabel.style.color = "#000";
			contactLabel.style.borderTop = "1px solid black";
			contactLabel.style.borderLeft = "1px solid black";
			contactLabel.style.borderRight = "1px solid black";
			contactLabel.style.background = "#884422";
			contactValue.style.visibility = "visible";
			contactLabel.style.height = "21px";
			contactValue.innerHTML = contact.current;
			occupiedCount = contact.breakerFuseWidth - 1;
			if ( occupiedCount == 0 ) {
				contactLabel.style.height = "19px";
				contactLabel.style.borderBottom = "1px solid black";
			}
		} else {
			if ( occupiedCount > 0 ) {
				contactLabel.style.color = "#000";
				contactLabel.style.borderLeft = "1px solid black";
				contactLabel.style.borderRight = "1px solid black";
				contactLabel.style.background = "#884422";
				occupiedCount--;
				if ( occupiedCount == 0 ) {
					contactLabel.style.borderBottom = "1px solid black";
				}
			} else {
				contactLabel.style.border = "none";
				contactLabel.style.color = "#00000040";
				contactLabel.style.background = "#00000000";
				contactLabel.style.height = "20px";
			}
			contactValue.style.visibility = "hidden";
		}
		console.log(contactIndex, contact.occupied, contact.breakerFuseWidth, occupiedCount);
  }
}

/******************************************************************************!
 * Function : DSMEditCancelCB
 ******************************************************************************/
function DSMEditCancelCB() {
  var                     editBox = document.getElementById("DSMEdit");
  editBox.style.visibility = "hidden";
  document.getElementById("ModeBlock").style.visibility = "hidden";
  for (i = 1; i <= dsm.contacts.length; i++) {
		name2 = "DSMEditContactValue" + i;
		document.getElementById(name2).style.visibility = "hidden";
	}
}

/*****************************************************************************!
 * Function : DSMEditDismissCB
 *****************************************************************************/
function DSMEditDismissCB()
{
  var                     editBox = document.getElementById("DSMEdit");
  var                     dsm = FindSelectedDSM();
  for (i = 1; i <= dsm.contacts.length; i++) {
		name2 = "DSMEditContactValue" + i;
		document.getElementById(name2).style.visibility = "hidden";
	}
  document.getElementById("ModeBlock").style.visibility = "hidden";
  editBox.style.visibility = "hidden";
  if ( dsm == null ) {
    return;
  }
  dsm.info.serialNumber = document.getElementById("DSMSerialNumber").value;
  dsm.info.productNumber = document.getElementById("DSMProductNumber").value;
}

/*****************************************************************************!
 * Function : RemoveDSMCB
 *****************************************************************************/
function RemoveDSMCB()
{
  var dsm = FindSelectedDSM();
  var bay;
  var update = {};
  var dsmIndex;
  if ( null == dsm ) {
    return;
  }

  bay = dsm.bay;
  if ( null == bay ) {
    return;
  }

  dsmIndex = dsm.dsmIndex;
  update.type  = "DSM";
  update.command = "remove";
  update.bayIndex = GetBayIndex(dsm.bay).toString();
  update.unitNumber = dsmIndex.toString();
  SendGeneralRequest(update);
  console.log(update);
  dsm.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : AddDSMCB
 *****************************************************************************/
function AddDSMCB() {
  var                         bay = FindSelectedBay();
  var                         maxDSMs;
  var                         i;
  
  if ( null == bay ) {
    return;
  }

  maxDSMs = bay.dsms.length;

  for (i = 0; i < maxDSMs; i++) {
    if ( bay.dsms[i].style.visibility == "hidden" ) {
      bay.dsms[i].style.visibility = "visible";
      DSMSendAdd(bay.dsms[i]);
      break;
    }
  }
}

/*****************************************************************************!
 * Function : DeselectDSMs
 *****************************************************************************/
function DeselectDSMs() {
  var i, k, bay;

  for (i = 0 ; i < Bays.length ; i++ ) {
    bay = Bays[i];
    for (k = 0 ; k < bay.dsms.length ; k++) {
      bay.dsms[k].selected = false;
      bay.dsms[k].style.background = DSMUnselectedBackground;
    }
  }
}

/*****************************************************************************!
 * Function : DeselectOtherDSMs
 *****************************************************************************/
function DeselectOtherDSMs(InDSM) {
  var i, k, bay;

  for (i = 0 ; i < Bays.length ; i++ ) {
    bay = Bays[i];
    for (k = 0 ; k < bay.dsms.length ; k++) {
      if ( bay.dsms[k] != InDSM ) {
        bay.dsms[k].selected = false;
        bay.dsms[k].style.background = DSMUnselectedBackground;
      }
    }
  }
}

/*****************************************************************************!
 * Function : DSMContactSelectCB
 *****************************************************************************/
function DSMContactSelectCB(InEvent) {
  DeselectDSMs();
  DeselectRectifiers();
}

/*****************************************************************************!
 * Function : FindSelectedDSM
 *****************************************************************************/
function FindSelectedDSM() {
  var i, bay, i2, dsm;
  for (i = 0; i < Bays.length; i++) {
    bay = Bays[i];
    for (i2 = 0; i2 < bay.dsms.length; i2++) {
      dsm = bay.dsms[i2];
      if ( dsm.selected ) {
        return dsm;
      }
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : DSMGetContactIndex
 *****************************************************************************/
function DSMGetContactIndex(InDSM, InContact) {
  var i;
  if ( null == InDSM || null == InContact ) {
    return;
  }

  for (i = 0; i < InDSM.contacts.length; i++) {
    if ( InDSM.contacts[i] == InContact ) {
      return i;
    }
  }
  return -1;
}

/*****************************************************************************!
 * Function : ShuntCurrentSliderCB
 *****************************************************************************/
function ShuntCurrentSliderCB(InIndex) {
  var slider = document.getElementById("ShuntCurrentSlider" + InIndex);
  var currentInput = document.getElementById("ShuntCurrent" + InIndex);
  currentInput.value = slider.value;
}

/*****************************************************************************!
 * Function : ShuntEditOKCB
 *****************************************************************************/
function ShuntEditOKCB() {
  var s, shuntIndex, dsm, current, element, update;
  var editor = document.getElementById("ShuntEdit");
  editor.style.visibility = "hidden";
  dsm = selectedContact.dsm;
  s = document.getElementById("ShuntSection1");
  if ( s.style.visibility == "visible" ) {
    shuntIndex = parseInt(document.getElementById("ShuntNumber1").innerHTML, 10) - 1;
    console.log(shuntIndex);
    element = document.getElementById("ShuntCurrent1");
    current = parseInt(element.value, 10);
    dsm.contacts[shuntIndex].current = current;
  }
  s.style.visibility = "hidden";
  var dsmIndex = dsm.dsmIndex;
  shuntIndex++;
  update = {
    "type" : "shunt",
    "command" : "update",
    "unitNumber" : dsmIndex.toString(),
    "bayIndex" : GetBayIndex(dsm.bay).toString(),
    "shuntIndex" : shuntIndex.toString(),
    "current" : current.toString()
  };
  SendGeneralRequest(update);
  console.log(update);
}

/*****************************************************************************!
 * Function : ShuntEditCancelCB
 *****************************************************************************/
function ShuntEditCancelCB() {
  var s, m;
  ShuntEditHide();
}

/******************************************************************************!
 * Function : ShuntEditHide
 ******************************************************************************/
function ShuntEditHide() {
  var s;
  var editor = document.getElementById("ShuntEdit");
  editor.style.visibility = "hidden";
  s = document.getElementById("ShuntSection1");
  s.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : RemoveDSM
 *****************************************************************************/
function RemoveDSM() {
  var                     dsm = FindSelectedDSM();
  var editor = document.getElementById("ShuntEdit");
  editor.style.visibility = "hidden";
}

/******************************************************************************!
 * Function : ShuntEditRemoveCB
 ******************************************************************************/
function ShuntEditRemoveCB() {
  var dsm, shuntIndex, update;
  ShuntEditHide();
  dsm = selectedContact.dsm;
  shuntIndex= selectedContact.contactIndex;
  DSMRemoveBreaker(dsm, shuntIndex);
  var dsmIndex = dsm.dsmIndex;
  var shuntIndex = shuntIndex + 1;
  update = {
    "type" : "shunt",
    "command" : "remove",
    "unitNumber" : dsmIndex.toString(),
    "bayIndex" : GetBayIndex(dsm.bay).toString(),
    "shuntIndex" : shuntIndex.toString()
  }
  SendGeneralRequest(update);
  console.log(update);
}

/******************************************************************************!
 * Function : DSMRemoveBreaker
 ******************************************************************************/
function DSMRemoveBreaker(InDSM, InIndex) {
  var breakerParent, contact, breaker, n, i;
  
  contact = InDSM.contacts[InIndex];
  breaker = contact.breakerFuse;
  contact.breakerFuse = null;
  n = contact.breakerCount;
  contact.breakerCount = 0 ;
  contact.breakerRating = null;
  breakerParent = breaker.parentElement;
  breakerParent.removeChild(breaker);
  for (i = InIndex; i < InIndex + n; i++) {
    InDSM.contacts[i].contactState = "Open";
    InDSM.contacts[i].occupied = false;
  }
}

/*****************************************************************************!
 * Function : DSMFindContact
 *****************************************************************************/
function DSMFindContact(InDSM, InContactIndex) {
  var k;
  for (k = 0; k < InDSM.contacts.length; k++) {
    if ( InDSM.contacts[k].contactIndex == InContactIndex ) {
      return InDSM.contacts[k];
    }
  }
  return null;
}
