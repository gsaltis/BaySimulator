/*****************************************************************************
 * FILE         : main.js
 * DATE         : December 31 2018
 * PROJECT      : 
 * COPYRIGHT    : Copyright (c) 2018 Vertiv Company
 *****************************************************************************/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var Bays = [];

var BayTop      = "30px";
var BayWidth    = 144;
var BayHeight   = 504;
var BayXGap     = 25;

var RectifierHeight     = 26;
var RectifierCount      = 10;
var SVGNS       = "http://www.w3.org/2000/svg";
var messageTimeout = null;
var websocketPortNumber = "8001";
var websocketPrefix = "ws://";
var ProgramName = "Bay Simulator";
var simulatorVersion = "";

/*****************************************************************************!
 * Function : BodyResizeCB
 *****************************************************************************/
function BodyResizeCB() {
  var height = document.body.clientHeight;
  var width = document.body.clientWidth;
  var mainMenuBar = document.getElementById("MenuBar");
  var menuBar = document.getElementById("FullMenuBar");
  var mainWindow = document.getElementById("MainWindow");
  var canWindow = document.getElementById("CANWindow");
  var contactBar = document.getElementById("ContactBar");
  var connectSection = document.getElementById("ConnectSection");
  var dsmSection = document.getElementById("DSMMenuSection");
	
  document.getElementById("MenuShield").style.visibility = "hidden";
  
  if ( width < 600 ) {
		console.log("");
    menuBar.style.height = "90";
    menuBarHeight = "95";
    mainWindow.style.top = "100";
    canWindow.style.top = "110";
    contactBar.style.top = "55";
    connectSectionLeft = "0px";
    connectSectionRight = "";
    connectSectionTop = "25px";
    dsmSectionLeft = "92";
    dsmSectionTop = "0px";
  } else {
		console.log("");
    menuBar.style.height = "70";
    menuBarHeight = "65";
    mainWindow.style.top = "70";
    canWindow.style.top = "70";
    contactBar.style.top = "25";
    connectSectionLeft = "";
    connectSectionRight = "0px";
    connectSectionTop = "0px";
    dsmSectionLeft = "92";
    dsmSectionTop = "0";
  }
  connectSection.style.left = connectSectionLeft;
  connectSection.style.right = connectSectionRight;
  connectSection.style.top = connectSectionTop;
  dsmSection.style.left = dsmSectionLeft;
  dsmSection.style.top = dsmSectionTop;
  mainMenuBar.style.height = menuBarHeight;
}

/*****************************************************************************!
 * Function : ContactAllowDrop
 *****************************************************************************/
function ContactAllowDrop(ev) {
  ev.preventDefault();
}

/*****************************************************************************!
 * Function : ContactDrag
 *****************************************************************************/
function ContactDrag(ev) {
  ev.dataTransfer.setData("text", ev.target.id);
}

/*****************************************************************************!
 * Function : ContactDrop
 *****************************************************************************/
function ContactDrop(ev) {
  ev.preventDefault();
  ev.preventDefault();
  var data = ev.dataTransfer.getData("text");    
}

/*****************************************************************************!
 * Function : BodyLoadCB
 *****************************************************************************/
function BodyLoadCB()
{
  var b = document.getElementById("ContactBarDraw");

  CreateCircuitBreaker(b, null,  5, 4, 10, 25, 1, "400 Amp Breaker", 0);
  CreateCircuitBreaker(b, null, 20, 4, 10, 25, 2, "600 Amp Breaker", 0);
  CreateCircuitBreaker(b, null, 45, 4, 10, 25, 3, "800 Amp Breaker", 0);
  CreateCircuitBreaker(b, null, 80, 4, 10, 25, 4, "1200 Amp Breaker", 0);

  CreateFuse(b, null, 150, 4, 10, 25, 1, "400 Amp Fuse", 0);
  CreateFuse(b, null, 165, 4, 10, 25, 2, "800 Amp Fuse", 0);
  CreateFuse(b, null, 190, 4, 10, 25, 3, "1200 Amp Fuse", 0);
  CreateFuse(b, null, 225, 4, 10, 25, 4, "1600 Amp Fuse", 0);

  CreateBullet(b, null, 300, 4, 10, 25, 3, "400 Amp Bullet", 0);
  BodyResizeCB();

  document.getElementById("CANWindowMenuButtonStart").style.visibility = "hidden";
  document.getElementById("CANWindowMenuButtonPause").style.visibility = "hidden";
  document.getElementById("HostAddress").value = websocketPrefix + document.location.hostname + ":" + websocketPortNumber;
  document.title = ProgramName + " - " + document.location.hostname;
  ConnectRequest();
}

/*****************************************************************************!
 * Function : ChangeSimulatorStateCB
 *****************************************************************************/
