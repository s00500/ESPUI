const UI_INITIAL_GUI = 200;
const UI_RELOAD = 201;
const UPDATE_OFFSET = 100;

const UI_EXTEND_GUI = 210;

const UI_TITEL = 0;

const UI_PAD = 1;
const UPDATE_PAD = 101;

const UI_CPAD = 2;
const UPDATE_CPAD = 102;

const UI_BUTTON = 3;
const UPDATE_BUTTON = 103;

const UI_LABEL = 4;
const UPDATE_LABEL = 104;

const UI_SWITCHER = 5;
const UPDATE_SWITCHER = 105;

const UI_SLIDER = 6;
const UPDATE_SLIDER = 106;

const UI_NUMBER = 7;
const UPDATE_NUMBER = 107;

const UI_TEXT_INPUT = 8;
const UPDATE_TEXT_INPUT = 108;

const UI_GRAPH = 9;
const ADD_GRAPH_POINT = 10;
const CLEAR_GRAPH = 109;

const UI_TAB = 11;
const UPDATE_TAB = 111;

const UI_SELECT = 12;
const UPDATE_SELECT = 112;

const UI_OPTION = 13;
const UPDATE_OPTION = 113;
const UI_MIN = 14;
const UPDATE_MIN = 114;
const UI_MAX = 15;
const UPDATE_MAX = 115;
const UI_STEP = 16;
const UPDATE_STEP = 116;

const UI_GAUGE = 17;
const UPDATE_GAUGE = 117;
const UI_ACCEL = 18;
const UPDATE_ACCEL = 118;

const UI_SEPARATOR = 19;
const UPDATE_SEPARATOR = 119;

const UI_TIME = 20;
const UPDATE_TIME = 120;

const UI_FILEDISPLAY = 21;
const UPDATE_FILEDISPLAY = 121;

const UI_FRAGMENT = 98;

const UP = 0;
const DOWN = 1;
const LEFT = 2;
const RIGHT = 3;
const CENTER = 4;

// Colors
const C_TURQUOISE = 0;
const C_EMERALD = 1;
const C_PETERRIVER = 2;
const C_WETASPHALT = 3;
const C_SUNFLOWER = 4;
const C_CARROT = 5;
const C_ALIZARIN = 6;
const C_DARK = 7;
const C_NONE = 255;

var controlAssemblyArray = new Object();
var FragmentAssemblyTimer = new Array();
var graphData = new Array();
var hasAccel = false;
var sliderContinuous = false;

function colorClass(colorId) {
    colorId = Number(colorId);
    switch (colorId) {
        case C_TURQUOISE:
            return "turquoise";

        case C_EMERALD:
            return "emerald";

        case C_PETERRIVER:
            return "peterriver";

        case C_WETASPHALT:
            return "wetasphalt";

        case C_SUNFLOWER:
            return "sunflower";

        case C_CARROT:
            return "carrot";

        case C_ALIZARIN:
            return "alizarin";

        case C_DARK:
        case C_NONE:
            return "dark";
        default:
            return "";
    }
}

var websock;
var websockConnected = false;
var WebSocketTimer = null;

function requestOrientationPermission() {
    /*
    // Currently this fails, since it needs secure context on IOS safari
    if (typeof DeviceMotionEvent.requestPermission === "function") {
      DeviceOrientationEvent.requestPermission()
        .then(response => {
          if (response == "granted") {
            window.addEventListener("deviceorientation", handleOrientation);
          }
        })
        .catch(console.error);
    } else {
      // Non IOS 13
      window.addEventListener("deviceorientation", handleOrientation);
    }
    */
}
/*
function handleOrientation(event) {
  var x = event.beta; // In degree in the range [-180,180]
  var y = event.gamma; // In degree in the range [-90,90]

  var output = document.querySelector(".output");
  output.innerHTML = "beta : " + x + "\n";
  output.innerHTML += "gamma: " + y + "\n";

  // Because we don't want to have the device upside down
  // We constrain the x value to the range [-90,90]
  if (x > 90) {
    x = 90;
  }
  if (x < -90) {
    x = -90;
  }

  // To make computation easier we shift the range of
  // x and y to [0,180]
  x += 90;
  y += 90;

  // 10 is half the size of the ball
  // It center the positioning point to the center of the ball
  var ball = document.querySelector(".ball");
  var garden = document.querySelector(".garden");
  var maxX = garden.clientWidth - ball.clientWidth;
  var maxY = garden.clientHeight - ball.clientHeight;
  ball.style.top = (maxY * y) / 180 - 10 + "px";
  ball.style.left = (maxX * x) / 180 - 10 + "px";
}
*/

