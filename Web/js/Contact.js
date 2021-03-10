/*****************************************************************************
 * FILE         : Contact.js
 * DATE         : February 1 2019
 * PROJECT      : Bay Simulator
 * COPYRIGHT    : Copyright (c) 2019 Vertiv Company
 *****************************************************************************/

/*global SVGNS*/
/*global Bays*/
/*global FindChildElementById*/
/*global FindChildElementsById*/
/*global DeselectOtherDSMs*/
/*global SelectDSM*/
/*global DSMHorizontalContactPositions*/
/*global DSMVerticalContactPositions*/
/*global DSMSendUpdate*/
/*global DSMGetContactIndex*/

/*****************************************************************************!
 * Local Vars
 *****************************************************************************/
var ContactBreakerClosedColor =  "#20F000C0";
var ContactBreakerOpenColor   =  "#F02000C0";

/*****************************************************************************!
 * Function : SelectContactCB
 *****************************************************************************/
function SelectContactCB(InEvent) {

  SelectContact(InEvent.srcElement);
}

/*****************************************************************************!
 * Function : SelectContact
 *****************************************************************************/
function SelectContact(InContact) {
  DeselectContacts();
  InContact.setAttribute("fill", "white");
  InContact.selected = true;
  DeselectOtherDSMs(InContact.parentElement);
  SelectDSM(InContact.parentElement);
}

/*****************************************************************************!
 * Function : DeselectContacts
 *****************************************************************************/
function DeselectContacts()
{
  var i, j, k;

  for (i = 0; i < Bays.length; i++) {
    for (k = 0; k < Bays[i].dsms.length; k++) {
      for (j = 0; j < Bays[i].dsms[k].contacts.length; j++) {
        Bays[i].dsms[k].contacts[j].setAttribute("fill", "black");
        Bays[i].dsms[k].contacts[j].selected = false;
      }
    }
  }
}

/*****************************************************************************!
 * Function : CreateHorizontalContact
 *****************************************************************************/
function CreateHorizontalContact(InDSM, InContactNumber) {
  var                         contact;

  contact = document.createElementNS(SVGNS, "rect");
  contact.setAttribute("width", 6);
  contact.setAttribute("height", 20);
  contact.setAttribute("x", DSMHorizontalContactPositions[InContactNumber].x);
  contact.setAttribute("y", DSMHorizontalContactPositions[InContactNumber].y);
  contact.setAttribute("fill", "black");
  contact.onclick = function(InEvent) { SelectContactCB(InEvent); };
  contact.dsm = InDSM;
  contact.occupied = false;
  contact.elementType = "contact";
  contact.selected = false;
  contact.current = 0;
  contact.contactState = "Open";
  contact.contactIndex = InContactNumber;
  contact.alarm = false;
  contact.breakerFuse = null;
	contact.breakerFuseWidth = 0;
  InDSM.appendChild(contact);
  InDSM.contacts.push(contact);
}

/*****************************************************************************!
 * Function : CreateVerticalContact
 *****************************************************************************/
function CreateVerticalContact(InDSM, InContactNumber) {
  var                         contact;

  contact = document.createElement("div");
  contact.className = "DSMVerticalContact";
  contact.style.left = DSMVerticalContactPositions[InContactNumber].x;
  contact.style.top =  DSMVerticalContactPositions[InContactNumber].y;
  contact.selected = false;
  contact.occupied = false;
  contact.dsm = InDSM;
  InDSM.appendChild(contact);
  InDSM.contacts.push(contact);
}

/*****************************************************************************!
 * 
 *****************************************************************************/
function BreakerAllowDrop(ev) {
  ev.preventDefault();
}

/*****************************************************************************!
 * 
 *****************************************************************************/
function BreakerDrag(ev) {
  ev.dataTransfer.setData("text", ev.target.id);
}

/*****************************************************************************!
 * 
 *****************************************************************************/
function BreakerDrop(ev) {
  ev.preventDefault();
  var data = ev.dataTransfer.getData("text");
  ev.target.appendChild(document.getElementById(data));
}

/*****************************************************************************!
 * Function : CreateCircuitBreaker
 *****************************************************************************/
