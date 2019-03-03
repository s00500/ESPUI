const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_TITEL=0;const UI_BUTTON=1;const UI_PAD=2;const UI_CPAD=3;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element)};handleEvent(fauxEvent);});break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:$("#row").append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'><h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr /><span id='l"+
data.id+
"' class='label label-wrap'>"+
data.value+
"</span></div>");break;case UI_BUTTON:$("#row").append("<div class='one columns card tcenter "+
colorClass(data.color)+
"'><h5>"+
data.label+
"</h5><hr/><button onmousedown='buttonclick("+
data.id+
", true)' onmouseup='buttonclick("+
data.id+
", false)' id='"+
data.id+
"'>"+
data.value+
"</button></div>");$("#"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);}});$("#"+data.id).on({touchend:function(e){e.preventDefault();buttonclick(data.id,false);}});break;case UI_SWITCHER:var label="<label id='sl"+data.id+"' class='switch checked'>";var input="<div class='in'><input type='checkbox' id='s"+
data.id+
"' onClick='switcher("+
data.id+
",null)' checked></div>";if(data.value=="0"){label="<label id='sl"+data.id+"' class='switch'>";input="<div class='in'><input type='checkbox' id='s"+
data.id+
"' onClick='switcher("+
data.id+
",null)' ></div>";}
$("#row").append("<div id='"+
data.id+
"' class='one columns card tcenter "+
colorClass(data.color)+
"'><h5>"+
data.label+
"</h5><hr/>"+
label+
input+
"</label>"+
"</div>");break;case UI_CPAD:center="<a class='confirm' onmousedown='padclick(CENTER, "+
data.id+
", true)' onmouseup='padclick(CENTER, "+
data.id+
", false)' href='#' id='pc"+
data.id+
"'>OK</a>";case UI_PAD:$("#row").append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'><h5>"+
data.label+
"</h5><hr/>"+
"<nav class='control'>"+
"<ul>"+
"<li><a onmousedown='padclick(FOR, "+
data.id+
", true)' onmouseup='padclick(FOR, "+
data.id+
", false)' href='#' id='pf"+
data.id+
"'>▲</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+
data.id+
", true)' onmouseup='padclick(RIGHT, "+
data.id+
", false)' href='#' id='pr"+
data.id+
"'>▲</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+
data.id+
", true)' onmouseup='padclick(LEFT, "+
data.id+
", false)' href='#' id='pl"+
data.id+
"'>▲</a></li>"+
"<li><a onmousedown='padclick(BACK, "+
data.id+
", true)' onmouseup='padclick(BACK, "+
data.id+
", false)' href='#' id='pb"+
data.id+
"'>▲</a></li>"+
"</ul>"+
center+
"</nav>"+
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(FOR,data.id,true);}});$("#pf"+data.id).on({touchend:function(e){e.preventDefault();padclick(FOR,data.id,false);}});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);}});$("#pl"+data.id).on({touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);}});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);}});$("#pr"+data.id).on({touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);}});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(BACK,data.id,true);}});$("#pb"+data.id).on({touchend:function(e){e.preventDefault();padclick(BACK,data.id,false);}});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);}});$("#pc"+data.id).on({touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);}});break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:if(data.value=="0")switcher(data.id,0);else switcher(data.id,1);break;case UI_SLIDER:$("#row").append("<div class='two columns card tcenter card-slider "+
colorClass(data.color)+
"'>"+
"<h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr />"+
"<div id='sl"+
data.id+
"' class='rkmd-slider slider-discrete slider-"+
colorClass(data.color)+
"'>"+
"<input type='range' min='0' max='100' value='"+
data.value+
"'>"+
"</div>"+
"</div>");$("#row").append("<script>"+"rkmd_rangeSlider('#sl"+data.id+"');"+"</script>");break;case UPDATE_SLIDER:slider_move($("#sl"+data.id),data.value,"100",false);break;case UI_NUMBER:$("#row").append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr />"+
"<input style='color:black;' id='num"+
data.id+
"' type='number' value='"+
data.value+
"' onchange='numberchange("+
data.id+
")' />"+
"</div>");break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UI_TEXT_INPUT:$("#row").append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr />"+
"<input style='color:black;' id='text"+
data.id+
"' value='"+
data.value+
"' onchange='textchange("+
data.id+
")' />"+
"</div>");break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;default:console.error("Unknown type or event");break;}};websock.onmessage=handleEvent;}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);console.log(val);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);console.log(val);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case FOR:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case BACK:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
)=====";