function saveGraphData() {
    localStorage.setItem("espuigraphs", JSON.stringify(graphData));
}

function restoreGraphData(id) {
    var savedData = localStorage.getItem("espuigraphs", graphData);
    if (savedData != null) {
        savedData = JSON.parse(savedData);
        let idData = savedData[id];
        return Array.isArray(idData) ? idData : [];
    }
    return [];
}

function restart() {
    $(document).add("*").off();
    $("#row").html("");
    conStatusError();
    start();
}

function conStatusError() {
    FragmentAssemblyTimer.forEach(element => {
        clearInterval(element);
    });
    FragmentAssemblyTimer = new Array();
    controlAssemblyArray = new Array();

    if (true === websockConnected) {
        websockConnected = false;
        websock.close();
        $("#conStatus").removeClass("color-green");
        $("#conStatus").addClass("color-red");
        $("#conStatus").html("Error / No Connection &#8635;");
        $("#conStatus").off();
        $("#conStatus").on({
            click: restart,
        });
    }
}

function handleVisibilityChange() {
    if (!websockConnected && !document.hidden) {
        restart();
    }
}

function start() {
    let location = window.location.hostname;
    let port = window.location.port;
//    let location = "192.168.10.198";
//    let port = "";

    document.addEventListener("visibilitychange", handleVisibilityChange, false);
    if (
        port != "" ||
        port != 80 ||
        port != 443
    ) {
        websock = new WebSocket( "ws://" + location + ":" + port + "/ws" );
    } else {
        websock = new WebSocket("ws://" + location + "/ws");
    }

    // is the timer running?
    if (null === WebSocketTimer) {
        // timer runs forever
        WebSocketTimer = setInterval(function () {
            // console.info("Periodic Timer has expired");
            // is the socket closed?
            if (websock.readyState === 3) {
                // console.info("Web Socket Is Closed");
                restart();
            }
        }, 5000);
    } // end timer was not running

    websock.onopen = function (evt) {
        console.log("websock open");
        $("#conStatus").addClass("color-green");
        $("#conStatus").text("Connected");
        websockConnected = true;
        FragmentAssemblyTimer.forEach(element => {
            clearInterval(element);
        });
        FragmentAssemblyTimer = new Array();
        controlAssemblyArray = new Array();
    };

    websock.onclose = function (evt) {
        // console.log("Close evt: '" + evt + "'");
        // console.log("Close reason: '" + evt.reason + "'");
        // console.log("Close code: '" + evt.code + "'");
        console.log("websock close");
        conStatusError();
        FragmentAssemblyTimer.forEach(element => {
            clearInterval(element);
        });
        FragmentAssemblyTimer = new Array();
        controlAssemblyArray = new Array();
    };

    websock.onerror = function (evt) {
        console.log("websock Error");
        // console.log("Error evt: '" + evt + "'");
        // console.log("Error data: '" + evt.data + "'");

        restart();
        FragmentAssemblyTimer.forEach(element => {
            clearInterval(element);
        });
        FragmentAssemblyTimer = new Array();
        controlAssemblyArray = new Array();
    };

    var handleEvent = function (evt) {
        // console.log("handleEvent:Data evt: '" + evt + "'");
        // console.log("handleEvent:Data data: '" + evt.data + "'");
        try {
            var data = JSON.parse(evt.data);
        }
        catch (Event) {
            console.error(Event);
            // console.info("start the update over again");
            websock.send("uiok:" + 0);
            return;
        }
        var e = document.body;
        var center = "";
        // console.info("data.type: '" + data.type + "'");

        switch (data.type) {
            case UI_INITIAL_GUI:
                // Clear current elements
                $("#row").html("");
                $("#tabsnav").html("");
                $("#tabscontent").html("");

                if (data.sliderContinuous) {
                    sliderContinuous = data.sliderContinuous;
                }
                // console.info("UI_INITIAL_GUI:data record: '" + data + "'");
                data.controls.forEach(element => {
                    // console.info("element: '" + JSON.stringify(element) + "'");
                    var fauxEvent = {
                        data: JSON.stringify(element),
                    };
                    handleEvent(fauxEvent);
                });

                //If there are more elements in the complete UI, then request them
                //Note: we subtract 1 from data.controls.length because the controls always
                //includes the title element
                if (data.totalcontrols > (data.controls.length - 1)) {
                    websock.send("uiok:" + (data.controls.length - 1));
                }
                break;

            case UI_EXTEND_GUI:
                // console.info("UI_EXTEND_GUI data record: '" + data + "'");
                data.controls.forEach(element => {
                    // console.info("UI_EXTEND_GUI:element: '" + JSON.stringify(element) + "'");
                    var fauxEvent = {
                        data: JSON.stringify(element),
                    };
                    handleEvent(fauxEvent);
                });

                //Do we need to keep requesting more UI elements?
                if (data.totalcontrols > data.startindex + (data.controls.length - 1)) {
                    websock.send("uiok:" + (data.startindex + (data.controls.length - 1)));
                }
                break;

            case UI_RELOAD:
                window.location.reload();
                break;

            case UI_TITEL:
                document.title = data.label;
                $("#mainHeader").html(data.label);
                break;

            /*
              Most elements have the same behaviour when added.
            */
            case UI_LABEL:
            case UI_NUMBER:
            case UI_TEXT_INPUT:
            case UI_SELECT:
            case UI_GAUGE:
            case UI_SEPARATOR:
                if (data.visible) addToHTML(data);
                break;

                /*
              These elements must call additional functions after being added to the DOM
            */
            case UI_BUTTON:
                if (data.visible) {
                    addToHTML(data);
                    $("#btn" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).addClass("pressed");
                            buttonclick(data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).removeClass("pressed");
                            buttonclick(data.id, false);
                        },
                    });
                }
                break;

            case UI_SWITCHER:
                if (data.visible) {
                    addToHTML(data);
                    switcher(data.id, data.value);
                }
                break;

            case UI_CPAD:
            case UI_PAD:
                if (data.visible) {
                    addToHTML(data);
                    $("#pf" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).parent().addClass("pressed");
                            padclick(UP, data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).parent().removeClass("pressed");
                            padclick(UP, data.id, false);
                        },
                    });
                    $("#pl" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).parent().addClass("pressed");
                            padclick(LEFT, data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).parent().removeClass("pressed");
                            padclick(LEFT, data.id, false);
                        },
                    });
                    $("#pr" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).parent().addClass("pressed");
                            padclick(RIGHT, data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).parent().removeClass("pressed");
                            padclick(RIGHT, data.id, false);
                        },
                    });
                    $("#pb" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).parent().addClass("pressed");
                            padclick(DOWN, data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).parent().removeClass("pressed");
                            padclick(DOWN, data.id, false);
                        },
                    });
                    $("#pc" + data.id).on({
                        touchstart: function (e) {
                            e.preventDefault();
                            $(this).addClass("pressed");
                            padclick(CENTER, data.id, true);
                        },
                        touchend: function (e) {
                            e.preventDefault();
                            $(this).removeClass("pressed");
                            padclick(CENTER, data.id, false);
                        },
                    });
                }
                break;

            case UI_SLIDER:
                //https://codepen.io/seanstopnik/pen/CeLqA
                if (data.visible) {
                    addToHTML(data);
                    rangeSlider(!sliderContinuous);
                }
                break;

            case UI_TAB:
                if (data.visible) {
                    $("#tabsnav").append(
                        "<li><a id='tablink" + data.id + "' href='#tab" + data.id + "'>" + data.value + "</a></li>"
                    );
                    $("#tabscontent").append("<div id='tab" + data.id + "'></div>");

                    // Add touch and click handlers for tab
                    // Note: Do NOT use e.preventDefault() in touchend - it blocks the click
                    // event which tabbedcontent.js needs to actually switch tabs
                    $("#tablink" + data.id).on({
                        touchend: function(e) {
                            tabclick(data.id);
                        },
                        mouseup: function(e) {
                            tabclick(data.id);
                        }
                    });

                    tabs = $(".tabscontent").tabbedContent({ loop: true }).data("api");
                    // switch to tab...
                    $("a")
                        .filter(function () {
                            return $(this).attr("href") === "#click-to-switch";
                        })
                        .on("click", function (e) {
                            var tab = prompt("Tab to switch to (number or id)?");
                            if (!tabs.switchTab(tab)) {
                                alert("That tab does not exist :\\");
                            }
                            e.preventDefault();
                        });
                }
                break;

            case UI_OPTION:
                if (data.parentControl) {
                    var parent = $("#select" + data.parentControl);
                    parent.append(
                        "<option id='option" +
                        data.id +
                        "' value='" +
                        data.value +
                        "' " +
                        data.selected +
                        ">" +
                        data.label +
                        "</option>"
                    );
                }
                break;

            case UI_MIN:
                if (data.parentControl) {
                    //Is it applied to a slider?
                    if ($('#sl' + data.parentControl).length) {
                        $('#sl' + data.parentControl).attr("min", data.value);
                    } else if ($('#num' + data.parentControl).length) {
                        //Or a number
                        $('#num' + data.parentControl).attr("min", data.value);
                    }
                }
                break;

            case UI_MAX:
                if (data.parentControl) {
                    //Is it applied to a slider?
                    if ($('#sl' + data.parentControl).length) {
                        $('#sl' + data.parentControl).attr("max", data.value);
                    } else if ($('#text' + data.parentControl).length) {
                        //Is it a text element
                        $('#text' + data.parentControl).attr("maxlength", data.value);
                    } else if ($('#num' + data.parentControl).length) {
                        //Or a number
                        $('#num' + data.parentControl).attr("max", data.value);
                    }
                }
                break;

            case UI_STEP:
                if (data.parentControl) {
                    //Is it applied to a slider?
                    if ($('#sl' + data.parentControl).length) {
                        $('#sl' + data.parentControl).attr("step", data.value);
                    } else if ($('#num' + data.parentControl).length) {
                        //Or a number
                        $('#num' + data.parentControl).attr("step", data.value);
                    }
                }
                break;

            case UI_GRAPH:
                if (data.visible) {
                    addToHTML(data);
                    graphData[data.id] = restoreGraphData(data.id);
                    renderGraphSvg(graphData[data.id], "graph" + data.id);
                }
                break;
            case ADD_GRAPH_POINT:
                var ts = new Date().getTime();
                graphData[data.id].push({ x: ts, y: data.value });
                saveGraphData();
                renderGraphSvg(graphData[data.id], "graph" + data.id);
                break;
            case CLEAR_GRAPH:
                graphData[data.id] = [];
                saveGraphData();
                renderGraphSvg(graphData[data.id], "graph" + data.id);
                break;

            case UI_ACCEL:
                if (hasAccel) break;
                hasAccel = true;
                if (data.visible) {
                    addToHTML(data);
                    requestOrientationPermission();
                }
                break;

            case UI_FILEDISPLAY:
                if (data.visible)
                {
                    addToHTML(data);
                    FileDisplayUploadFile(data);
                }
                break;
    
            /*
             * Update messages change the value/style of a component without adding new HTML
             */
            case UPDATE_LABEL:
                $("#l" + data.id).html(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#l" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_SWITCHER:
                switcher(data.id, data.value == "0" ? 0 : 1);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#sl" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_SLIDER:
                $("#sl" + data.id).attr("value", data.value)
                slider_move($("#sl" + data.id).parent().parent(), data.value, "100", false);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#sl" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_NUMBER:
                $("#num" + data.id).val(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#num" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_TEXT_INPUT:
                $("#text" + data.id).val(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#text" + data.id).attr("style", data.elementStyle);
                }
                if (data.hasOwnProperty('inputType')) {
                    $("#text" + data.id).attr("type", data.inputType);
                }
                break;

            case UPDATE_SELECT:
                $("#select" + data.id).val(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#select" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_BUTTON:
                $("#btn" + data.id).val(data.value);
                $("#btn" + data.id).text(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#btn" + data.id).attr("style", data.elementStyle);
                }
                break;

            case UPDATE_PAD:
            case UPDATE_CPAD:
                break;
            case UPDATE_GAUGE:
                $("#gauge" + data.id).val(data.value);
                if (data.hasOwnProperty('elementStyle')) {
                    $("#gauge" + data.id).attr("style", data.elementStyle);
                }
                break;
            case UPDATE_ACCEL:
                break;

            case UPDATE_TIME:
                var rv = new Date().toISOString();
                websock.send("time:" + rv + ":" + data.id);
                break;

            case UPDATE_FILEDISPLAY:
                FileDisplayUploadFile(data);
                break;
        
            case UI_FRAGMENT:
                // console.info("Starting Fragment Processing");
                let FragmentLen = data.length;
                let FragementOffset = data.offset;
                let NextFragmentOffset = FragementOffset + FragmentLen;
                let Total = data.total;
                let Arrived = (FragmentLen + FragementOffset);
                let FragmentFinal = Total === Arrived;
                // console.info("UI_FRAGMENT:FragmentLen        '" + FragmentLen + "'");
                // console.info("UI_FRAGMENT:FragementOffset    '" + FragementOffset + "'");
                // console.info("UI_FRAGMENT:NextFragmentOffset '" + NextFragmentOffset + "'");
                // console.info("UI_FRAGMENT:Total              '" + Total + "'");
                // console.info("UI_FRAGMENT:Arrived            '" + Arrived + "'");
                // console.info("UI_FRAGMENT:FragmentFinal      '" + FragmentFinal + "'");

                if (!data.hasOwnProperty('control')) 
                {
                    console.error("UI_FRAGMENT:Missing control record, skipping control");
                    // console.info("Done Fragment Processing");
                    break;
                }
                let control = data.control;
                StopFragmentAssemblyTimer(data.control.id);
                
                // is this the first fragment?
                if(0 === FragementOffset)
                {
                    // console.info("Found first fragment");
                    controlAssemblyArray[control.id] = data;
                    // console.info("Value: " + controlAssemblyArray[control.id].control.value);
                    controlAssemblyArray[control.id].offset = NextFragmentOffset;
                    StartFragmentAssemblyTimer(control.id);
                    let TotalRequest = JSON.stringify({ 'id' : control.id, 'offset' : NextFragmentOffset });
                    websock.send("uifragmentok:" + 0 + ": " + TotalRequest + ":");
                    // console.info("asked for fragment " + TotalRequest);
                    // console.info("Done Fragment Processing");
                    break;
                }

                // not first fragment. are we assembling this control?
                if("undefined" === typeof controlAssemblyArray[control.id])
                {
                    // it looks like we missed the first fragment. Start the control over
                    console.error("Missing first fragment for control: " + control.id);
                    StartFragmentAssemblyTimer(control.id);
                    let TotalRequest = JSON.stringify({ 'id' : control.id, 'offset' : 0 });
                    websock.send("uifragmentok:" + 0 + ": " + TotalRequest + ":");
                    // console.info("asked for fragment " + TotalRequest);
                    // console.info("Done Fragment Processing");
                    break;
                }
                
                // is this the expected next fragment
                if(FragementOffset !== controlAssemblyArray[control.id].offset)
                {
                    console.error("Wrong next fragment. Expected: " + controlAssemblyArray[control.id].offset + " Got: " + FragementOffset);
                    StartFragmentAssemblyTimer(control.id);
                    let TotalRequest = JSON.stringify({ 'id' : control.id, 'offset' : controlAssemblyArray[control.id].length + controlAssemblyArray[control.id].offset });
                    websock.send("uifragmentok:" + 0 + ": " + TotalRequest + ":");
                    // console.info("asked for the expected fragment: " + TotalRequest);
                    // console.info("Done Fragment Processing");
                    break;
                }

                // console.info("Add to existing fragment");
                controlAssemblyArray[control.id].control.value += control.value;
                controlAssemblyArray[control.id].offset = NextFragmentOffset;
                // console.info("Value: " + controlAssemblyArray[control.id].control.value);

                if(true === FragmentFinal)
                {
                    var fauxEvent = {
                        data: JSON.stringify(controlAssemblyArray[control.id].control),
                    };
                    handleEvent(fauxEvent);
                    controlAssemblyArray[control.id] = null;
                    // console.info("Found last fragment");
                }
                else
                {
                    // console.info("Ask for next fragment.");
                    StartFragmentAssemblyTimer(control.id);
                    let TotalRequest = JSON.stringify({ 'id' : control.id, 'offset' : NextFragmentOffset});
                    websock.send("uifragmentok:" + 0 + ": " + TotalRequest + ":");
                    // console.info("asked for the next fragment: " + TotalRequest);
                }
                // console.info("Done Fragment Processing");
                break;

            default:
                console.error("Unknown type or event");
                break;
        }

        if (data.type >= UI_TITEL && data.type < UPDATE_OFFSET) {
            //A UI element was just added to the DOM
            processEnabled(data);
        }

        if (data.type >= UPDATE_OFFSET && data.type < UI_INITIAL_GUI) {
            //An "update" message was just recieved and processed
            var element = $("#id" + data.id);

            if (data.hasOwnProperty('panelStyle')) {
                $("#id" + data.id).attr("style", data.panelStyle);
            }

            if (data.hasOwnProperty('visible')) {
                if (data['visible'])
                    $("#id" + data.id).show();
                else
                    $("#id" + data.id).hide();
            }

            if (data.hasOwnProperty('panelClass')) {
                var element = $("#id" + data.id);
                var baseClass = element.attr("data-base-class");
                if (baseClass) {
                    element.attr("class", baseClass + " " + data.panelClass);
                }
            }

            if (data.type == UPDATE_SLIDER) {
                element.removeClass(
                    "slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin"
                );
                element.addClass("slider-" + colorClass(data.color));
            } else {
                element.removeClass(
                    "turquoise emerald peterriver wetasphalt sunflower carrot alizarin"
                );
                element.addClass(colorClass(data.color));
            }

            processEnabled(data);
        }

        $(".range-slider__range").each(function () {
            $(this)[0].value = $(this).attr("value");
            $(this).next().html($(this).attr("value"));
        });
    };

    websock.onmessage = handleEvent;
}

