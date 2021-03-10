/*****************************************************************************!
 * FILE			: websocketif.js
 * DATE			: April 18 2018
 * PROJECT		: CAN Simulator
 *****************************************************************************/

/*global  nextSerialNumber*/
/*global DisplayNewBay*/
/*global CreateBay*/
/*global DSMProductNumber*/

/*****************************************************************************!
 * Local Data
 *****************************************************************************/
var WebSocketIFConnection = null;

/*****************************************************************************!
 * Function : InitializeWebSocket
 *****************************************************************************/
function InitializeWebSocket() {
  var requestButton = document.getElementById("DisconnectButton");
  requestButton.disabled = true;
}

/*****************************************************************************!
 * Function : ConnectRequest
 *****************************************************************************/
function ConnectRequest() {
  var hostAddressElement = document.getElementById("HostAddress");
  hostAddress = hostAddressElement.value;
  // ShowDeviceList();

  WebSocketIFConnection = new WebSocket(hostAddress);

  WebSocketIFConnection.onopen = function () {
		// SetConnectedButtons();
		AddNormalMessage("Connected to " + hostAddress);
		// GetDeviceDefinitions();
		SendAnnounce();
    document.getElementById("DisconnectButton").innerHTML = "Connected : ";
    document.getElementById("DisconnectButton").style.color = "#fff";
    document.getElementById("DisconnectButton").style.background = "#0C0";
    document.getElementById("MenuShield").style.visibility = "hidden";
		document.getElementById("OffLineOverlay").style.visibility = "hidden";
  };

  // Log errors
  WebSocketIFConnection.onerror = function (error) {
		SetErrorMessage("Error connecting to " + hostAddress);
		document.getElementById("OffLineOverlay").style.visibility = "visible";
		SetDisconnectedButtons();
  };

  // 
  WebSocketIFConnection.onclose = function() {
		AddNormalMessage("Disconnecting");
		document.getElementById("OffLineOverlay").style.visibility = "visible";
  }
  
  // Log messages from the server
  WebSocketIFConnection.onmessage = function (e) {
		HandleResponse(e.data);
  };

  // document.getElementById("DeviceSelector").disabled = false;
}

/*****************************************************************************!
 * Function : DisconnectRequest
 *****************************************************************************/
function DisconnectRequest()
{
  var element;

  element = document.getElementById("DeviceSelector");
  element.disabled = true;
  element.selectedIndex = 0;

  SetDisconnectedButtons();
  WebSocketIFConnection.close();
  HideDeviceList();
  MessageDisplayClear();
  document.getElementById("AddDeviceButton").innerHTML = "";
  document.getElementById("RemoveDeviceButton").innerHTML = "";
  document.getElementById("DeviceButtonSeparator").style.visibility = "hidden";
  ClearDeviceDisplayElements();
  Devices = [];
}

/*******************************************************************************
 * Function : ClearDeviceDisplayElements
 *******************************************************************************/
function ClearDeviceDisplayElements()
{
  var n, i, element;
  element = document.getElementById("DeviceTabBar");
  n = element.childNodes.length;
  for (i = 0; i < n; i++) {
		element.removeChild(element.childNodes[0]);
  }

  element = document.getElementById("DeviceContents");
  n = element.childNodes.length;
  for (i = 0; i < n; i++) {
		element.removeChild(element.childNodes[0]);
  }
}

/*****************************************************************************!
 * Function : SendMessage
 *****************************************************************************/
function SendMessage()
{
  var elements = document.getElementsByClassName("tab-link");
  var i, index, type;

  for ( i = 0; i < elements.length; i++ ) {
		if (elements[i].className == "tab-link active" ) {
	    break;
		}
  }
  if ( i >= elements.length ) {
		return;
  }
  index = elements[i].getAttribute("DeviceIndex");
  type = elements[i].getAttribute("DeviceType");
  SendDataRequest(type, index);
}

/*****************************************************************************!
 * Function : SendAnnounce
 *****************************************************************************/
