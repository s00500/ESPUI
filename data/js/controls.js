const UI_INITIAL_GUI = 200;

const UI_TITEL = 0;
const UI_BUTTON = 1;
const UI_PAD = 2;
const UI_CPAD = 3;

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

const FOR = 0;
const BACK = 1;
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

function restart() {
  $(document)
    .add("*")
    .off();
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
    click: restart
  });
}

function handleVisibilityChange() {
  if (!websockConnected && !document.hidden) {
    restart();
  }
}

function start() {
  document.addEventListener("visibilitychange", handleVisibilityChange, false);
  websock = new WebSocket("ws://" + window.location.hostname + "/ws");
  websock.onopen = function(evt) {
    console.log("websock open");
    $("#conStatus").addClass("color-green");
    $("#conStatus").text("Connected");
    websockConnected = true;
  };

  websock.onclose = function(evt) {
    console.log("websock close");
    conStatusError();
  };

  websock.onerror = function(evt) {
    console.log(evt);
    conStatusError();
  };

  var handleEvent = function(evt) {
    //console.log(evt);
    var data = JSON.parse(evt.data);
    var e = document.body;
    var center = "";
    switch (data.type) {
      case UI_INITIAL_GUI:
        data.controls.forEach(element => {
          var fauxEvent = {
            data: JSON.stringify(element)
          };
          handleEvent(fauxEvent);
        });
        break;
      case UI_TITEL:
        document.title = data.label;
        $("#mainHeader").html(data.label);
        break;
      case UI_LABEL:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter " +
            colorClass(data.color) +
            "'><h5 id='" +
            data.id +
            "'>" +
            data.label +
            "</h5><hr /><span id='l" +
            data.id +
            "' class='label label-wrap'>" +
            data.value +
            "</span></div>"
        );
        break;
      case UI_BUTTON:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='one columns card tcenter " +
            colorClass(data.color) +
            "'><h5>" +
            data.label +
            "</h5><hr/><button onmousedown='buttonclick(" +
            data.id +
            ", true)' onmouseup='buttonclick(" +
            data.id +
            ", false)' id='" +
            data.id +
            "'>" +
            data.value +
            "</button></div>"
        );
        $("#" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            buttonclick(data.id, true);
          }
        });
        $("#" + data.id).on({
          touchend: function(e) {
            e.preventDefault();
            buttonclick(data.id, false);
          }
        });
        break;
      case UI_SWITCHER:
        var label = "<label id='sl" + data.id + "' class='switch checked'>";
        var input =
          "<div class='in'><input type='checkbox' id='s" +
          data.id +
          "' onClick='switcher(" +
          data.id +
          ",null)' checked></div>";
        if (data.value == "0") {
          label = "<label id='sl" + data.id + "' class='switch'>";
          input =
            "<div class='in'><input type='checkbox' id='s" +
            data.id +
            "' onClick='switcher(" +
            data.id +
            ",null)' ></div>";
        }
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div id='" +
            data.id +
            "' class='one columns card tcenter " +
            colorClass(data.color) +
            "'><h5>" +
            data.label +
            "</h5><hr/>" +
            label +
            input +
            "</label>" +
            "</div>"
        );
        break;
      case UI_CPAD:
        center =
          "<a class='confirm' onmousedown='padclick(CENTER, " +
          data.id +
          ", true)' onmouseup='padclick(CENTER, " +
          data.id +
          ", false)' href='#' id='pc" +
          data.id +
          "'>OK</a>";
      //NO BREAK
      case UI_PAD:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter " +
            colorClass(data.color) +
            "'><h5>" +
            data.label +
            "</h5><hr/>" +
            "<nav class='control'>" +
            "<ul>" +
            "<li><a onmousedown='padclick(FOR, " +
            data.id +
            ", true)' onmouseup='padclick(FOR, " +
            data.id +
            ", false)' href='#' id='pf" +
            data.id +
            "'>▲</a></li>" +
            "<li><a onmousedown='padclick(RIGHT, " +
            data.id +
            ", true)' onmouseup='padclick(RIGHT, " +
            data.id +
            ", false)' href='#' id='pr" +
            data.id +
            "'>▲</a></li>" +
            "<li><a onmousedown='padclick(LEFT, " +
            data.id +
            ", true)' onmouseup='padclick(LEFT, " +
            data.id +
            ", false)' href='#' id='pl" +
            data.id +
            "'>▲</a></li>" +
            "<li><a onmousedown='padclick(BACK, " +
            data.id +
            ", true)' onmouseup='padclick(BACK, " +
            data.id +
            ", false)' href='#' id='pb" +
            data.id +
            "'>▲</a></li>" +
            "</ul>" +
            center +
            "</nav>" +
            "</div>"
        );

        $("#pf" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            padclick(FOR, data.id, true);
          },
          touchend: function(e) {
            e.preventDefault();
            padclick(FOR, data.id, false);
          }
        });
        $("#pl" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            padclick(LEFT, data.id, true);
          },
          touchend: function(e) {
            e.preventDefault();
            padclick(LEFT, data.id, false);
          }
        });
        $("#pr" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            padclick(RIGHT, data.id, true);
          },
          touchend: function(e) {
            e.preventDefault();
            padclick(RIGHT, data.id, false);
          }
        });
        $("#pb" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            padclick(BACK, data.id, true);
          },
          touchend: function(e) {
            e.preventDefault();
            padclick(BACK, data.id, false);
          }
        });
        $("#pc" + data.id).on({
          touchstart: function(e) {
            e.preventDefault();
            padclick(CENTER, data.id, true);
          },
          touchend: function(e) {
            e.preventDefault();
            padclick(CENTER, data.id, false);
          }
        });

        break;
      case UPDATE_LABEL:
        $("#l" + data.id).html(data.value);
        break;
      case UPDATE_SWITCHER:
        if (data.value == "0") switcher(data.id, 0);
        else switcher(data.id, 1);
        break;
      case UI_SLIDER:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter card-slider " +
            colorClass(data.color) +
            "'>" +
            "<h5 id='" +
            data.id +
            "'>" +
            data.label +
            "</h5><hr />" +
            "<div id='sl" +
            data.id +
            "' class='rkmd-slider slider-discrete slider-" +
            colorClass(data.color) +
            "'>" +
            "<input type='range' min='0' max='100' value='" +
            data.value +
            "'>" +
            "</div>" +
            "</div>"
        );
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<script>" + "rkmd_rangeSlider('#sl" + data.id + "');" + "</script>"
        );
        break;

      case UPDATE_SLIDER:
        slider_move($("#sl" + data.id), data.value, "100", false);
        break;

      case UI_NUMBER:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5 id='" +
            data.id +
            "'>" +
            data.label +
            "</h5><hr />" +
            "<input style='color:black;' id='num" +
            data.id +
            "' type='number' value='" +
            data.value +
            "' onchange='numberchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
        break;

      case UPDATE_NUMBER:
        $("#num" + data.id).val(data.value);
        break;

      case UI_TEXT_INPUT:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5 id='" +
            data.id +
            "'>" +
            data.label +
            "</h5><hr />" +
            "<input style='color:black;' id='text" +
            data.id +
            "' value='" +
            data.value +
            "' onchange='textchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
        break;

      case UPDATE_TEXT_INPUT:
        $("#text" + data.id).val(data.value);
        break;
        
      case UI_TAB:
        $("#tabsnav").append(
          "<li><a href='#tab" + data.id + "'>" + data.value + "</a></li>"
        );
        $("#tabscontent").append(
          "<div id='tab" + data.id + "'></div>"
        );
        
        tabs = $('.tabscontent').tabbedContent({loop: true}).data('api');
            // switch to tab...
            $('a').filter(function() {
                return $(this).attr('href') === '#click-to-switch';
            }).on('click', function(e) {
                var tab = prompt('Tab to switch to (number or id)?');
                if (!tabs.switchTab(tab)) {
                    alert('That tab does not exist :\\');
                }
                e.preventDefault();
            });
        break;
        
      case UI_SELECT:
        var element;
        if(data.parentControl) {
          element = $("#tab"+data.parentControl);
        } else {
          element = $("#row")
        }
        element.append(
          "<div class='two columns card tcenter " +
            colorClass(data.color) +
            "'>" +
            "<h5 id='" +
            data.id +
            "'>" +
            data.label +
            "</h5><hr />" +
            "<select style='color:black;' id='select" +
            data.id +
            "' onchange='selectchange(" +
            data.id +
            ")' />" +
            "</div>"
        );
        break;
        
      case UI_OPTION:
        if(data.parentControl) {
          var element = $("#select"+data.parentControl);
          element.append(
            "<option id='option" + data.id + "' value='" + data.value + "'>" + data.label + "</option>"
          );
        }
        break;
        
      case UPDATE_SELECT:
        $("#select" + data.id).val(data.value);
        break;

      default:
        console.error("Unknown type or event");
        break;
    }
  };

  websock.onmessage = handleEvent;
}

function numberchange(number) {
  var val = $("#num" + number).val();
  websock.send("nvalue:" + val + ":" + number);
  console.log(val);
}

function textchange(number) {
  var val = $("#text" + number).val();
  websock.send("tvalue:" + val + ":" + number);
  console.log("tvalue:" + val + ":" + number);
  console.log(val);
}

function selectchange(number) {
  var val = $("#select" + number).val();
  websock.send("svalue:" + val + ":" + number);
  console.log("svalue:" + val + ":" + number);
  console.log(val);
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
    case FOR:
      if (isdown) websock.send("pfdown:" + number);
      else websock.send("pfup:" + number);
      break;
    case BACK:
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