async function FileDisplayUploadFile(data)
{
    let text = await downloadFile(data.value);
    let ItemToUpdateId = "fd" + data.id;
    // console.info("ItemToUpdateId: " + ItemToUpdateId);
    // console.info("          text: " + text);
    // populate the text object
    $("#" + ItemToUpdateId).val(text);
    $("#" + ItemToUpdateId).css("textAlign", "left");
    $("#" + ItemToUpdateId).css("white-space", "nowrap");
    $("#" + ItemToUpdateId).css("overflow", "scroll");
    $("#" + ItemToUpdateId).css("overflow-y", "scroll");
    $("#" + ItemToUpdateId).css("overflow-x", "scroll");
    $("#" + ItemToUpdateId).scrollTop($("#" + ItemToUpdateId).val().length);

    // scroll the page to the updated control
    // $("#" + ItemToUpdateId).focus();

} // FileDisplayUploadFile

async function downloadFile(filename)
{
    let response = await fetch(filename);
		
	if(response.status != 200) {
		throw new Error("File Read Server Error: '" + response.status + "'");
	}
		
	// read response stream as text
	let text_data = await response.text();

	return text_data;
} // downloadFile

function StartFragmentAssemblyTimer(Id)
{
    StopFragmentAssemblyTimer(Id);
    FragmentAssemblyTimer[Id] = setInterval(function(_Id)
    {
        // does the fragment assembly still exist?
        if("undefined" !== typeof controlAssemblyArray[_Id])
        {
            if(null !== controlAssemblyArray[_Id])
            {
                // we have a valid control that is being assembled
                // ask for the next part
                let TotalRequest = JSON.stringify({ 'id' : controlAssemblyArray[_Id].control.id, 'offset' : controlAssemblyArray[_Id].offset});
                websock.send("uifragmentok:" + 0 + ": " + TotalRequest + ":");
            }
        }
    }, 1000, Id);
}