function CreateCircuitBreaker(InParent, InContact, InX, InY, InWidth, InHeight, InCount, InRating, InContactIndex) {

  var clickArea, b, breaker;
  var x1, x2, x3, x4, y1, y2, y3, y4, cx;
  var i, h, d;
  this.body = document.createElementNS(SVGNS, "g");

  InParent.appendChild(this.body);
  
  var breakerBody = document.createElementNS(SVGNS, "rect");

  if ( InContact ) {
    InContact.breakerFuse = breakerBody;
    InContact.breakerFuseWidth = InCount; 
    InContact.breakerCount = InCount;
    InContact.breakerRating = InRating;
  }
  var totalWidth = InWidth * InCount;
  breakerBody.setAttribute("x", InX);
  breakerBody.setAttribute("y", InY);
  breakerBody.setAttribute("width", totalWidth);
  breakerBody.setAttribute("height", InHeight);
  breakerBody.setAttribute("fill", "silver");
  breakerBody.setAttribute("stroke", "black");

  b = document.createElementNS(SVGNS, "rect");
  b.setAttribute("y", InY + 5);
  b.setAttribute("x", InX);
  b.setAttribute("width", totalWidth);
  b.setAttribute("height", InHeight - 10);
  b.setAttribute("fill", "#000000FF");

  breaker = document.createElementNS(SVGNS, "path");
  breaker.id = "Switch";
  x1 = InX + 3;
  x2 = InX + (totalWidth - 3);
  x3 = InX + (totalWidth - 3);
  x4 = InX + 2;

  y1 = InY + ((InHeight/2));
  y2 = InY + ((InHeight/2));
  y3 = y1 + 4;
  y4 = y2 + 4;

  this.body.appendChild(breakerBody);
  this.body.appendChild(b);
  this.body.appendChild(breaker);
  cx = InX + InWidth/2;
  
  for (i = 0; i < InCount; i++) {
    h = document.createElementNS(SVGNS, "circle");
    h.setAttribute('cx', cx);
    h.setAttribute('cy', InY + 3);
    h.setAttribute("r", 1.4);
    h.setAttribute("fill", "#00000080");
    this.body.appendChild(h);

    h = document.createElementNS(SVGNS, "circle");
    h.setAttribute('cx', cx);
    h.setAttribute('cy', InY + InHeight - 3);
    h.setAttribute("r", 1.4);
    h.setAttribute("fill", "#000000FF");
    this.body.appendChild(h);
    cx += InWidth;
  }

  d = "M" + x1 + "," + y1 + " ";
  d += x2 + "," + y2 + " ";
  d += x3 + "," + y3 + " ";
  d += x4 + "," + y4 + "Z";

  breaker.setAttribute("d", d);
  breaker.setAttribute("fill", ContactBreakerClosedColor);
  clickArea = document.createElementNS(SVGNS, "rect");
  clickArea.setAttribute("fill", "#00000010");
  clickArea.setAttribute("x", InX);
  clickArea.setAttribute("y", InY);
  clickArea.setAttribute("width", totalWidth);
  clickArea.setAttribute("height", InHeight);
  clickArea.contactType = InRating;
  clickArea.contactWidth = InCount;
  clickArea.contactIndex = InContactIndex;
  clickArea.contact = InContact;
  if ( InContact ) {
    clickArea.dsm = InContact.dsm;
  }
  clickArea.breaker = breaker;
  clickArea.style.cursor = "hand";
  clickArea.draggable = "true";
  clickArea.ondragstart = function(event) { BreakerDrag(event); }
  clickArea.contactBasicType = "Breaker";
  clickArea.contactState = "Closed";
  clickArea.id = "SelectArea";
  this.body.appendChild(clickArea);
  this.body.contactState = "Closed";
  clickArea.onclick = function(InEvent) { ContactTypeAssignCB(InEvent); };
  clickArea.onmouseover = function(InEvent) { ContactTypeHoverCB(InEvent, true); };
  clickArea.onmouseleave= function(InEvent) { ContactTypeHoverCB(InEvent, false); };
  if ( InContact ) {
    SetContactState(InContact.dsm, InContact.contactIndex, "Closed");
  }
  return this.body;
}

/*****************************************************************************!
 * Function : CreateFuse
 *****************************************************************************/
