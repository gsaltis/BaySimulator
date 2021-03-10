/*****************************************************************************
 * FILE         : Bay.js
 * DATE         : December 31 2018
 * PROJECT      : 
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Function : CreateBay
 *****************************************************************************/
function CreateBay(InBayName, InBayType, InBayIndex)
{
  var             n = Bays.length;
  var             x = (n * ( BayWidth + BayXGap) + BayXGap);
  var             bay, dsm, t;
  var             mainWindow = document.getElementById("MainWindow");
  var				editAllButton;
  var maxRectifierCount;
  var maxDSMCount;
  bay = document.createElement("div");
  bay.id = InBayName;
  bay.bayType = InBayType;
  bay.elementType = "Bay";
  bay.className = "Bay BaySize BayUnSelected";
  bay.style.top = BayTop;
  bay.style.left = x + "px";
  bay.rectifiers = [];
  bay.dsms = [];
  bay.bayName = InBayName;
  bay.bayIndex = InBayIndex;
  bay.selected = false;
  CreateBayNameDisplay(bay);

  maxRectifierCount = 10;
  maxDSMCount = 2;
  if ( InBayType == "Distribution" ) {
		maxRectifierCount = 0;
		maxDSMCount = 4;
  }
  editAllButton = document.createElement("div");
  editAllButtonDraw = document.createElementNS(SVGNS, "svg");
  editAllButton.className = "BayEditAllButton";

  r = document.createElementNS(SVGNS, "path");
  r.setAttribute("d", "m0, 0  15, 9  -15,9  0 0");
  r.setAttribute("fill", "#00880080");
  r.style.cursor = "hand";
  r.onclick = function(InEvent) { BayEditCB(InEvent); }
  editAllButtonDraw.appendChild(r);
  editAllButton.appendChild(editAllButtonDraw);
  bay.appendChild(editAllButton);
  bay.onmousedown = function() { BaySelectCB(event); }
  mainWindow.appendChild(bay);
  for ( i = 0; i < maxRectifierCount; i++ ) {
    CreateRectifier(bay, i + 1);
    HideRectifier(bay, i+1);
  }
  t = 30;
  for (i = 0; i < maxDSMCount; i++) {
		dsm = CreateDSM(i+1, bay.bayType == "Distribution");
		bay.dsms.push(dsm);
		bay.appendChild(dsm);
		dsm.bay = bay;
		dsm.style.top = t + "px";
		t += 110;
  }
  return bay;
}

/*****************************************************************************!
 * Function : BayEditCB
 *****************************************************************************/
function BayEditCB(InEvent)
{
  var editBox;
  var bay = InEvent.srcElement.parentElement.parentElement.parentElement;
  editBox = document.getElementById("BayEditBox");
  document.getElementById("BayEditBoxBayType").innerHTML = bay.bayType;
  var state = editBox.style.visibility;

  if  ( state == "visible" ) {
		editBox.style.visibility = "hidden";
		bay.style.width = "144px";
		InEvent.srcElement.setAttribute("d", "m0, 0  15, 9  -15,9  0 0");
  } else {
		editBox.style.visibility = "visible";
		bay.style.width = "368px";
		InEvent.srcElement.setAttribute("d", "m0, 9  15, -9  0,18  0 0");
  }
  document.getElementById("BayEditBoxRectCurrent").value = 0;
  document.getElementById("BayEditBoxDSMCurrent").value = 0;
  document.getElementById("BayEditBoxRectPower").checked = false;
}

/*****************************************************************************!
 * Function : BayEditBoxCloseCB
 *****************************************************************************/