function SendAnnounce() {
  var request = {};
  request.command = "announce";
  WebSocketIFConnection.send(JSON.stringify(request));
}

/*****************************************************************************!
 * Function : SendDataRequest
 *****************************************************************************/
function SendDataRequest(InType, InIndex) {
  var value = {};
  var request = {};
  request.command = "read";
  request.device = InType;
  value.name = "all";
  value.index = InIndex.toString();
  request.value = value;
  WebSocketIFConnection.send(JSON.stringify(request));
}

/*****************************************************************************!
 * Function : HandleResponse
 *****************************************************************************/
function HandleResponse(InJSONString) {
  var r = JSON.parse(InJSONString);
  if ( r.command == "read" ) {
		HandleRead(r);
  } else if ( r["command"] == "write" ) {
		HandleWrite(r);
  } else if ( r["command"] == "create" ) {
		HandleCreate(r);
  } else if ( r["command"] == "canpacket" ) {
    HandleCANPacket(r);
  } else if ( r["command"] == "canpacketstring" ) {
    console.log(r);
    HandleCANPacketString(r);
  } else if ( r["command"] == "set" ) {
    HandleSet(r);
  } else if ( r["command"] == "add" ) {
    HandleAdd(r);
  } else if ( r["command"] == "cleardisplay" ) {
		HandleClearDisplay();
  } else if ( r["command"] == "serialNumber") {
    HandleSerialNumber(r);
  } else if ( r["command"] == "Init" ) {
    HandleInit(r);
  } else if ( r.command == "info" ) {
		HandleInfo(r);
  } else if ( r.command == "status" ) {
		HandleStatus(r);
  }
}

/******************************************************************************!
 * Function : HandleStatus
 ******************************************************************************/
function HandleStatus(InJSONDoc) {
  var rectifiers = InJSONDoc.rectifiers;
  var dsms = InJSONDoc.dsms;
  
  var n = rectifiers.length;
  var i;
  var rect, bay;
  
  for ( i = 0 ; i < n; i++) {
		RectifierUpdate
		bay = BayFindByIndex(rectifiers[i].bay);
		if ( null == bay ) {
	    return;
		}
		rect = BayFindRectifier(bay, rectifiers[i].unit);
		if ( null == rect ) {
	    return;
		}
		RectifierUpdateStatus(rect, rectifiers[i].power == "true" , rectifiers[i].comm == "true",
                          rectifiers[i].current, rectifiers[i].StartUpState);
  }

  n = dsms.length;

  for ( i = 0; i < n; i++ ) {
		bay = BayFindByIndex(dsms[i].bay);
		if ( null == bay ) {
	    return;
		}

		dsm = BayFindDSM(bay, dsms[i].unit);
		if ( null == dsm ) {
	    return;
		}
		shunts = dsms[i].shunts;
		m = shunts.length;
		for ( j = 0; j < m; j++) {
	    contact = DSMFindContact(dsm, shunts[j].shunt);
	    if ( contact != null ) {
				if ( shunts[j].occupied == "true" ) {
					ContactSet(contact, shunts[j].power == "true", shunts[j].current);
				}
	    }
		}
  }
}

/******************************************************************************!
 * Function : HandleInfo
 ******************************************************************************/
function HandleInfo(InJSONDoc) {
  var					bay;
  
  bay = BayFindByIndex(InJSONDoc.bayIndex);
  if ( null == bay ) {
		return;
  }
  rect = BayFindRectifier(bay, InJSONDoc.unit);
  if ( null == rect ) {
		return;
  }
  
  rect.rectifierInfo.serialNumber = InJSONDoc.serialNumber;
  rect.rectifierInfo.productNumber = InJSONDoc.productNumber;
  rect.rectifierInfo.outputCurrent = InJSONDoc.current;
  rect.rectifierInfo.power = InJSONDoc.power == "On";
  rect.rectifierInfo.alarms = InJSONDoc.alarms; 
  rect.rectifierInfo.CANAddress = InJSONDoc.canAddress;
  RectifierEdit(rect); 
}