function CreateFuse(InParent, InContact, InX, InY, InWidth, InHeight, InCount, InRating, InContactIndex) {

	console.log(InCount);
  var x, i, b, clickArea;
  x = InX;
  var totalWidth = InWidth * InCount;
  this.fuseBody = document.createElementNS(SVGNS, "g");
  InParent.appendChild(this.fuseBody);

  if ( InContact ) {
    InContact.breakerFuse = this.fuseBody;
    InContact.breakerFuseWidth = InCount;
    InContact.breakerCount = InCount;
    InContact.breakerRating = InRating;
  }
  for (i = 0; i < InCount; i++, x += InWidth) {
    var fuse = document.createElementNS(SVGNS, "rect");
    fuse.setAttribute("x", x);
    fuse.setAttribute("y", InY);
    fuse.setAttribute("width", InWidth);
    fuse.setAttribute("height", InHeight);
    fuse.setAttribute("fill", "silver");
    fuse.setAttribute("stroke", "black");
    
    b = document.createElementNS(SVGNS, "rect");
    b.id = "Switch";
    b.setAttribute("y", InY + 6);
    b.setAttribute("x", x+1);
    b.setAttribute("width", InWidth-2);
    b.setAttribute("height", InHeight - 12);
    b.setAttribute("fill", "#20A000FF");
    b.setAttribute("stroke", "black");
    
    this.fuseBody.appendChild(fuse);
    this.fuseBody.appendChild(b);
  }
  clickArea = document.createElementNS(SVGNS, "rect");
  clickArea.setAttribute("fill", "#00000010");
  clickArea.setAttribute("x", InX);
  clickArea.setAttribute("y", InY);
  clickArea.setAttribute("width", totalWidth);
  clickArea.setAttribute("height", InHeight);
  clickArea.contactType = InRating;
  clickArea.contactWidth = InCount;
  clickArea.contactIndex = InContactIndex;
  clickArea.contactBasicType = "Fuse";
  clickArea.contactState = "Closed";
  clickArea.contact = InContact;
  if ( InContact ) {
    clickArea.dsm = InContact.dsm;
  }
  clickArea.id = "SelectArea";
  this.fuseBody.contactState = "Closed";
  this.fuseBody.appendChild(clickArea);
  clickArea.onclick = function(InEvent) { ContactTypeAssignCB(InEvent); };
  clickArea.onmouseover = function(InEvent) { ContactTypeHoverCB(InEvent, true); };
  clickArea.onmouseleave= function(InEvent) { ContactTypeHoverCB(InEvent, false); };
  if ( InContact ) {
    SetContactState(InContact.dsm, InContact.contactIndex, "Closed");
  }
  return this.fuseBody;
}

/*****************************************************************************!
 * Function : CreateBullet
 *****************************************************************************/