function StopFragmentAssemblyTimer(Id)
{
    if("undefined" !== typeof FragmentAssemblyTimer[Id])
    {
        if(FragmentAssemblyTimer[Id])
        {
            clearInterval(FragmentAssemblyTimer[Id]);
        }
    }
}

function sliderchange(number) {
    var val = $("#sl" + number).val();
    websock.send("slvalue:" + val + ":" + number);

    $(".range-slider__range").each(function () {
        $(this).attr("value", $(this)[0].value);
    });
}

function numberchange(number) {
    var val = $("#num" + number).val();
    websock.send("nvalue:" + val + ":" + number);
}

function textchange(number) {
    var val = $("#text" + number).val();
    websock.send("tvalue:" + val + ":" + number);
}

function tabclick(number) {
    var val = $("#tab" + number).val();
    websock.send("tabvalue:" + val + ":" + number);
}

function selectchange(number) {
    var val = $("#select" + number).val();
    websock.send("svalue:" + val + ":" + number);
}

function buttonclick(number, isdown) {
    if ($("#btn" + number).prop("disabled")) {
        return;
    }
    if (isdown) websock.send("bdown:" + number);
    else websock.send("bup:" + number);
}

function padclick(type, number, isdown) {
    if ($("#id" + number + " nav").hasClass("disabled")) {
        return;
    }
    switch (type) {
        case CENTER:
            if (isdown) websock.send("pcdown:" + number);
            else websock.send("pcup:" + number);
            break;
        case UP:
            if (isdown) websock.send("pfdown:" + number);
            else websock.send("pfup:" + number);
            break;
        case DOWN:
            if (isdown) websock.send("pbdown:" + number);
            else websock.send("pbup:" + number);
            break;
        case LEFT:
            if (isdown) websock.send("pldown:" + number);
            else websock.send("plup:" + number);
            break;
        case RIGHT:
            if (isdown) websock.send("prdown:" + number);
            else websock.send("prup:" + number);
            break;
    }
}

