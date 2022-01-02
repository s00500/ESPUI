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
const UPTDATE_ACCEL = 117;

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

    case C_NONE:
      return "dark";
    default:
      return "";
  }
}

var websock;
var websockConnected = false;

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
    return savedData[id];
  }
  return [];
}

function restart() {
  $(document).add("*").off();
  $("#row").html("");
  websock.close();
  start();
}

function conStatusError() {
  websockConnected = false;
  $("#conStatus").removeClass("color-green");
  $("#conStatus").addClass("color-red");
  $("#conStatus").html("Error / No Connection &#8635;");
  $("#conStatus").off();
  $("#conStatus").on({
    click: restart,
  });
}

function handleVisibilityChange() {
  if (!websockConnected && !document.hidden) {
    restart();
  }
}

function start() {
  document.addEventListener("visibilitychange", handleVisibilityChange, false);
  if (
    window.location.port != "" ||
    window.location.port != 80 ||
    window.location.port != 443
  ) {
    websock = new WebSocket(
      "ws://" + window.location.hostname + ":" + window.location.port + "/ws"
    );
  } else {
    websock = new WebSocket("ws://" + window.location.hostname + "/ws");
  }
  websock.onopen = function (evt) {
    console.log("websock open");
    $("#conStatus").addClass("color-green");
    $("#conStatus").text("Connected");
    websockConnected = true;
  };

  websock.onclose = function (evt) {
    console.log("websock close");
    conStatusError();
  };

  websock.onerror = function (evt) {
    console.log(evt);
    conStatusError();
  };

  var handleEvent = function (evt) {
    console.log(evt);
    var data = JSON.parse(evt.data);
    var e = document.body;
    var center = "";

    panelStyle = data.hasOwnProperty('panelStyle') ? " style='" + data.panelStyle + "'" : "";
    elementStyle = data.hasOwnProperty('elementStyle') ? " style='" + data.elementStyle + "'" : "";

    switch (data.type) {
      case UI_INITIAL_GUI:
        // Clear current elements
        $("#row").html("");
        $("#tabsnav").html("");
        $("#tabscontent").html("");

        if (data.sliderContinuous) {
          sliderContinuous = data.sliderContinuous;
        }
        data.controls.forEach(element => {
          var fauxEvent = {
            data: JSON.stringify(element),
          };
          handleEvent(fauxEvent);
        });
        break;

      case UI_EXTEND_GUI:
        data.controls.forEach(element => {
          var fauxEvent = {
            data: JSON.stringify(element),
          };
          handleEvent(fauxEvent);
        });
        break;
      
      case UI_RELOAD:
        window.location.reload();
        break;

      case UI_TITEL:
        document.title = data.label;
        $("#mainHeader").html(data.label);
        break;

      case UI_LABEL:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" + 
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<span id='l" +
            data.id +
            "' " + elementStyle + " class='label label-wrap'>" +
            data.value +
            "</span>" +
            "</div>"
        );
		}
        break;

      case UI_BUTTON:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='one columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<button id='btn" +
            data.id +
            "' " + elementStyle + " " +
            "onmousedown='buttonclick(" +
            data.id +
            ", true)' " +
            "onmouseup='buttonclick(" +
            data.id +
            ", false)'>" +
            data.value +
            "</button></div>"
        );
        $("#btn" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            buttonclick(data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            buttonclick(data.id, false);
          },
        });
		}
        break;

      case UI_SWITCHER:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='one columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<label id='sl" +
            data.id +
            "' " + elementStyle + " class='switch " +
            (data.value == "1" ? "checked" : "") +
            "'>" +
            "<div class='in'><input type='checkbox' id='s" +
            data.id +
            "' onClick='switcher(" +
            data.id +
            ",null)' " +
            (data.value == "1" ? "checked" : "") +
            "/></div>" +
            "</label>" +
            "</div>"
        );
        switcher(data.id, data.value);
		}
        break;

      case UI_CPAD:
      case UI_PAD:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<nav class='control'>" +
            "<ul>" +
            "<li><a onmousedown='padclick(UP, " +
            data.id +
            ", true)' onmouseup='padclick(UP, " +
            data.id +
            ", false)' id='pf" +
            data.id +
            "'>&#9650;</a></li>" +
            "<li><a onmousedown='padclick(RIGHT, " +
            data.id +
            ", true)' onmouseup='padclick(RIGHT, " +
            data.id +
            ", false)' id='pr" +
            data.id +
            "'>&#9650;</a></li>" +
            "<li><a onmousedown='padclick(LEFT, " +
            data.id +
            ", true)' onmouseup='padclick(LEFT, " +
            data.id +
            ", false)' id='pl" +
            data.id +
            "'>&#9650;</a></li>" +
            "<li><a onmousedown='padclick(DOWN, " +
            data.id +
            ", true)' onmouseup='padclick(DOWN, " +
            data.id +
            ", false)' id='pb" +
            data.id +
            "'>&#9650;</a></li>" +
            "</ul>" +
            (data.type == UI_CPAD
              ? "<a class='confirm' onmousedown='padclick(CENTER," +
                data.id +
                ", true)' onmouseup='padclick(CENTER, " +
                data.id +
                ", false)' id='pc" +
                data.id +
                "'>OK</a>"
              : "") +
            "</nav>" +
            "</div>"
        );

        $("#pf" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            padclick(UP, data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            padclick(UP, data.id, false);
          },
        });
        $("#pl" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            padclick(LEFT, data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            padclick(LEFT, data.id, false);
          },
        });
        $("#pr" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            padclick(RIGHT, data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            padclick(RIGHT, data.id, false);
          },
        });
        $("#pb" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            padclick(DOWN, data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            padclick(DOWN, data.id, false);
          },
        });
        $("#pc" + data.id).on({
          touchstart: function (e) {
            e.preventDefault();
            padclick(CENTER, data.id, true);
          },
          touchend: function (e) {
            e.preventDefault();
            padclick(CENTER, data.id, false);
          },
        });
		}
        break;

      //https://codepen.io/seanstopnik/pen/CeLqA
      case UI_SLIDER:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter card-slider " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<div class='range-slider'>" +
            "<input id='sl" +
            data.id +
            "' type='range' min='0' max='100' value='" +
            data.value +
            "' " + elementStyle + " class='range-slider__range'>" +
            "<span class='range-slider__value'>" +
            data.value +
            "</span>" +
            "</div>" +
            "</div>"
        );
        rangeSlider(!sliderContinuous);
		}
        break;

      case UI_NUMBER:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<input style='color:black;' " + elementStyle + " id='num" +
            data.id +
            "' type='number' value='" +
            data.value +
            "' onchange='numberchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
		}
        break;

      case UI_TEXT_INPUT:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<input style='color:black;' " + elementStyle + " id='text" +
            data.id +
            "' value='" +
            data.value +
            "' onchange='textchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
		}
        break;

      case UI_TAB:
		if (data.visible) {
        $("#tabsnav").append(
          "<li><a onmouseup='tabclick(" + data.id + ")' href='#tab" + data.id + "'>" + data.value + "</a></li>"
        );
        $("#tabscontent").append("<div id='tab" + data.id + "'></div>");

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

      case UI_SELECT:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<select style='color:black;' " + elementStyle + " id='select" +
            data.id +
            "' onchange='selectchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
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
          var parent = $("#id" + data.parentControl + " input");
          if (parent.size()) {
            parent.attr("min", data.value);
          }
        }
        break;

      case UI_MAX:
        if (data.parentControl) {
          var parent = $("#id" + data.parentControl + " input");
          if (parent.size()) {
            parent.attr("max", data.value);
          }
        }
        break;

      case UI_STEP:
        if (data.parentControl) {
          var parent = $("#id" + data.parentControl + " input");
          if (parent.size()) {
            parent.attr("step", data.value);
          }
        }
        break;
      case UI_GRAPH:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "<figure id='graph" +
            data.id +
            "'>" +
            "<figcaption>" +
            data.label +
            "</figcaption>" +
            "</figure>" +
            "</div>"
        );
        graphData[data.id] = restoreGraphData(data.id);
        renderGraphSvg(graphData[data.id], "graph" + data.id);
		}
        break;
      case ADD_GRAPH_POINT:
        var ts = Math.round(new Date().getTime() / 1000);
        graphData[data.id].push({ x: ts, y: data.value });
        saveGraphData();
        renderGraphSvg(graphData[data.id], "graph" + data.id);
        break;
      case CLEAR_GRAPH:
        graphData[data.id] = [];
        saveGraphData();
        renderGraphSvg(graphData[data.id], "graph" + data.id);
        break;
      case UI_GAUGE:
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "WILL BE A GAUGE <input style='color:black;' id='gauge" +
            data.id +
            "' type='number' value='" +
            data.value +
            "' onchange='numberchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
		}
        break;

      case UI_ACCEL:
        if (hasAccel) break;
        var parent;
        if (data.parentControl) {
          parent = $("#tab" + data.parentControl);
        } else {
          parent = $("#row");
        }
        hasAccel = true;
		if (data.visible) {
        parent.append(
          "<div id='id" +
            data.id +
            "' " + panelStyle + " class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5>" +
            data.label +
            "</h5><hr/>" +
            "ACCEL // Not implemented fully!<div class='accelerometer' id='accel" +
            data.id +
            "' ><div class='ball" +
            data.id +
            "'></div><pre class='accelerometeroutput" +
            data.id +
            "'></pre>" +
            "</div>"
        );

        requestOrientationPermission();
		}
        break;

      case UPDATE_LABEL:
        $("#l" + data.id).html(data.value);
        if(data.hasOwnProperty('elementStyle')) {
          $("#l" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_SWITCHER:
        switcher(data.id, data.value == "0" ? 0 : 1);
        if(data.hasOwnProperty('elementStyle')) {
          $("#sl" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_SLIDER:
        slider_move($("#id" + data.id), data.value, "100", false);
        if(data.hasOwnProperty('elementStyle')) {
          $("#sl" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_NUMBER:
        $("#num" + data.id).val(data.value);
        if(data.hasOwnProperty('elementStyle')) {
          $("#num" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_TEXT_INPUT:
        $("#text" + data.id).val(data.value);
        if(data.hasOwnProperty('elementStyle')) {
          $("#text" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_SELECT:
        $("#select" + data.id).val(data.value);
        if(data.hasOwnProperty('elementStyle')) {
          $("#select" + data.id).attr("style", data.elementStyle);
        }
        break;

      case UPDATE_BUTTON:
      case UPDATE_PAD:
      case UPDATE_CPAD:
        break;
      case UPDATE_GAUGE:
        $("#gauge" + data.id).val(data.value);
        if(data.hasOwnProperty('elementStyle')) {
          $("#gauge" + data.id).attr("style", data.elementStyle);
        }
        break;
      case UPDATE_ACCEL:
        break;

      default:
        console.error("Unknown type or event");
        break;
    }

    if (data.type >= UPDATE_OFFSET && data.type < UI_INITIAL_GUI) {
      var element = $("#id" + data.id);

      if(data.hasOwnProperty('panelStyle')) {
        $("#id" + data.id).attr("style", data.panelStyle);
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
    }

    $(".range-slider__range").each(function(){ 
      $(this)[0].value = $(this).attr("value");
      $(this).next().html($(this).attr("value"));
    });
  };

  websock.onmessage = handleEvent;
}

function sliderchange(number) {
  var val = $("#sl" + number).val();
  websock.send("slvalue:" + val + ":" + number);
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
  if (isdown) websock.send("bdown:" + number);
  else websock.send("bup:" + number);
}

function padclick(type, number, isdown) {
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
    if ($("#s" + number).is(":checked")) {
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
  var slidercb = function() {
    sliderchange($(this).attr("id").replace(/^\D+/g, ""));
  };

  range.on({input: function() {
    $(this).next().html(this.value)}
  });

  range.each(function() {
    $(this).next().html(this.value);
    if($(this).attr("callbackSet") != "true") {
      if (!isDiscrete) {
        $(this).on({input: slidercb}); //input fires when dragging
      } else {
        $(this).on({change: slidercb}); //change fires only once released
      }
      $(this).attr("callbackSet", "true");
    }
  });
};