/******************************************************************************!
 * Function : HandleInit
 ******************************************************************************/
function HandleInit(InJSONDoc) {
  HandleClearDisplay();
  console.log(InJSONDoc.serialNumber);
  nextSerialNumber = InJSONDoc.serialNumber;
  DSMProductNumber = InJSONDoc.DSMProductNumber;
	DSMDistOnlyProductNumber = InJSONDoc.DSMDistOnlyProductNumber;
  InitPopulateBays(InJSONDoc.bays);
  setInterval(function() { PeriodicBayUpdateCB(); }, 5000);
}

/*****************************************************************************!
 * Function : PeriodicBayUpdateCB
 *****************************************************************************/
function PeriodicBayUpdateCB()
{
  var value = {};
  value.command = "status";
  WebSocketIFConnection.send(JSON.stringify(value));
}

/******************************************************************************!
 * Function : InitPopulateBays
 ******************************************************************************/
function InitPopulateBays(InJSONBays)
{
  console.log(InJSONBays);
  var i, bay, k, j, b, n, m, c, s, dsm;
  for (i = 0; i < InJSONBays.length; i++) {
		bay = CreateBay(InJSONBays[i].name, InJSONBays[i].type, InJSONBays[i].index);
		for (j = 0; j < InJSONBays[i].rectifiers.length; j++) {
	    rectIndex = InJSONBays[i].rectifiers[j].UnitNumber-1;
	    if ( rectIndex >= 0 && rectIndex < 10 ) {
				DisplayRectifier(bay.rectifiers[rectIndex]);
				if ( InJSONBays[i].rectifiers[j].Power == "On" ) {
					bay.rectifiers[j].rectifierInfo.power = true;
					TurnOnRectifier(bay.rectifiers[j]);
				}
	    }
		}
		for (j = 0; j < InJSONBays[i].dsms.length; j++) {
	    bay.dsms[j].style.visibility = "visible";
	    
	    for (k = 0; k < InJSONBays[i].dsms[j].Shunts.length; k++ ) {
				shunt = InJSONBays[i].dsms[j].Shunts[k];
				if ( shunt.occupied == "Yes" ) {
					x = (k + 1) * 10 + (k > 5 ? 5 : 0);
					y = 40;
					b = null;
					contact = bay.dsms[j].contacts[k];
					contact.occupied = true;
					if ( shunt.type == "Bullet" ) {
						b = CreateBullet(bay.dsms[j], contact, x, y, 10, 25, shunt.width, shunt.maxCurrent, k);
					} else if ( shunt.type == "Fuse" ) {
						console.log(j, shunt.width);
						b = CreateFuse(bay.dsms[j], contact, x, y, 10, 25, shunt.width, shunt.maxCurrent, k);
					} else if ( shunt.type == "Breaker" ) {
						b = CreateCircuitBreaker(bay.dsms[j], contact, x, y, 10, 25, shunt.width, shunt.maxCurrent, k);
					}
					if ( b != null ) {
						b.contactState = shunt.power;
						if ( shunt.type == "Breaker" ) {
							SetBreaker(b, contact);
						}
						contact.breakerFuse = b;
						contact.current = shunt.current;
						b.maxCurrent = shunt.maxCurrent;
						n = b.children.length;
						for ( m = 0; m < n; m++) {
							c = b.children[m];
							if ( c.id == "SelectArea" ) {
								c.onclick = function(InEvent) { ContactSelectAssignedCB(InEvent); }
							}
						}
					}
				}
	    }
		}
		DisplayNewBay(bay, false);
  }
}

/******************************************************************************!
 * Function : HandleSerialNumber
 ******************************************************************************/
function HandleSerialNumber() {
  var serialNumber;
  
  serialNumber = r["serialNumber"];
  console.log(serialNumber);
}

/******************************************************************************!
 * Function : HandleClearDisplay
 ******************************************************************************/
function HandleClearDisplay() {
}

/******************************************************************************!
 * Function : HandleAdd
 ******************************************************************************/