const uint8_t JS_CONTROLS_GZIP[2133] PROGMEM = { 31,139,8,0,14,55,124,92,2,255,213,89,109,114,219,56,18,253,239,83,112,152,169,72,92,59,178,148,25,103,50,162,233,42,69,86,98,109,52,178,71,150,39,91,251,71,5,145,80,196,50,5,114,65,80,138,215,165,107,236,65,230,72,115,146,109,0,4,248,97,210,178,173,202,86,237,31,155,108,52,30,30,94,55,26,32,228,134,36,102,198,205,112,54,28,15,167,195,222,104,246,233,102,232,188,109,183,109,87,53,76,135,211,193,200,201,25,62,220,76,167,151,99,167,147,89,174,122,231,206,219,236,181,207,223,127,202,222,71,189,15,128,240,179,50,92,157,247,166,131,212,216,105,255,156,249,93,127,25,78,251,23,131,137,115,82,116,213,246,78,251,36,231,61,26,158,131,237,93,201,87,90,59,237,119,153,231,248,230,183,15,96,251,165,232,153,90,59,237,95,114,115,29,252,99,10,74,92,221,76,157,247,69,239,92,75,167,253,62,235,241,105,210,187,186,112,126,77,13,189,243,115,105,153,93,93,14,199,220,53,109,232,143,6,189,73,234,220,105,43,247,143,151,19,45,236,135,94,255,179,214,116,52,248,56,213,138,78,134,159,46,166,90,207,254,96,60,5,218,74,182,254,108,122,51,249,253,230,114,120,61,208,80,253,217,224,183,193,164,55,58,215,120,253,217,213,0,122,77,134,127,64,215,183,218,248,101,48,237,93,95,93,244,70,57,248,217,245,205,248,227,232,242,75,97,140,126,111,50,185,156,234,184,244,103,189,209,240,159,189,201,112,172,229,239,207,206,123,147,207,90,227,254,108,124,57,30,56,111,79,78,236,69,66,92,230,135,196,112,195,32,164,253,0,197,113,83,60,14,61,235,62,125,112,198,201,106,142,169,182,219,241,198,103,238,50,231,135,98,156,159,107,151,98,150,80,98,194,159,127,37,161,31,99,211,78,93,210,169,43,7,188,194,20,5,158,110,206,116,80,30,17,102,152,82,127,141,169,118,202,116,81,78,27,204,80,28,45,81,192,180,147,214,73,249,196,9,89,4,225,38,135,35,101,83,237,46,162,52,204,250,43,9,85,51,10,252,127,35,234,19,237,192,21,84,141,30,162,183,166,237,225,5,74,2,166,140,166,189,221,30,172,17,53,54,120,30,135,238,173,157,123,238,135,132,96,151,97,207,89,160,32,198,89,20,40,142,25,162,172,105,221,255,216,244,66,55,89,97,194,172,22,242,188,166,249,55,211,106,133,139,69,211,178,127,108,154,175,104,184,129,247,37,91,5,77,211,180,236,20,183,229,6,97,140,193,37,69,177,183,7,185,0,147,107,134,88,18,15,96,162,20,70,168,225,194,209,181,47,140,65,241,42,92,99,153,25,166,8,249,155,175,20,99,98,90,15,92,129,103,193,143,98,175,194,75,146,22,44,140,99,99,28,26,41,3,78,242,245,171,247,239,126,58,177,43,122,101,83,47,88,73,243,222,13,124,247,182,155,42,183,45,204,121,137,136,23,224,63,252,216,159,251,129,207,238,250,96,248,10,250,220,251,139,230,15,229,249,191,126,253,131,146,188,181,244,61,15,19,235,94,199,131,7,83,195,170,24,105,119,152,248,96,13,15,35,63,102,152,192,82,49,215,122,76,87,140,105,30,85,115,57,18,162,235,0,58,4,111,140,47,120,126,13,207,152,53,205,77,220,61,62,54,15,55,62,241,194,77,43,8,93,196,9,180,150,97,204,8,90,225,67,243,120,19,231,194,31,146,48,194,196,81,68,155,120,205,248,42,38,113,24,96,232,253,21,0,165,167,193,253,158,16,193,186,72,51,252,13,216,105,233,50,10,89,54,49,154,96,123,155,163,38,114,243,41,220,132,35,64,150,51,182,128,134,185,237,17,52,110,168,66,224,171,80,134,66,68,172,132,192,91,61,196,144,243,247,235,203,113,43,66,20,86,19,52,180,184,205,18,125,177,163,163,62,15,189,59,97,115,225,13,83,7,150,124,90,25,185,123,139,221,69,56,173,141,197,29,188,43,154,129,27,163,97,16,183,22,33,29,32,232,132,3,204,97,157,51,193,2,106,201,55,201,240,158,187,119,5,161,152,65,9,250,234,47,238,148,179,181,181,115,147,105,234,78,48,83,203,158,83,140,110,109,69,64,156,20,186,154,60,243,89,0,115,225,76,2,52,199,129,8,241,10,249,228,2,35,15,170,100,186,78,51,135,50,158,56,34,116,179,90,132,34,72,41,168,83,167,158,191,134,16,66,18,57,13,182,9,249,198,146,172,72,108,64,129,245,12,38,165,50,204,195,131,220,134,147,234,1,239,214,225,129,217,56,59,93,158,24,190,231,52,192,75,52,249,158,48,171,87,65,7,44,167,199,203,19,240,133,42,114,118,26,71,136,136,62,65,177,147,98,34,250,24,226,239,155,13,69,81,134,182,70,65,130,5,26,199,56,59,61,6,254,103,102,121,182,242,88,181,99,186,144,149,47,156,110,237,220,96,106,243,132,49,40,58,33,89,133,73,140,161,14,16,167,33,109,162,242,53,11,19,62,50,248,186,179,26,202,61,137,30,117,150,213,167,241,152,218,153,62,18,39,83,136,139,97,30,166,125,100,45,102,97,226,46,69,121,236,102,11,203,186,199,173,136,98,158,151,231,114,143,132,149,152,39,149,66,28,9,234,80,105,235,161,65,242,151,0,167,69,118,251,96,81,168,195,107,151,47,57,161,189,99,158,202,92,225,146,196,129,38,113,152,229,146,92,228,6,176,129,34,237,129,82,162,8,248,36,74,152,83,200,7,159,64,112,133,221,224,197,192,105,136,46,243,240,155,20,60,46,165,106,72,250,156,181,26,128,111,35,133,104,145,36,8,32,86,233,184,42,14,54,236,101,89,160,28,199,108,155,214,253,243,166,194,167,240,63,163,175,121,111,15,106,86,211,195,100,252,174,11,12,90,82,163,152,171,104,19,6,222,98,214,212,3,254,21,213,85,117,255,20,41,130,80,212,23,62,93,53,138,171,53,66,158,204,71,249,121,112,100,236,92,178,187,122,168,117,187,164,120,225,52,94,201,128,68,110,105,9,95,126,62,61,70,103,166,38,205,57,127,175,130,253,184,192,230,41,65,235,156,72,124,231,107,200,134,36,213,57,240,207,64,199,106,221,224,83,236,57,162,85,185,87,43,182,40,41,246,215,127,254,228,146,65,2,248,187,89,137,79,191,231,240,170,238,80,205,140,238,195,140,127,161,62,135,88,165,127,53,175,96,31,94,252,51,250,57,188,42,253,171,121,205,119,241,58,150,137,38,243,89,24,32,37,139,75,156,175,13,158,18,47,220,210,10,249,87,189,167,85,163,63,97,87,171,196,206,109,107,2,60,216,159,186,200,132,26,238,193,190,220,11,224,101,242,116,127,242,114,125,213,176,167,251,178,47,162,151,233,207,247,167,47,178,189,134,253,124,95,246,5,240,50,121,119,127,242,233,70,85,67,223,221,151,126,9,190,230,64,151,187,188,20,27,93,62,99,179,207,25,113,80,170,234,168,79,131,21,71,42,125,168,81,20,218,150,141,129,132,241,160,161,243,224,144,41,110,61,95,186,243,242,151,55,113,224,123,59,119,97,81,203,158,253,233,36,122,169,99,87,92,243,249,68,111,87,154,132,252,247,198,243,99,151,98,134,213,251,19,184,229,207,146,148,223,126,52,140,149,15,251,67,27,254,163,111,78,163,211,134,39,169,121,163,252,249,209,200,151,234,114,209,46,201,10,196,252,136,129,155,201,121,207,196,80,215,130,101,179,241,170,120,16,182,108,240,130,79,191,180,71,101,86,200,240,201,105,206,248,101,88,147,143,153,195,177,142,50,170,71,38,204,194,84,9,90,76,4,121,169,253,125,142,96,123,4,95,134,37,102,119,1,63,227,115,200,238,60,64,238,173,45,183,86,146,172,74,73,33,35,72,196,173,112,125,192,96,71,151,119,95,202,85,190,21,191,10,96,31,63,174,63,103,231,127,11,16,178,9,46,122,73,195,80,181,43,58,255,115,193,255,155,226,252,118,173,36,249,19,68,230,189,94,42,113,73,43,73,96,135,206,234,190,91,221,186,137,27,185,166,121,67,110,9,156,250,68,146,24,33,53,68,69,215,131,110,243,119,120,43,28,199,8,168,231,174,176,242,183,183,133,172,145,47,242,134,14,104,56,58,27,210,6,65,50,187,9,141,69,148,137,32,220,53,15,225,255,161,217,213,206,118,254,166,16,218,10,151,198,57,29,171,6,149,218,60,54,42,123,201,168,249,107,11,233,127,228,199,252,248,44,46,171,211,199,226,56,115,110,203,193,139,237,168,228,146,68,57,135,220,112,122,99,229,97,58,42,13,152,222,98,230,46,48,229,246,219,173,99,18,185,187,169,68,110,129,75,46,7,225,72,91,143,188,120,2,242,162,14,153,31,122,234,161,159,160,95,52,175,131,230,103,217,122,232,224,9,208,65,29,180,56,104,214,99,211,39,96,211,10,236,194,47,24,234,208,146,70,30,206,122,12,139,76,19,79,142,35,46,107,248,187,216,234,178,124,247,227,166,217,77,111,160,76,75,255,136,148,142,27,35,128,95,227,220,208,106,167,84,253,115,63,48,40,16,123,203,103,80,70,242,201,78,172,226,47,83,25,156,192,51,178,185,116,248,175,105,187,105,148,125,34,26,70,89,187,58,209,150,176,219,15,177,107,104,237,128,215,39,218,255,2,112,11,124,138,244,31,0,0 };