function CreateBullet(InParent, InContact, InX, InY, InWidth, InHeight, InCount, InRating, InContactIndex) {
  var b, breaker, x1, x2, x3, x4, y1, y2, y3, y4, d, cx, i, h, clickArea;
  this.body = document.createElementNS(SVGNS, "g");
  this.body.contactState = "Closed";
  InParent.appendChild(this.body);
  var breakerBody = document.createElementNS(SVGNS, "rect");

  if ( InContact ) {
    InContact.breakerFuse = this.body;
    InContact.breakerFuseWidth = InWidth;
    InContact.breakerCount = InCount;
    InContact.breakerRating = InRating;
  }

  var totalWidth = InWidth * InCount;
  breakerBody.setAttribute("x", InX);
  breakerBody.setAttribute("y", InY);
  breakerBody.setAttribute("width", totalWidth);
  breakerBody.setAttribute("height", InHeight);
  breakerBody.setAttribute("fill", "silver");
  breakerBody.setAttribute("stroke", "black");

  b = document.createElementNS(SVGNS, "rect");
  b.setAttribute("y", InY + 5);
  b.setAttribute("x", InX);
  b.setAttribute("width", totalWidth);
  b.setAttribute("height", InHeight - 10);
  b.setAttribute("fill", "#404080");

  breaker = document.createElementNS(SVGNS, "path");
  breaker.id = "Switch";
  x1 = InX + 2;
  x2 = InX + (totalWidth - 2);
  x3 = InX + (totalWidth - 3);
  x4 = InX + 3;

  y1 = InY + ((InHeight/2));
  y2 = InY + ((InHeight/2));
  y3 = y1 + 4;
  y4 = y2 + 4;

  d = "M" + x1 + "," + y1 + " ";
  d += x2 + "," + y2 + " ";
  d += x3 + "," + y3 + " ";
  d += x4 + "," + y4 + "Z";

  breaker.setAttribute("d", d);
  breaker.setAttribute("fill", ContactBreakerClosedColor);
  
  this.body.appendChild(breakerBody);
  this.body.appendChild(b);
  this.body.appendChild(breaker);
  cx = InX + InWidth/2;
  
  for (i = 0; i < InCount; i++) {
    h = document.createElementNS(SVGNS, "circle");
    h.setAttribute('cx', cx);
    h.setAttribute('cy', InY + 3);
    h.setAttribute("r", 1.5);
    h.setAttribute("fill", "#0000F0C0");
    this.body.appendChild(h);

    h = document.createElementNS(SVGNS, "circle");
    h.setAttribute('cx', cx);
    h.setAttribute('cy', InY + InHeight - 3);
    h.setAttribute("r", 1.5);
    h.setAttribute("fill", "#0000F0C0");
    this.body.appendChild(h);
    cx += InWidth;
  }

  clickArea = document.createElementNS(SVGNS, "rect");
  clickArea.id = "SelectArea";
  clickArea.setAttribute("fill", "#00000010");
  clickArea.setAttribute("x", InX);
  clickArea.setAttribute("y", InY);
  clickArea.setAttribute("width", totalWidth);
  clickArea.setAttribute("height", InHeight);
  clickArea.contactType = InRating;
  clickArea.contactBasicType = "Bullet";
  clickArea.contactState = "Closed";
  clickArea.contactWidth = InCount;
  clickArea.contact = InContact;
  if ( InContact ) {
    clickArea.dsm = InContact.dsm;
  }
  clickArea.contactIndex = InContactIndex;
  this.body.appendChild(clickArea);
  clickArea.onclick = function(InEvent) { ContactTypeAssignCB(InEvent); };
  clickArea.onmouseover = function(InEvent) { ContactTypeHoverCB(InEvent, true); };
  clickArea.onmouseleave= function(InEvent) { ContactTypeHoverCB(InEvent, false); };
  if ( InContact ) {
    SetContactState(InContact.dsm, InContact.contactIndex, "Closed");
  }
  return this.body;
}

/*****************************************************************************!
 * Function : ContactTypeAssignCB
 *****************************************************************************/
function ContactTypeAssignCB(InEvent) {
  var contact, contactIndex, dsm, n, i, c, m;
  var x, y, type, contactWidth, k, b, contactType, contactTypeParts;
  var maxCurrent, update;
  
  contact = FindSelectedContact();
  if ( contact == null ) {
    return;
  }
  if ( contact.occupied ) {
    return;
  }
  contactIndex = DSMGetContactIndex(contact.dsm, contact);
  dsm = contact.dsm;
  contactType = InEvent.srcElement.contactType;

  x = parseInt(contact.getAttribute("x"), 10);
  x -= 2;
  y = parseInt(contact.getAttribute("y"), 10);
  contactTypeParts = contactType.split(' ');
  if ( contactTypeParts.length != 3 ) {
    return;
  }
  type = contactTypeParts[2];
  maxCurrent = parseInt(contactTypeParts[0], 10);
  contactWidth = InEvent.srcElement.contactWidth;
  m = contactIndex;
  if ( ContactElementFits(dsm, contact, contactWidth) ) {
    for (k = 0; k < contactWidth; k++, m++) {
      dsm.contacts[m].occupied = false;
    }
		console.log(contactWidth);
		dsm.contacts[contactWidth].occupied = true;
    b = null;
    if ( type == "Bullet" ) {
      b = CreateBullet(dsm, contact, x, y, 10, 25, contactWidth, contactType, contactIndex);
    } else if ( type == "Fuse" ) {
      b = CreateFuse(dsm, contact, x, y, 10, 25, contactWidth, contactType, contactIndex);
    } else if ( type == "Breaker" ) {
      b = CreateCircuitBreaker(dsm, contact, x, y, 10, 25, contactWidth, contactType, contactIndex);
    }
    DeselectContacts();
    if ( b == null ) {
      return;
    }
    contact.breakerFuse = b;
    b.maxCurrent = maxCurrent;
    n = b.children.length;
    for (i = 0;i < n; i++) {
      c = b.children[i];
      if ( c.id == "SelectArea" ) {
        c.onclick = function(InEvent) { ContactSelectAssignedCB(InEvent); };
        break;
      }
    }
    dsmIndex = dsm.dsmIndex;
    
    contactIndex++; 
    update = {
      "type" : "shunt",
      "command" : "add",
      "shuntType" : type.toString(),
      "shuntIndex" : contactIndex.toString(),
      "unitNumber" : dsmIndex.toString(),
      "bayIndex" : GetBayIndex(dsm.bay).toString(),
      "shuntMaxCurrent" : maxCurrent.toString(),
      "shuntSize" : contactWidth.toString()
    }
    nextContact = FindNextUnSelectedContact(contact);
    if ( nextContact ) {
      SelectContact(nextContact);
    }
    SendGeneralRequest(update);   
  }
}