function HandleAdd(InJSONString) {
  var value = InJSONString["value"];
  
  if ( value == "bay" ) {
		HandleAddBay(InJSONString);
  } else if ( value == "rectifier" ) {
		HandleAddRectifier(InJSONString);
  }
}

/******************************************************************************!
 * Function : HandleAddRectifier
 ******************************************************************************/
function HandleAddRectifier(InJSONString) {
  var unit, bay, bayIndex, rect;
  
  unit  = InJSONString.unit;
  bayIndex   = InJSONString.bay;
  
  bay = BayFindByIndex(bayIndex);
  if ( null == bay ) {
		return;
  }
  rect = BayFindRectifier(bay,unit);
  if ( null == rect ) {
		return;
  }
  if ( rect.style.visibility == "hidden" ) {
		rect.style.visibility = "visible";
  }
}

/******************************************************************************!
 * Function : HandleAddBay
 ******************************************************************************/
function HandleAddBay(InJSONString) {
  var type, name, bay, bayIndex;
  
  if ( Bays.length < 1 ) { 
		type = InJSONString.type;
		name = InJSONString.name;
		bayIndex  = InJSONString.index;
		bay = CreateBay(name, type, bayIndex);
		DisplayNewBay(bay, false);
  }
}

/*****************************************************************************!
 * Function : HandleSet
 *****************************************************************************/
function HandleSet(InPacket) {
  if ( InPacket["value"] == "nodeaddress" ) {
    HandleSetNodeAddress(InPacket);
    return;
  } 
  
  if ( InPacket["value"] == "rectifier" ) {
		HandleRectifierSet(InPacket);
		return;
  }
}

/******************************************************************************!
 * Function : HandleRectifierSet
 ******************************************************************************/
function HandleRectifierSet(InPacket) {
  var bayIndex, unit, bay, rectifier;
  
  bayIndex = InPacket.bayIndex;
  unit = InPacket.unitNumber;
  
  bay = BayFindByIndex(bayIndex);
  if ( null == bay ) {
		return;
  }
  rectifier = BayFindRectifier(bay, unit);
  if ( rectifier == null ) {
		return;
  }

  if ( InPacket["setvalue"] == "on") {
		rectifier.rectifierInfo.power = true;
		TurnOnRectifier(rectifier);
		return;
  }
  if ( InPacket["setvalue"] == "off") {
		rectifier.rectifierInfo.power = false;
		TurnOffRectifier(rectifier);
		return;
  }
}

/*****************************************************************************!
 * Function : HandleSetNodeAddress
 *****************************************************************************/
function HandleSetNodeAddress(InPacket) {
  var                     nodeType, bayIndex, unitNumber;
  var                     canAddress, bay, rectifier;

  nodeType = InPacket["nodeType"];
  bayIndex = InPacket["bayIndex"];
  unitNumber = InPacket["unitNumber"];
  canAddress = InPacket["CANAddress"];
  
  if ( nodeType == NODE_TYPE_RECTIFIER ) {
    if ( bayIndex > Bays.length ) {
      return;
    }
    bay = BayFindByIndex(bayIndex);
    rectifier = BayFindRectifier(bay, unitNumber);
    if ( null == rectifier ) {
      return;
    }
    rectifier.rectifierInfo.CANAddress = canAddress;
  }
}

/*****************************************************************************!
 * Function : HandleCANPacketString
 *****************************************************************************/
function HandleCANPacketString(InPacket) {
  CANWindowAddStringEntry(InPacket);
}

/*****************************************************************************!
 * Function : HandleCANPacket
 *****************************************************************************/
function HandleCANPacket(InPacket) {
  CANWindowAddEntry(InPacket);
}

/*****************************************************************************!
 * Function : HandleCreate
 *****************************************************************************/
function HandleCreate(InCreateResponse) {
  var dev = new Device(InCreateResponse.value.index, InCreateResponse.value.name, InCreateResponse.device);
  var deviceDef = FindDeviceDef(InCreateResponse.device);
  dev.values = []
  Devices.push(dev);
  AddDeviceToGUI(dev);
  SelectDevice(dev);
  AddNormalMessage(deviceDef.desc + " " + dev.index + " added");
  SendDataRequest(InCreateResponse.device, dev.index);
}