function BayEditBoxCloseCB(InEventCB)
{
  var editBox;
  editBox = document.getElementById("BayEditBox");
  editBox.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : CreateBayNameDisplay
 *****************************************************************************/
function CreateBayNameDisplay(InBay) {
  var                     bayNameDiv;

  bayNameDiv = document.createElement("div");
  bayNameDiv.className = "BayName";
  InBay.bayNameElement = bayNameDiv;
  InBay.appendChild(bayNameDiv);
  bayNameDiv.innerHTML = InBay.bayName;
}

/*****************************************************************************!
 * Function : CreateDistributionBay
 *****************************************************************************/
function CreateDistributionBay(InBayName, InBayIndex)
{
  var             n = Bays.length;
  var             x = (n * ( BayWidth + BayXGap) + BayXGap);
  var             bay;
  var             mainWindow = document.getElementById("MainWindow");

  bay = document.createElement("div");
  bay.bayType = "Distribution";
  bay.className = "Bay BaySize BayUnSelected";
  bay.elementType = "Bay";
  bay.style.top = BayTop;
  bay.bayIndex = InBayIndex;
  bay.style.left = x + "px";
  bay.rectifiers = [];
  bay.dsms = [];
  bay.bayName = InBayName;
  CreateBayNameDisplay(bay);

  bay.onmousedown = function() { BaySelectCB(event); }
  mainWindow.appendChild(bay);
  Bays.push(bay);

  dsm = CreateVerticalDSM(1);
  bay.dsms.push(dsm);
  dsm.bay = bay;
  bay.appendChild(dsm);

  dsm = CreateVerticalDSM(2);
  bay.dsms.push(dsm);
  dsm.style.left = "72";
  dsm.bay = bay;
  bay.appendChild(dsm);
  
  dsm = CreateVerticalDSM(3);
  bay.dsms.push(dsm);
  dsm.style.top = "254";
  dsm.bay = bay;
  bay.appendChild(dsm);
  
  dsm = CreateVerticalDSM(4);
  bay.dsms.push(dsm);
  dsm.style.left = "72";
  dsm.style.top = "254";
  dsm.bay = bay;
  bay.appendChild(dsm);

  return bay;
}

/*****************************************************************************!
 * Function : SelectBay
 *****************************************************************************/
function SelectBay(InBay) {

  if ( InBay.selected ) {
    return;
  }
  DeselectRectifiers();
  DeselectDSMs();
  DeselectBays();
  DeselectContacts();
  InBay.className = "Bay BaySize BaySelected";
}

/*****************************************************************************!
 * Function : DeselectBays
 *****************************************************************************/
function DeselectBays() {
  var                         i, n, j, m, bay, c, cs;
  
  n = Bays.length;
  for (i = 0; i < n; i++) {
    bay = Bays[i];
    bay.className = "Bay BaySize BayUnSelected";
  }
}

/*****************************************************************************!
 * Function : FindSelectedBay
 *****************************************************************************/
function FindSelectedBay() {
  var n, i, bay, classParts, j, k;
  
  n = Bays.length;
  for (i = 0; i < n; i++) {
    bay = Bays[i];
    classParts = bay.className.split(' ');
    j = classParts.length;
    for (k = 0; k < j; k++) {
      if ( classParts[k] == "BaySelected" ) {
        return bay;
      }
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : BaySelectCB
 *****************************************************************************/
function BaySelectCB(InEvent) {

  var element = InEvent.srcElement;

  while (element && element.elementType == null) {
    element = element.parentElement;
  }
  if ( null == element ) {
    return;
  }

  if ( element.elementType != "Rectifier" && element.elementType != "Bay" && element.elementType != "DSM" ) {
    element = element.parentElement;
  }
  
  if ( element.elementType == "Rectifier" || element.elementType == "DSM") {
    element = element.parentElement;
  }

  SelectBay(element);
}

/*****************************************************************************!
 * Function : AddBayCB
 *****************************************************************************/
function AddBayCB()
{

  var bay = FindBayByName("Main Bay");
  if ( bay != null ) {
    HideMainBayMenuSelection();
  }
  var id = document.getElementById("SubMenuAddBay");
  id.style.visibility = "visible";
}

/*****************************************************************************!
 * Function : FindBayByName
 *****************************************************************************/
function FindBayByName(InName) {
  var i;
  for ( i = 0; i < Bays.length; i++ ) {
    if ( Bays[i].bayName == InName ) {
      return Bays[i];
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : HideMainBayMenuSelection
 *****************************************************************************/
function HideMainBayMenuSelection()
{
  var element = document.getElementById("SubMenuAddBay");
  element.children[0].style.visibility = "hidden";
  element.children[1].style.top = "4px";
  element.children[2].style.top = "24px";
  element.style.height = "44px";
}

/*****************************************************************************!
 * Function : AddSupplementalBay
 *****************************************************************************/
function AddSupplementalBay() {
  if ( Bays.length >= 8 ) {
    return;
  }
  var bayIndex = Bays.length + 1;
  var bayName = "Bay " + bayIndex;
  var bay = CreateBay(bayName, "Supplemental", bayIndex);
  DisplayNewBay(bay, true);
}

/******************************************************************************!
 * Function : DisplayNewBay
 ******************************************************************************/
function DisplayNewBay(InBay, InSendBayAdd) {
  Bays.push(InBay);
  ReArrangeBays();
  ReNumberBays();
  SelectBay(InBay);
  SubMenuAddBayHideCB();
  if ( InSendBayAdd ) {
    BaySendAdd(InBay)
  }
}

/*****************************************************************************!
 * Function : AddBay
 *****************************************************************************/
function AddBay(InType) {
	var hostAddress = document.location.hostname;
	var bayIndex = hostAddress.substring(hostAddress.length-1, hostAddress.length);
  var bayName = "Bay " + bayIndex;
  bay = CreateBay(bayName, InType, 1);
	document.getElementById("BayMenuSection").style.visibility = "hidden";
  DisplayNewBay(bay, true);
	AddNormalMessage(bayName + " created");
}

/*****************************************************************************!
 * Function : AddMainBay
 *****************************************************************************/
function AddMainBay() {
	AddBay("Supplemental");
}

/*****************************************************************************!
 * Function : AddDistibutionBay
 *****************************************************************************/
function AddDistributionBay() {
	AddBay("Distribution");
}

/*****************************************************************************!
 * Function : BaySendAdd
 *****************************************************************************/
function BaySendAdd(InBay) {
  var                 update;
  if ( InBay == null ) {
    return;
  }
  update = new Object();
  update.command = "add";
  update.type = "Bay";
  update.name = InBay.bayName;
  update.index = GetBayIndex(InBay);
  update.bayType = InBay.bayType;
  SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : RemoveBayCB
 *****************************************************************************/
function RemoveBayCB() {
  var bay = FindSelectedBay();

  if ( null == bay ) {
    return;
  }
  var bayIndex = GetBayIndex(bay);
  if ( bayIndex == -1 ) {
    return;
  }

  bay.remove();
  Bays.splice(bayIndex, 1);

  ReArrangeBays();
  ReNumberBays();
}

/*****************************************************************************!
 * Function : ReArrangeBays
 *****************************************************************************/
function ReArrangeBays() {
  var x, w, i;
  
  x = BayXGap;
  w = BayWidth;
  for (i = 0 ; i < Bays.length; i++) {
    Bays[i].style.left = x;
    x += w + BayXGap;
  }
}

/*****************************************************************************!
 * Function : ReNumberBays
 *****************************************************************************/
function ReNumberBays() {
}

/*****************************************************************************!
 * Function : GetBayIndex
 *****************************************************************************/
function GetBayIndex(InBay) {
  return InBay.bayIndex;
}

/*****************************************************************************!
 * Function : BayFindRectifier
 *****************************************************************************/
function BayFindRectifier(InBay, InUnitNumber) {
  if ( InBay == null ) {
    return null;
  }
  var rectifier = null;
  InBay.rectifiers.forEach(function(InValue, InIndex, InArray) {
    if ( InValue.rectifierIndex == InUnitNumber ) {
      rectifier = InValue;
    }
  });
  return rectifier;
}
/*****************************************************************************!
 * Function : BayFindDSM
 *****************************************************************************/
function BayFindDSM(InBay, InUnitNumber) {
  if ( InBay == null ) {
    return null;
  }
  var dsm = null;
  InBay.dsms.forEach(function(InValue, InIndex, InArray) {
    if ( InValue.dsmIndex == InUnitNumber ) {
      dsm = InValue;
    }
  });
  return dsm;
}

/*****************************************************************************!
 * Function : BayFindRectifierByIndex
 *****************************************************************************/
function BayFindRectifierByIndex(InBay, InIndex) {
  var i;
  for (i = 0; i < InBay.rectifiers.length; i++) {
    if ( InBay.rectifiers[i].rectifierIndex == InIndex ) {
      return InBay.rectifiers[i];
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : BayFindByIndex
 *****************************************************************************/
function BayFindByIndex(InIndex) {
  var i;
  
  for (i = 0; i < Bays.length; i++ ) {
    if ( Bays[i].bayIndex == InIndex ) {
      return Bays[i];
    }
  }
  return null;
}