/*****************************************************************************!
 * Function : ContactTypeHoverCB
 *****************************************************************************/
function ContactTypeHoverCB(InEvent, InShow) {
  var shunt = InEvent.srcElement;
  var dsm = shunt.dsm;
  var currentBox = document.getElementById("ShuntCurrentDisplay");
  var bay;
  var dsmX, dsmY;
  if ( dsm == undefined ) {
    return;
  }

  if ( InShow == false ) {
    currentBox.style.visibility = "hidden";
    return;
  }
  bay = dsm.bay;
  bayX = parseInt(bay.style.left, 10);
  bayY = parseInt(bay.style.top, 10);
  dsmX = parseInt(dsm.style.left, 10);
  dsmY = parseInt(dsm.style.top, 10);
  contactX = parseInt(shunt.getAttribute("x"), 10);
  contactY = parseInt(shunt.getAttribute("y"), 10);
  boxH = parseInt(currentBox.style.height, 10);

  currentBox.style.visibility = "visible";
  currentBox.style.top = contactY + bayY + dsmY - boxH;
  currentBox.style.left = contactX + bayX + dsmX;
  currentBox.style.zIndex = 100;
  currentBox.innerHTML = shunt.contact.current;
}

/*****************************************************************************!
 * Function : ContactSelectAssignedCB
 *****************************************************************************/
function ContactSelectAssignedCB(InEvent) {
  if ( InEvent.ctrlKey ) {
    ContactEdit(InEvent);
    return;
  }
  if ( InEvent.srcElement.contactBasicType == "Breaker" ) {
    ToggleBreaker(InEvent.srcElement.parentElement, InEvent.srcElement.contact);
  } else if ( InEvent.srcElement.contactBasicType == "Bullet" ) {
    ToggleBullet(InEvent.srcElement.parentElement, InEvent.srcElement.contact);
  } else if ( InEvent.srcElement.contactBasicType == "Fuse" ) {
    ToggleFuse(InEvent.srcElement.parentElement, InEvent.srcElement.contact);
  }
}

/*****************************************************************************!
 * Function : ContactEdit
 *****************************************************************************/
function ContactEdit(InEvent) {
  var section, shuntIndex, current, contact, dsm;
  var element = InEvent.srcElement;
  contact = element.contact;
  selectedContact = contact;
  dsm = element.dsm;
  var shuntEdit = document.getElementById("ShuntEdit");
  shuntEdit.style.visibility = "visible";
  shuntEdit.style.height = 110;

  section = document.getElementById("ShuntSection1");
  shuntIndex = contact.contactIndex;
  section.style.visibility = "visible";
  document.getElementById("ShuntNumber1").innerHTML = shuntIndex + 1;
  current = dsm.contacts[shuntIndex].current;
  document.getElementById("ShuntCurrent1").value = current;
  document.getElementById("ShuntCurrentSlider1").value = current;
  
  selectedContact = contact;
}

/*****************************************************************************!
 * Function : ToggleBreaker
 *****************************************************************************/
function ToggleBreaker(InBreaker, InContact)
{
  if ( InBreaker.contactState == "Closed" ) {
    InBreaker.contactState = "Open";
  } else {
    InBreaker.contactState = "Closed";
  }
  SetBreaker(InBreaker, InContact);
  SetContactState(InContact.dsm, InContact.contactIndex, InBreaker.contactState);
  var dsmIndex = InContact.dsm.dsmIndex;
  var index = InContact.contactIndex + 1
  update = {
    "type" : "shunt",
    "command" : InBreaker.contactState.toString(),
    "bayIndex" : GetBayIndex(InContact.dsm.bay).toString(),
    "unitNumber" : dsmIndex.toString(),
    "shuntIndex" : index.toString()
  }
  SendGeneralRequest(update);
}