/*****************************************************************************!
 * Function : HandleWrite
 *****************************************************************************/
function HandleWrite(InWriteResponse) {
}

/*****************************************************************************!
 * Function : HandleRead
 *****************************************************************************/
function HandleRead(InRequest) {
  if ( InRequest.status == "ERROR" ) {
		AddNormalMessage(InRequest.message);
  } else if ( InRequest.device == "CONFIG" ) {
		HandleConfigResponse(InRequest);
  } else if ( InRequest.device == "ALL" ) {
		HandleDevicesRequest(InRequest);
  } else if ( InRequest.values.type == "regnames" ) {
		HandleRegNamesResponse(InRequest);
  } else if ( InRequest.type == "ids" ) {
		HandleIDs(InRequest);
  } else if ( InRequest.type == "all" ) {
		DevicePopulate(InRequest);
  }
}

/*****************************************************************************
 * Function : HandleConfigResponse
 *****************************************************************************/
function HandleConfigResponse(InResponse) {
  var values = InResponse.value;
  if ( values.name == "devices" ) {
		SetDevices(values.devices);
  } else if ( values.name == "regnames" ) {
  }
}

/*****************************************************************************
 * Function : SetDevices
 *****************************************************************************/
function SetDevices(InDeviceNames) {
  var				deviceComboBox;
  var				i;
  var				button;

  deviceComboBox = document.getElementById("DeviceSelector");
  button = document.createElement("option");
  button.value = "All";
  button.style.color = "#000";
  button.innerHTML = "All";
  deviceComboBox.appendChild(button);
  for (i = 0 ; i < InDeviceNames.length ; i++) {
		button = document.createElement("option");
		button.value = InDeviceNames[i].name;
		button.style.color = "#000";
		button.innerHTML = InDeviceNames[i].name;
		deviceComboBox.appendChild(button);
		DevicesAddDeviceDef(InDeviceNames[i].name, InDeviceNames[i].desc, InDeviceNames[i].displayColumns);
  }
  DevicesGetRegisters();
}

/*****************************************************************************
 * Function : HandleRegNamesResponse
 *****************************************************************************/
function HandleRegNamesResponse(InResponse) {
  if ( InResponse.status == "Error" ) {
		return;
  }
  values = InResponse.values;

  DeviceSetRegisters(values.device, values.registers);
}

/*****************************************************************************
 * Function : HandleDevicesRequest
 *****************************************************************************/
function HandleDevicesRequest(InRequest) {
}

/*****************************************************************************!
 * Function : HandleIDs 
 *****************************************************************************/
function HandleIDs(InCounts) {
  var									values = InCounts.values;
  var									i, removeButton;
  removeButton = document.getElementById("RemoveDeviceButton");
  removeButton.className = "button dark tight disabled";
  for ( i = 0 ; i < values.length ; i++ ) {
		AddDevices(values[i].type, values[i].ids);
		if ( values[i].ids.length != 0 ) {
	    removeButton.className = "button dark tight";
		}
  }
  tabBar = document.getElementById("DeviceTabBar");
  for ( i = 0 ; i < tabBar.childNodes.length ; i++ ) {
		if ( tabBar.childNodes[i].className == "tab-link" ) {
	    SelectDeviceByIndex(i);
	    break;
		}
  }
}

/*****************************************************************************!
 * Function : AddDevices
 *****************************************************************************/
function AddDevices(InType, InDevices) {
  var									i;
  var									tabBar
  for ( i = 0 ; i < InDevices.length ; i++ ) {
		
		var dev = new Device(InDevices[i].index, InDevices[i].name, InType);
		dev.name = InDevices[i].name;
		dev.values = []
		Devices.push(dev);
		AddDeviceToGUI(dev);
  }
}

/*****************************************************************************!
 * Function : SendRegNamesRequest
 *****************************************************************************/