function ChangeSimulatorStateCB()
{
  var                         menu = document.getElementById("MenuSimState");
  if ( menu.innerHTML == "Start Simulator" ) {
    menu.className = "MenuEntry MenuEntrySimRunning";
    menu.innerHTML = "Stop Simulator";
    AddNormalMessage("Simulator Started");
  } else {
    menu.className = "MenuEntry";
    menu.innerHTML = "Start Simulator";
    AddNormalMessage("Simulator Stopped");
  }
}

/*****************************************************************************!
 * Function : AddMessage
 *****************************************************************************/
function AddMessage(InLevel, InMessage)
{
  var                         id = document.getElementById("MessageArea");

  if ( InLevel == "" ) {
    id.className = "MessageArea";
  } else {
    id.className = "MessageArea MessageArea" + InLevel;
  }
  id.innerHTML = InMessage;
  messageTimeout = setTimeout(function() { ClearMessage(); }, 3000);
}

/*****************************************************************************!
 * Function : ClearMessage
 *****************************************************************************/
function ClearMessage()
{
  AddMessage("", "");
  clearTimeout(messageTimeout);
}

/*****************************************************************************!
 * Function : AddNormalMessage
 *****************************************************************************/
function AddNormalMessage(InMessage) {
  AddMessage("", InMessage);
}

/*****************************************************************************!
 * Function : AddWarningMessage
 *****************************************************************************/
function AddWarningMessage(InMessage) {
  AddMessage("Warning", InMessage);
}

/*****************************************************************************!
 * Function : AddAlarmMessage
 *****************************************************************************/
function AddAlarmMessage(InMessage) {
  AddMessage("Alarm", InMessage);
  
}

/*****************************************************************************!
 * Function : SubMenuAddBayHideCB
 *****************************************************************************/
function SubMenuAddBayHideCB()
{
  var id = document.getElementById("SubMenuAddBay");
  id.style.visibility = "hidden";
}

/*****************************************************************************!
 * Function : RemoveClass
 *****************************************************************************/
function RemoveClass(InElement, InClassName) {
	/*    var oldClassName;
				var classNameElements = [];

				if ( InElement == null ) {
        return;
				}

				oldClassName = InElement.className;
				classNameElements = oldClassName.split(' ').filter(function(value, index, arr) {
        return value != InClassName;
				});
				InElement.className = classNameElements.join(' ');
	*/
}

/*****************************************************************************!
 * Function : FindChildElementById
 *****************************************************************************/
function FindChildElementById(InElement, InID) {
  var i, n, ch;

  n = InElement.children.length;
  if ( n == 0 ) {
    return null;
  }
  for (i = 0; i < n; i++) {
    ch = InElement.children[i];
    if ( ch.id == InID ) {
      return ch;
    }
  }
  return null;
}

/*****************************************************************************!
 * Function : FindChildElementsById
 *****************************************************************************/
function FindChildElementsById(InElement, InID) {
  var i, n, ch, elements;

  elements = []

  n = InElement.children.length;
  if ( n == 0 ) {
    return null;
  }
  for (i = 0; i < n; i++) {
    ch = InElement.children[i];
    if ( ch.id == InID ) {
      elements.push(ch);
    }
  }
  return elements;
}

/*****************************************************************************!
 * Function : CANMonitorStart 
 *****************************************************************************/
function CANMonitorStart()
{

}

/*****************************************************************************!
 * Function : CANMonitorPause
 *****************************************************************************/
function CANMonitorPause()
{


}

/*****************************************************************************!
 * Function : CANMonitorStop
 *****************************************************************************/
function CANMonitorStop()
{


}

/*****************************************************************************!
 * Function : CANMonitorClear
 *****************************************************************************/
function CANMonitorClear()
{


}

/*****************************************************************************!
 * Function : SaveBayCB
 *****************************************************************************/
function SaveBayCB()
{
  var update = {
    "command" : "save"
  };
  SendGeneralRequest(update);   
  AddNormalMessage("Bay saved");
}

/*****************************************************************************!
 * Function : ReadBayCB
 *****************************************************************************/
function ReadBayCB()
{
  var update = {
    "command" : "read"
  };
  SendGeneralRequest(update);   
  AddNormalMessage("Reading Bay");
}

/*****************************************************************************!
 * Function : ShowAboutCB
 *****************************************************************************/
function ShowAboutCB()
{
 	var aboutBox = document.getElementById("AboutBox");
 	aboutBox.style.visibility = "visible";
	document.getElementById("AboutVersion").innerHTML = versionString;
	document.getElementById("AboutApplication").innerHTML = programNameString;
	document.getElementById("MenuShield").style.visibility ="visible";
}

/*****************************************************************************!
 * Function : CloseAboutBoxCB
 *****************************************************************************/
function CloseAboutBoxCB()
{
 	var aboutBox = document.getElementById("AboutBox");
 	aboutBox.style.visibility = "hidden";
	document.getElementById("MenuShield").style.visibility ="hidden";
}
