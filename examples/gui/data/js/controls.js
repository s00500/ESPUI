const UI_TITEL = 0;
const UI_LABEL = 1;
const UI_BUTTON = 2;
const UI_SWITCHER = 3;
const UI_PAD = 4;
const UI_CPAD = 5;
const UPDATE_LABEL = 6;
const UPDATE_SWITCHER = 7;
const UI_SLIDER = 8;
const UPDATE_SLIDER = 9;

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
const C_NONE = 7;

function colorClass(colorId) {
    colorId = Number(colorId);
    switch (colorId) {
        case C_TURQUOISE:
            return "turquoise";
            break;

        case C_EMERALD:
            return "emerald";
            break;

        case C_PETERRIVER:
            return "peterriver";
            break;

        case C_WETASPHALT:
            return "wetasphalt";
            break;

        case C_SUNFLOWER:
            return "sunflower";
            break;

        case C_CARROT:
            return "carrot"
            break;

        case C_ALIZARIN:
            return "alizarin"
            break;

        case C_NONE:
            return ""
            break;

        default:
            return "";
    }
}

var websock;

function start() {
    websock = new WebSocket('ws://' + window.location.hostname + '/ws');
    websock.onopen = function(evt) {
        console.log('websock open');
        $("#conStatus").addClass("color-green");
        $("#conStatus").text("Connected");
    };
    websock.onclose = function(evt) {
        console.log('websock close');
        $("#conStatus").removeClass("color-green");
        $("#conStatus").addClass("color-red");
        $("#conStatus").text("Error / No Connection");
    };
    websock.onerror = function(evt) {
        console.log(evt);
        $("#conStatus").removeClass("color-green");
        $("#conStatus").addClass("color-red");
        $("#conStatus").text("Error / No Connection");
    };
    websock.onmessage = function(evt) {
        console.log(evt);
        var data = JSON.parse(evt.data);
        var e = document.body;
        var center = "";
        switch (data.type) {
            case UI_TITEL:
                document.title = data.label;
                $('#mainHeader').html(data.label);
                break;
            case UI_LABEL:
                $('#row').append("<div class='two columns card tcenter " + colorClass(data.color) + "'><h5 id='" + data.id + "'>" + data.label + "</h5><hr /><span id='l" + data.id + "' class='label'>" + data.value + "</span></div>");
                break;
            case UI_BUTTON:
                $('#row').append("<div class='one columns card tcenter " + colorClass(data.color) + "'><h5>" + data.label + "</h5><hr/><button onmousedown='buttonclick(" + data.id + ", true)' onmouseup='buttonclick(" + data.id + ", false)' id='" + data.id + "'>" + data.value + "</button></div>");
                $('#' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        buttonclick(data.id, true)
                    }
                });
                $('#' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        buttonclick(data.id, false)
                    }
                });
                break;
            case UI_SWITCHER:
                var label = "<label id='sl" + data.id + "' class='switch checked'>";
                var input = "<input type='checkbox' id='s" + data.id + "' onClick='switcher(" + data.id + ",null)' checked>";
                if (data.value == "0") {
                    label = "<label id='sl" + data.id + "' class='switch'>";
                    input = "<input type='checkbox' id='s" + data.id + "' onClick='switcher(" + data.id + ",null)' >";
                }
                $('#row').append(
                    "<div id='" + data.id + "' class='one columns card tcenter " + colorClass(data.color) + "'><h5>" + data.label + "</h5><hr/>" +
                    label + "<i class='icon-ok'></i>" +
                    "<i class='icon-remove'></i>" + input +
                    "</label>" +
                    "</div>");
                break;
            case UI_CPAD:
                center = "<a class='confirm' onmousedown='padclick(CENTER, " + data.id + ", true)' onmouseup='padclick(CENTER, " + data.id + ", false)' href='#' id='pc" + data.id + "'>OK</a>";
                //NO BREAK
            case UI_PAD:
                $('#row').append(
                    "<div class='two columns card tcenter " + colorClass(data.color) + "'><h5>" + data.label + "</h5><hr/>" +
                    "<nav class='control'>" +
                    "<ul>" +
                    "<li><a onmousedown='padclick(FOR, " + data.id + ", true)' onmouseup='padclick(FOR, " + data.id + ", false)' href='#' id='pf" + data.id + "'>▲</a></li>" +
                    "<li><a onmousedown='padclick(RIGHT, " + data.id + ", true)' onmouseup='padclick(RIGHT, " + data.id + ", false)' href='#' id='pr" + data.id + "'>▲</a></li>" +
                    "<li><a onmousedown='padclick(LEFT, " + data.id + ", true)' onmouseup='padclick(LEFT, " + data.id + ", false)' href='#' id='pl" + data.id + "'>▲</a></li>" +
                    "<li><a onmousedown='padclick(BACK, " + data.id + ", true)' onmouseup='padclick(BACK, " + data.id + ", false)' href='#' id='pb" + data.id + "'>▲</a></li>" +
                    "</ul>" +
                    center +
                    "</nav>" +
                    "</div>");

                $('#pf' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        padclick(FOR, data.id, true)
                    }
                });
                $('#pf' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        padclick(FOR, data.id, false)
                    }
                });
                $('#pl' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        padclick(LEFT, data.id, true)
                    }
                });
                $('#pl' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        padclick(LEFT, data.id, false)
                    }
                });
                $('#pr' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        padclick(RIGHT, data.id, true)
                    }
                });
                $('#pr' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        padclick(RIGHT, data.id, false)
                    }
                });
                $('#pb' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        padclick(BACK, data.id, true)
                    }
                });
                $('#pb' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        padclick(BACK, data.id, false)
                    }
                });
                $('#pc' + data.id).on({
                    'touchstart': function(e) {
                        e.preventDefault();
                        padclick(CENTER, data.id, true)
                    }
                });
                $('#pc' + data.id).on({
                    'touchend': function(e) {
                        e.preventDefault();
                        padclick(CENTER, data.id, false)
                    }
                });

                break;
            case UPDATE_LABEL:
                $('#l' + data.id).html(data.value);
                break;
            case UPDATE_SWITCHER:
                if (data.value == "0")
                    switcher(data.id, 0);
                else
                    switcher(data.id, 1);
                break;
            case UI_SLIDER:
                $('#row').append(
                    "<div class='two columns card tcenter card-slider " + colorClass(data.color) + "'>" +
                    "<h5 id='" + data.id + "'>" + data.label + "</h5><hr />" +
                    "<div id='sl" + data.id + "' class='rkmd-slider slider-discrete slider-" + colorClass(data.color) + "'>" +
                    "<input type='range' min='0' max='100' value='" + data.value + "'>" +
                    "</div>" +
                    "</div>"
                );
                $('#row').append(
                  "<script>" +
                  "rkmd_rangeSlider('#sl" + data.id + "');" +
                  "</script>"
                );
                break;

            case UPDATE_SLIDER:
                slider_move($('#sl'+data.id), data.value ,'100', false);
                break;
            default:
                console.error('Unknown type or event');
                break;
        }
    };
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
        if ($('#s' + number).is(':checked')) {
            websock.send("sactive:" + number);
            $('#sl' + number).addClass('checked');
        } else {
            websock.send("sinactive:" + number);
            $('#sl' + number).removeClass('checked');
        }
    } else if (state == 1) {
        $('#sl' + number).addClass('checked');
        $('#sl' + number).prop("checked", true);
    } else if (state == 0) {
        $('#sl' + number).removeClass('checked');
        $('#sl' + number).prop("checked", false);
    }
}