function switcher(number, state) {
    if (state == null) {
        if (!$("#sl" + number).hasClass("checked")) {
            websock.send("sactive:" + number);
            $("#sl" + number).addClass("checked");
        } else {
            websock.send("sinactive:" + number);
            $("#sl" + number).removeClass("checked");
        }
    } else if (state == 1) {
        $("#sl" + number).addClass("checked");
        $("#sl" + number).prop("checked", true);
    } else if (state == 0) {
        $("#sl" + number).removeClass("checked");
        $("#sl" + number).prop("checked", false);
    }
}

var rangeSlider = function (isDiscrete) {
    var range = $(".range-slider__range");
    var slidercb = function () {
        sliderchange($(this).attr("id").replace(/^\D+/g, ""));
    };

    range.on({
        input: function () {
            $(this).next().html(this.value)
        }
    });

    range.each(function () {
        $(this).next().html(this.value);
        if ($(this).attr("callbackSet") != "true") {
            if (!isDiscrete) {
                $(this).on({ input: slidercb }); //input fires when dragging
            } else {
                $(this).on({ change: slidercb }); //change fires only once released
            }
            $(this).attr("callbackSet", "true");
        }
    });
};


var addToHTML = function (data) {
    panelStyle = data.hasOwnProperty('panelStyle') ? " style='" + data.panelStyle + "' " : "";
    panelClass = data.hasOwnProperty('panelClass') ? " " + data.panelClass + " " : "";
    panelwide = data.hasOwnProperty('wide') ? "wide" : "";

    if (!data.hasOwnProperty('parentControl') || $("#tab" + data.parentControl).length > 0) {
        //We add the control with its own panel
        var parent = data.hasOwnProperty('parentControl') ?
            $("#tab" + data.parentControl) :
            $("#row");

        var html = "";
        switch (data.type) {
            case UI_LABEL:
            case UI_BUTTON:
            case UI_SWITCHER:
            case UI_CPAD:
            case UI_PAD:
            case UI_SLIDER:
            case UI_NUMBER:
            case UI_TEXT_INPUT:
            case UI_SELECT:
            case UI_GRAPH:
            case UI_GAUGE:
            case UI_ACCEL:
            case UI_FILEDISPLAY:
                var baseClass = "two columns " + panelwide + " card tcenter " + colorClass(data.color);
                html = "<div id='id" + data.id + "' " + panelStyle + " class='" + baseClass + panelClass + "' data-base-class='" + baseClass + "'><h5>" + data.label + "</h5><hr/>" +
                    elementHTML(data) +
                    "</div>";
                break;

            case UI_SEPARATOR:
                var baseClass = "sectionbreak columns";
                html = "<div id='id" + data.id + "' " + panelStyle + " class='" + baseClass + panelClass + "' data-base-class='" + baseClass + "'>" +
                    "<h5>" + data.label + "</h5><hr/></div>";
                break;
            case UI_TIME:
                //Invisible element
                break;
        }

        parent.append(html);

    } else {
        //We are adding to an existing panel so we only need the HTML for the element
        var parent = $("#id" + data.parentControl);
        parent.append(elementHTML(data));
    }
}