function SendRegNamesRequest(InDevice) {
  var value = {};
  value.command = "read";
  value.device = InDevice;
  value.name = "regnames";
  value2 = {}
  value2.name = "regnames";
  value2.device = InDevice;
  value.value = value2;
  WebSocketIFConnection.send(JSON.stringify(value));
}

/*****************************************************************************!
 * Function : GetDeviceIDs
 *****************************************************************************/
function GetDeviceIDs(InDeviceName) {
  var value = {};
  value.command = "read";
  value.device = InDeviceName;
  value.name = "ids;"
  value2 = {}
  value2.name = "ids";
  value.value = value2;
  WebSocketIFConnection.send(JSON.stringify(value));
}

/*****************************************************************************!
 * Function : SendGeneralRequest
 *****************************************************************************/
function SendGeneralRequest(InRequest) {
  if ( WebSocketIFConnection ) {
		WebSocketIFConnection.send(JSON.stringify(InRequest));
  }
}

/*****************************************************************************!
 * Function : SetConnectedButtons
 *****************************************************************************/
function SetConnectedButtons() {
  var closeButton = document.getElementById("DisconnectButton");
  closeButton.className = "button primary tight";
  closeButton.style.opacity = 1;
  var connectButton = document.getElementById("ConnectButton");
  connectButton.className = "button dark tight disabled";
  connectButton.style.opacity = .2;
  HideDeviceButtons();
}

/*****************************************************************************!
 * Function : HideDeviceButtons
 *****************************************************************************/
function HideDeviceButtons() {
  var element = document.getElementById("SaveDeviceButton");
  element.style.visibility = "hidden";
  element = document.getElementById("SaveDeviceButtonSeparator");
  element.style.visibility = "hidden";

  element = document.getElementById("AddDeviceButton");
  // 	element.innerHTML = "";

  element = document.getElementById("RemoveDeviceButton");
  // 	element.innerHTML = "";

  element = document.getElementById("DeviceButtonSeparator");
  // 	element.style.visibility = "hidden"
}

/*****************************************************************************!
 * Function : ShowSaveDeviceButtons
 *****************************************************************************/
function ShowSaveDeviceButtons() {
  var element = document.getElementById("SaveDeviceButton");
  element.style.visibility = "visible";
  element = document.getElementById("SaveDeviceButtonSeparator");
  element.style.visibility = "visible";
}

/*****************************************************************************!
 * Function : SetDisconnectedButtons
 *****************************************************************************/
function SetDisconnectedButtons() {
  var closeButton = document.getElementById("DisconnectButton");
  closeButton.className = "button primary tight disabled";
  var connectButton = document.getElementById("ConnectButton");
  connectButton.className = "button primary tight";
  connectButton.style.opacity = 1;
  closeButton.style.opacity = .2;
  HideDeviceButtons();
}

/*******************************************************************************!
 * Function : GetDeviceDefinitions
 *******************************************************************************/
function GetDeviceDefinitions() {
  var				request = {};
  if ( DeviceDefs.length > 0 ) {
		return;
  }
  request.command = "read";
  request.device  = "CONFIG";
  request.name    = "devices";
  request.value  = {};
  request.value.name = "devices";
  SendGeneralRequest(request);
}

/*****************************************************************************!
 * Function : SetMonitorCANProtocol
 *****************************************************************************/
function SetMonitorCANProtocol(InValue) {
  var             request = {};

  request.command = "set";
  request.canprotocolmonitor = InValue ? "true" : "false";
  SendGeneralRequest(request);
}

/*****************************************************************************!
 * Function : SetRectifierPower
 *****************************************************************************/
function SetRectifierPower(InRectifier) {
  var             request = {};

  request.command = "set";
  request.unitNumber = InRectifier.rectifierIndex;
  request.bayIndex = GetBayIndex(InRectifier.bay);
  request.rectifierPower = InRectifier.rectifierInfo.power ? "On" : "Off";
  SendGeneralRequest(request);
}