/******************************************************************************!
 * Function : SetBreaker
 ******************************************************************************/
function SetBreaker(InBreaker, InContact) {
  var x, y, x1, x2, x3, x4, y1, y2, y3, y4, width, height, d, update;
  var selectArea = FindChildElementById(InBreaker, "SelectArea");
  x = parseInt(selectArea.getAttribute("x"), 10);
  y = parseInt(selectArea.getAttribute("y"), 10);
  width = parseInt(selectArea.getAttribute("width"), 10);
  height = parseInt(selectArea.getAttribute("height"), 10);

  if ( InBreaker.contactState == "Open" ) {
    x1 = x + 2;
    x2 = x + (width - 2);
    x3 = x + (width - 3);
    x4 = x + 3;
    
    y1 = y + ((height/2)-4);
    y2 = y + ((height/2)-4);
    y3 = y1 + 4;
    y4 = y2 + 4;
  } else {
    x1 = x + 3;
    x2 = x + (width - 3);
    x3 = x + (width - 2);
    x4 = x + 2;
    
    y1 = y + ((height/2));
    y2 = y + ((height/2));
    y3 = y1 + 4;
    y4 = y2 + 4;
  }
  d = "M" + x1 + "," + y1 + " ";
  d += x2 + "," + y2 + " ";
  d += x3 + "," + y3 + " ";
  d += x4 + "," + y4 + "Z";

  var sw = FindChildElementById(InBreaker, "Switch");
  
  if ( sw ) {
    if ( InBreaker.contactState == "Open" ) {
      sw.setAttribute("fill", ContactBreakerOpenColor);
      sw.setAttribute("d", d);
    } else {
      sw.setAttribute("fill", ContactBreakerClosedColor);
      sw.setAttribute("d", d);
    }
  }
}

/*****************************************************************************!
 * Function : ToggleBullet
 *****************************************************************************/