var elementHTML = function (data) {
    var id = data.id
    var elementStyle = data.hasOwnProperty('elementStyle') ? " style='" + data.elementStyle + "' " : "";
    var inputType = data.hasOwnProperty('inputType') ? " type='" + data.inputType + "' " : "";
    switch (data.type) {
        case UI_LABEL:
            return "<span id='l" + id + "' " + elementStyle +
                " class='label label-wrap'>" + data.value + "</span>";
        case UI_FILEDISPLAY:
            return "<textarea id='fd" + id + "' rows='4' " + elementStyle +
                " class='label label-wrap'>" + "</textarea>";
        case UI_BUTTON:
            return "<button id='btn" + id + "' " + elementStyle +
                " onmousedown='buttonclick(" + id + ", true)'" +
                " onmouseup='buttonclick(" + id + ", false)'>" +
                data.value + "</button>";
        case UI_SWITCHER:
            return "<label id='sl" + id + "' " + elementStyle +
                " class='switch " + (data.value == "1" ? "checked" : "") +
                (data.hasOwnProperty('vertical') ? " vert-switcher " : "") +
                "'>" +
                "<div class='in'>" +
                "<input type='checkbox' id='s" + id + "' onClick='switcher(" + id + ",null)' " +
                (data.value == "1" ? "checked" : "") + "/></div></label>";
        case UI_CPAD:
        case UI_PAD:
            return "<nav class='control'><ul>" +
                "<li><a onmousedown='padclick(UP, " + id + ", true)' " +
                "onmouseup='padclick(UP, " + id + ", false)' id='pf" + id + "'>&#9650;</a></li>" +
                "<li><a onmousedown='padclick(RIGHT, " + id + ", true)' " +
                "onmouseup='padclick(RIGHT, " + id + ", false)' id='pr" + id + "'>&#9650;</a></li>" +
                "<li><a onmousedown='padclick(LEFT, " + id + ", true)' " +
                "onmouseup='padclick(LEFT, " + id + ", false)' id='pl" + id + "'>&#9650;</a></li>" +
                "<li><a onmousedown='padclick(DOWN, " + id + ", true)' " +
                "onmouseup='padclick(DOWN, " + id + ", false)' id='pb" + id + "'>&#9650;</a></li>" +
                "</ul>" +
                (data.type == UI_CPAD
                    ? "<a class='confirm' onmousedown='padclick(CENTER," + id + ", true)' " +
                    "onmouseup='padclick(CENTER, " + id + ", false)' id='pc" + id + "'>OK</a>"
                    : "") +
                "</nav>";
        case UI_SLIDER:
            return "<div class='range-slider " +
                (data.hasOwnProperty('vertical') ? " vert-slider " : "") +
                "'>" +
                "<input id='sl" + id + "' type='range' min='0' max='100' value='" + data.value + "' " +
                elementStyle + " class='range-slider__range'><span class='range-slider__value'>" +
                data.value + "</span></div>";
        case UI_NUMBER:
            return "<input style='color:black; " + data.elementStyle + "' id='num" + id +
                "' type='number' value='" + data.value + "' onchange='numberchange(" + id + ")' />";
        case UI_TEXT_INPUT:
            return "<input " + inputType + "style='color:black; " + data.elementStyle + "' id='text" + id +
                "' value='" + data.value + "' onchange='textchange(" + id + ")' />";
        case UI_SELECT:
            return "<select style='color:black; " + data.elementStyle + "' id='select" + id +
                "' onchange='selectchange(" + id + ")' />";
        case UI_GRAPH:
            return "<figure id='graph" + id + "'><figcaption>" + data.label + "</figcaption></figure>";
        case UI_GAUGE:
            return "WILL BE A GAUGE <input style='color:black;' id='gauge" + id +
                "' type='number' value='" + data.value + "' onchange='numberchange(" + id + ")' />";
        case UI_ACCEL:
            return "ACCEL // Not implemented fully!<div class='accelerometer' id='accel" + id +
                "' ><div class='ball" + id + "'></div><pre class='accelerometeroutput" + id + "'></pre>";
        default:
            return "";
    }
}

