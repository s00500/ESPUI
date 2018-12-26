const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=100;const UI_TITEL=0;const UI_LABEL=1;const UPDATE_LABEL=6;const UI_BUTTON=2;const UI_SWITCHER=3;const UPDATE_SWITCHER=7;const UI_PAD=4;const UI_CPAD=5;const UI_SLIDER=8;const UPDATE_SLIDER=9;const UI_NUMBER=10;const UPDATE_NUMBER=11;const UI_TEXT_INPUT=12;const UPDATE_TEXT_INPUT=13;const UI_GRAPH=14;const CLEAR_GRAPH=15;const ADD_GRAPH_POINT=16;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_NONE=7;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";break;case C_EMERALD:return"emerald";break;case C_PETERRIVER:return"peterriver";break;case C_WETASPHALT:return"wetasphalt";break;case C_SUNFLOWER:return"sunflower";break;case C_CARROT:return"carrot";break;case C_ALIZARIN:return"alizarin";break;case C_NONE:return"";break;default:return"";}}
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

const uint8_t JS_CONTROLS_GZIP[2108] PROGMEM = { 31,139,8,0,72,101,35,92,2,255,213,89,209,114,226,56,22,125,207,87,120,220,83,13,222,208,16,186,183,103,103,33,78,21,77,232,14,51,12,100,9,153,222,218,23,74,216,162,113,197,200,94,89,134,206,166,248,141,249,144,249,164,249,146,189,146,44,91,118,236,64,66,245,84,205,75,98,95,93,29,29,157,123,117,37,11,39,32,17,51,110,135,243,225,120,56,27,246,70,243,79,183,67,187,125,118,214,117,84,195,108,56,27,140,108,205,48,234,125,0,67,91,25,174,47,123,179,65,98,252,33,243,250,112,59,155,77,198,246,219,204,114,243,121,56,235,95,13,166,246,187,124,215,212,254,143,204,247,186,119,105,255,61,123,237,243,247,247,26,212,104,120,9,29,126,44,0,73,235,63,51,191,241,237,47,31,192,210,62,203,59,42,115,91,155,229,224,223,51,208,224,250,118,102,183,223,230,189,245,166,119,89,143,79,211,222,245,149,221,86,44,251,163,65,111,170,140,138,106,239,242,82,154,230,215,147,225,24,250,43,129,62,78,166,169,164,31,122,253,159,83,57,71,131,143,179,84,180,233,240,211,213,44,149,171,63,24,207,128,117,58,224,124,118,59,253,215,237,100,120,51,72,161,250,243,193,47,131,105,111,116,153,226,245,231,215,3,232,53,29,254,10,93,223,166,198,207,131,89,239,230,250,170,55,210,224,231,55,183,227,143,163,201,231,220,24,253,222,116,58,153,165,218,247,231,189,209,240,63,189,233,112,156,198,186,63,31,79,198,3,8,222,50,38,14,243,2,98,56,129,31,208,190,143,162,168,46,30,135,174,245,144,60,216,227,120,189,192,52,181,119,163,173,199,156,149,230,135,34,172,79,173,67,49,139,41,49,225,207,127,227,192,139,176,217,93,80,140,238,186,137,99,50,95,229,134,215,152,34,223,45,56,101,18,40,191,16,51,76,169,183,193,180,224,154,9,163,92,183,152,161,40,92,33,159,21,92,83,185,148,103,20,147,165,31,108,31,97,74,13,149,151,131,40,13,138,88,74,85,229,132,124,239,127,136,122,164,224,198,149,86,46,170,201,197,75,20,251,44,51,239,118,39,27,68,141,45,94,68,129,115,215,213,158,251,1,33,216,97,216,181,151,200,143,112,22,49,138,35,134,40,171,91,15,223,215,221,192,137,215,152,48,171,137,92,183,110,254,205,180,154,193,114,89,183,186,223,215,205,87,52,216,194,251,138,173,253,186,105,90,221,4,183,233,248,65,132,193,37,65,233,238,78,180,100,32,55,12,177,56,26,192,180,41,140,80,193,133,163,167,190,48,6,197,235,96,131,101,22,153,34,61,222,124,161,24,19,211,122,228,10,60,115,126,20,187,37,94,146,180,96,97,180,140,113,96,36,12,56,201,215,175,126,252,225,221,251,110,73,175,108,234,57,43,169,63,56,190,231,220,117,18,229,118,185,57,175,16,113,125,252,171,23,121,11,207,247,216,125,31,12,95,64,159,7,111,89,255,174,56,255,215,175,191,83,146,55,87,158,235,98,98,61,164,241,224,193,76,97,85,140,82,119,152,248,96,3,15,35,47,98,152,192,178,50,55,233,152,142,24,211,108,148,115,105,8,209,211,0,218,4,111,141,207,120,113,3,207,152,213,205,109,212,105,181,204,211,173,71,220,96,219,244,3,7,113,2,205,85,16,49,130,214,248,212,108,109,35,45,252,1,9,66,76,108,69,180,142,55,140,175,120,18,5,62,134,222,95,0,80,122,26,220,239,128,8,86,69,154,225,175,192,46,149,46,163,144,101,19,163,49,238,238,52,106,34,55,15,225,38,28,1,178,152,177,57,52,204,109,79,160,113,67,25,2,95,133,50,20,34,98,5,4,222,234,34,134,236,159,110,38,227,102,136,40,172,38,104,104,114,155,37,250,98,59,141,250,34,112,239,133,205,129,55,76,109,88,242,73,21,229,238,77,118,31,226,164,142,230,183,245,142,104,6,110,140,6,126,212,92,6,116,128,160,19,246,49,135,181,47,4,11,168,37,95,37,195,7,238,222,17,132,34,6,165,232,139,183,188,87,206,214,174,171,77,166,158,118,130,153,90,122,197,82,199,135,78,74,158,121,204,135,185,112,38,62,90,96,95,132,120,141,60,114,133,145,11,149,51,89,167,153,67,17,79,28,52,58,89,45,66,33,164,20,212,169,115,215,219,64,8,33,137,236,26,219,6,124,19,138,215,36,50,160,220,186,6,147,82,25,230,233,137,182,57,37,122,192,187,117,122,98,214,46,206,87,239,13,207,181,107,224,37,154,60,87,152,213,171,160,3,150,243,214,234,61,248,66,21,185,56,143,66,68,68,31,63,223,73,49,17,125,12,241,247,205,150,162,48,67,219,32,63,198,2,141,99,92,156,183,128,255,133,89,156,173,60,69,237,153,46,100,229,11,167,91,57,55,152,218,34,102,12,138,78,64,214,65,28,97,168,3,196,174,73,155,168,124,245,220,132,27,6,95,119,86,77,185,199,225,147,206,178,250,212,158,82,59,211,71,226,100,10,113,49,204,211,164,143,172,197,44,136,157,149,40,143,157,108,97,89,15,184,25,82,204,243,242,82,238,145,176,18,117,82,9,68,67,80,135,74,91,13,13,146,191,4,56,41,178,187,71,139,66,29,121,59,124,201,9,237,109,243,92,230,10,151,36,242,83,18,167,89,46,201,69,110,0,27,40,210,46,40,37,138,128,71,194,152,217,185,124,240,8,4,87,216,13,94,12,236,154,232,178,8,190,74,193,163,66,170,6,164,207,89,171,1,248,54,146,139,22,137,125,31,98,149,140,171,226,208,133,189,44,11,148,109,155,103,166,245,240,188,169,240,41,252,105,244,83,222,187,147,138,213,244,56,25,191,233,2,131,150,196,40,230,42,218,132,129,183,152,21,245,128,127,8,117,84,221,63,71,138,32,20,245,165,71,215,181,252,106,13,145,43,243,81,126,57,52,140,189,75,118,95,15,181,110,87,20,47,237,218,43,25,144,208,41,44,225,201,207,231,45,4,74,43,210,156,243,183,42,216,79,11,108,158,19,180,209,68,226,59,95,77,54,196,137,206,190,119,1,58,150,235,6,95,105,207,17,173,204,189,92,177,101,65,177,63,126,251,157,75,6,9,224,237,103,37,190,10,159,195,171,188,67,57,51,122,12,51,254,241,250,28,98,165,254,229,188,252,99,120,241,47,236,231,240,42,245,47,231,181,216,199,171,37,19,77,230,179,48,64,74,230,151,56,95,27,60,37,94,184,165,229,242,175,124,79,43,71,63,96,87,43,197,214,182,53,1,238,31,79,93,100,66,5,119,255,88,238,57,240,34,121,122,60,121,185,190,42,216,211,99,217,231,209,139,244,23,199,211,23,217,94,193,126,113,44,251,28,120,145,188,115,60,249,100,163,170,160,239,28,75,191,0,95,113,160,211,174,64,197,70,167,103,108,246,57,35,14,74,101,29,211,211,96,201,145,42,61,212,40,10,103,86,23,3,9,227,81,67,251,209,33,83,92,135,190,116,231,229,47,111,34,223,115,247,238,194,162,150,61,251,211,73,244,82,199,174,168,226,243,137,222,173,83,18,242,223,27,215,139,28,138,25,86,239,7,112,211,207,146,148,223,126,212,140,181,7,251,195,25,252,71,95,237,90,251,12,158,164,230,181,226,231,71,77,47,213,197,162,93,144,21,136,121,33,3,55,147,243,158,139,161,110,4,203,122,237,85,254,32,108,117,193,11,62,253,146,30,165,89,33,195,39,167,57,231,151,97,117,62,166,134,99,53,50,170,13,19,102,97,170,4,205,39,130,188,238,254,54,71,176,35,130,47,195,18,177,123,159,159,241,57,100,103,225,35,231,174,43,183,86,18,175,11,73,33,35,72,196,13,114,117,192,96,71,151,119,95,202,85,190,229,191,10,96,31,111,85,159,179,245,95,9,132,108,130,75,186,164,97,168,202,21,173,255,146,240,87,83,156,223,174,21,36,63,64,100,222,235,165,18,23,180,146,4,246,232,172,238,187,213,173,155,184,145,171,155,183,228,142,192,169,79,36,137,17,80,67,84,244,116,208,157,126,135,183,198,81,132,128,186,118,133,165,223,222,230,178,70,190,200,27,58,160,97,167,217,144,52,8,146,217,77,104,36,162,76,4,225,142,121,10,255,79,205,78,234,220,213,111,10,161,45,119,105,172,233,88,54,168,212,230,169,81,217,75,70,213,175,45,164,127,195,139,248,241,89,92,86,39,143,249,113,22,220,166,193,139,237,168,224,18,135,154,131,54,92,186,177,242,48,53,10,3,38,183,152,218,5,166,220,126,59,85,76,66,103,63,149,208,201,113,209,114,16,142,180,213,200,203,3,144,151,85,200,252,208,83,13,125,128,126,225,162,10,154,159,101,171,161,253,3,160,253,42,104,113,208,172,198,166,7,96,211,18,236,220,47,24,234,208,146,68,30,206,122,12,139,76,19,79,182,45,46,107,248,187,216,234,178,124,247,162,186,217,73,110,160,76,43,253,17,41,25,55,66,0,191,193,218,208,106,167,84,253,181,31,24,20,72,119,199,103,80,68,242,200,94,172,252,47,83,25,156,192,51,178,185,180,249,175,105,251,105,20,125,66,26,132,89,187,58,209,22,176,207,30,99,87,208,218,3,159,158,104,255,15,153,88,55,124,9,32,0,0 };