function ToggleBullet(InBreaker, InContact)
{
  InBreaker.contactState = InBreaker.contactState == "Closed" ? "Open" : "Closed";
  SetBullet(InBreaker, InContact);
  var index = InContact.contactIndex + 1;
  var dsmIndex = InContact.dsm.dsmIndex;
  update = {
    "type" : "shunt",
    "command" : InBreaker.contactState,
    "bayIndex" : GetBayIndex(InContact.dsm.bay).toString(),
    "unitNumber" : dsmIndex.toString(),
    "shuntIndex" : index.toString()
  }
  console.log(update);
  SetContactState(InContact.dsm, InContact.contactIndex, InBreaker.contactState);
  SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : SetBullet
 *****************************************************************************/
function SetBullet(InBreaker, InContact)
{
  var x1, x2, x3, x4, x, y1, y2, y3, y4, y, height, d, width, update;
  var selectArea = FindChildElementById(InBreaker, "SelectArea");
  x = parseInt(selectArea.getAttribute("x"), 10);
  y = parseInt(selectArea.getAttribute("y"), 10);
  width = parseInt(selectArea.getAttribute("width"), 10);
  height = parseInt(selectArea.getAttribute("height"), 10);

  if ( InBreaker.contactState == "Open" ) {
    x1 = x + 2;
    x2 = x + (width - 2);
    x3 = x + (width - 3);
    x4 = x + 3;
    
    y1 = y + ((height/2)-4);
    y2 = y + ((height/2)-4);
    y3 = y1 + 4;
    y4 = y2 + 4;
  } else {
    x1 = x + 3;
    x2 = x + (width - 3);
    x3 = x + (width - 2);
    x4 = x + 2;
    
    y1 = y + ((height/2));
    y2 = y + ((height/2));
    y3 = y1 + 4;
    y4 = y2 + 4;
  }
  d = "M" + x1 + "," + y1 + " ";
  d += x2 + "," + y2 + " ";
  d += x3 + "," + y3 + " ";
  d += x4 + "," + y4 + "Z";

  var sw = FindChildElementById(InBreaker, "Switch");
  
  if ( sw ) {
    if ( InBreaker.contactState == "Open" ) {
      sw.setAttribute("fill", ContactBreakerOpenColor);
      sw.setAttribute("d", d);
    } else {
      sw.setAttribute("fill", ContactBreakerClosedColor);
      sw.setAttribute("d", d);
    }
  }
}

/*****************************************************************************!
 * Function : ToggleFuse
 *****************************************************************************/
function ToggleFuse(InBreaker)
{
  var i, sw, update;

  InBreaker.contactState = InBreaker.contactState == "Open" ? "Closed" : "Open";
  SetFuse(InBreaker);
  var index = InContact.contactIndex + 1;
  var dsmIndex = InContact.dsm.dsmIndex;
  update = {
    "type" : "shunt",
    "command" : InBreaker.contactState,
    "bayIndex" : GetBayIndex(InContact.dsm.bay).toString(),
    "unitNumber" : dsmIndex.toString(),
    "shuntIndex" : index.toString()
  }
  console.log(update);
  SetContactState(InContact.dsm, InContact.contactIndex, InBreaker.contactState);
  SendGeneralRequest(update);
}

/*****************************************************************************!
 * Function : SetFuse
 *****************************************************************************/
function SetFuse(InBreaker)
{
  var sws = FindChildElementsById(InBreaker, "Switch");
  for (i = 0; i < sws.length; i++) {
    sw = sws[i];
    if ( InBreaker.contactState == "Closed" ) {
      sw.setAttribute("fill", ContactBreakerClosedColor);
    } else {
      sw.setAttribute("fill", ContactBreakerOpenColor);
    }
  }
}

/******************************************************************************!
 * Function : SetContactState
 ******************************************************************************/
function SetContactState(InDSM, InContactIndex, InState) {
  var i, n;
  if ( InContactIndex >= InDSM.contacts.length ) {
    return;
  }
  n = InDSM.contacts[InContactIndex].breakerCount + InContactIndex;
  if ( n > InDSM.contacts.length ) {
    return;
  }
  for (i = InContactIndex; i < n; i++) {
    InDSM.contacts[i].contactState = InState;
  }
}

/*****************************************************************************!
 * Function : ContactElementFits
 *****************************************************************************/
function
ContactElementFits(InDSM, InContact, InWidth) {
  if ( InDSM == null || InContact == null || InWidth == 0 ) {
    return;
  }
  var startIndex, endIndex, i;
  var contactIndex = DSMGetContactIndex(InDSM, InContact);

  if ( contactIndex < 6 ) {
    startIndex = 0;
    endIndex = 6;
  } else {
    startIndex = 6;
    endIndex = 12;
  }
  for(i = startIndex; i < contactIndex; i++) {
    if ( !InDSM.contacts[i].occupied  ) {
      return false;
    }
    i += InDSM.contacts[i].contactWidth;
  }
  if ( contactIndex + InWidth > endIndex ) {
    return false;
  }
  return true;
}

/*****************************************************************************!
 * Function : FindSelectedContact
 *****************************************************************************/
function FindSelectedContact() {
  var i, j, k;

  for (i = 0; i < Bays.length; i++) {
    for (k = 0; k < Bays[i].dsms.length; k++) {
      for (j = 0; j < Bays[i].dsms[k].contacts.length; j++) {
        if ( Bays[i].dsms[k].contacts[j].selected ) {
          return Bays[i].dsms[k].contacts[j];
        }
      }
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : FindNextUnSelectedContact
 *****************************************************************************/
function FindNextUnSelectedContact(InContact) {
  dsm = InContact.dsm;
  for (i = InContact.contactIndex; i < dsm.contacts.length; i++) {
    if ( !dsm.contacts[i].occupied ) {
      return dsm.contacts[i];
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : ContactSet
 *****************************************************************************/
function ContactSet(InContact, InPower, InCurrent) {
  var breakerFuse;
  if ( InContact.breakerFuse == null ) {
    return;
  }

  breakerFuse = InContact.breakerFuse;
  if ( InPower ) {
    InContact.contactState = "Closed";
    breakerFuse.contactState = "Closed";
  } else {
    InContact.contactState = "Open";
    breakerFuse.contactState = "Open";
  }
  InContact.current = InCurrent;
  selectArea = FindChildElementById(breakerFuse, "SelectArea");
  if ( selectArea.contactBasicType == "Breaker" ) {
    SetBreaker(breakerFuse, InContact);
  } else if ( selectArea.contactBasicType == "Bullet" ) {
    SetBullet(breakerFuse, InContact);
  } else if ( selectArea.contactBasicType == "Fuse" ) {
    SetFuse(breakerFuse);
  }
}