var processEnabled = function (data) {
    //Handle the enabling and disabling of controls
    //Most controls can be disabled through the use of $("#<item>").prop("disabled", true) and CSS will style it accordingly
    //The switcher and pads also require the addition of the "disabled" class
    switch (data.type) {
        case UI_SWITCHER:
        case UPDATE_SWITCHER:
            if (data.enabled) {
                $("#sl" + data.id).removeClass('disabled');
                $("#s" + data.id).prop("disabled", false);
            } else {
                $("#sl" + data.id).addClass('disabled');
                $("#s" + data.id).prop("disabled", true);
            }
            break;

        case UI_SLIDER:
        case UPDATE_SLIDER:
            $("#sl" + data.id).prop("disabled", !data.enabled);
            break;

        case UI_NUMBER:
        case UPDATE_NUMBER:
            $("#num" + data.id).prop("disabled", !data.enabled);
            break;

        case UI_TEXT_INPUT:
        case UPDATE_TEXT_INPUT:
            $("#text" + data.id).prop("disabled", !data.enabled);
            break;

        case UI_SELECT:
        case UPDATE_SELECT:
            $("#select" + data.id).prop("disabled", !data.enabled);
            break;

        case UI_BUTTON:
        case UPDATE_BUTTON:
            $("#btn" + data.id).prop("disabled", !data.enabled);
            break;

        case UI_PAD:
        case UI_CPAD:
        case UPDATE_PAD:
        case UPDATE_CPAD:
        case UI_FILEDISPLAY:
        case UPDATE_FILEDISPLAY:
            if (data.enabled) {
                $("#id" + data.id + " nav").removeClass('disabled');
            } else {
                $("#id" + data.id + " nav").addClass('disabled');
            }
            break;
    }
}
