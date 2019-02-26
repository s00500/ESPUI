const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=100;const UI_TITEL=0;const UI_LABEL=1;const UPDATE_LABEL=6;const UI_BUTTON=2;const UI_SWITCHER=3;const UPDATE_SWITCHER=7;const UI_PAD=4;const UI_CPAD=5;const UI_SLIDER=8;const UPDATE_SLIDER=9;const UI_NUMBER=10;const UPDATE_NUMBER=11;const UI_TEXT_INPUT=12;const UPDATE_TEXT_INPUT=13;const UI_GRAPH=14;const CLEAR_GRAPH=15;const ADD_GRAPH_POINT=16;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_NONE=7;const C_DARK=8;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
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

const uint8_t JS_CONTROLS_GZIP[2118] PROGMEM = { 31,139,8,0,29,85,117,92,2,255,213,89,225,114,219,184,17,254,159,167,224,49,55,145,88,59,178,157,52,215,171,104,122,70,145,149,88,141,78,118,101,249,210,233,31,13,68,66,17,199,20,200,130,160,20,215,163,215,232,131,244,145,250,36,93,0,4,8,210,164,101,91,147,206,244,143,68,46,22,31,62,236,46,22,11,208,143,73,202,172,155,225,108,56,30,78,135,189,209,236,243,205,208,59,57,62,118,125,213,48,29,78,7,35,207,16,140,122,31,65,112,162,4,87,231,189,233,32,23,254,82,104,125,188,153,78,47,199,222,187,66,114,253,117,56,237,95,12,38,222,251,114,87,45,255,83,161,123,213,59,247,254,88,188,246,249,251,7,3,106,52,60,135,14,191,86,128,164,244,207,133,222,248,230,183,143,32,57,57,46,43,42,241,137,49,203,193,223,166,96,131,171,155,169,119,242,174,172,109,54,189,47,122,124,158,244,174,46,188,19,197,178,63,26,244,38,74,168,168,246,206,207,165,104,118,117,57,28,67,127,101,160,79,151,19,109,210,143,189,254,23,109,206,209,224,211,84,27,109,50,252,124,49,213,230,234,15,198,83,96,173,7,156,77,111,38,127,189,185,28,94,15,52,84,127,54,248,109,48,233,141,206,53,94,127,118,53,128,94,147,225,239,208,245,157,22,126,29,76,123,215,87,23,189,145,1,63,187,190,25,127,26,93,126,45,141,209,239,77,38,151,83,109,251,254,172,55,26,254,189,55,25,142,181,175,251,179,241,229,120,160,157,215,159,157,247,38,95,192,53,139,140,248,44,140,137,229,199,81,76,251,17,74,211,182,120,28,6,206,125,254,224,141,179,213,28,83,45,119,211,77,200,252,165,161,135,82,108,206,180,75,49,203,40,177,225,231,31,89,28,166,216,118,115,149,124,226,74,1,175,48,69,81,160,155,11,43,40,141,4,51,76,105,184,198,84,43,21,86,81,74,27,204,80,154,44,81,196,180,146,182,146,210,73,51,178,136,226,141,129,35,141,166,218,125,68,105,92,244,87,6,84,205,40,10,255,137,104,72,180,2,55,167,106,12,16,189,181,221,0,47,80,22,49,37,180,221,237,246,213,26,81,107,131,231,105,236,223,186,198,115,63,38,4,251,12,7,222,2,69,41,46,188,64,113,202,16,101,109,231,254,231,118,16,251,217,10,19,230,116,80,16,180,237,63,216,78,39,94,44,218,142,251,115,219,126,77,227,13,188,47,217,42,106,219,182,227,230,184,29,63,138,83,12,42,57,138,187,125,101,56,152,92,51,196,178,116,0,19,165,48,66,3,23,142,174,117,97,12,138,87,241,26,203,200,176,133,203,223,126,163,24,19,219,121,160,10,60,75,122,20,7,53,90,146,180,96,97,29,89,227,216,202,25,112,146,111,94,255,250,203,251,15,110,77,175,98,234,37,41,105,223,251,81,232,223,118,115,203,109,75,115,94,34,18,68,248,247,48,13,231,97,20,178,187,62,8,190,129,125,238,195,69,251,167,234,252,223,188,249,73,153,188,179,12,131,0,19,231,94,251,131,59,83,195,42,31,105,117,152,248,96,13,15,163,48,101,152,192,82,177,215,122,76,95,140,105,31,214,115,57,20,70,215,14,244,8,222,88,95,241,252,26,158,49,107,219,155,180,123,116,100,31,108,66,18,196,155,78,20,251,136,19,232,44,227,148,17,180,194,7,246,209,38,53,220,31,147,56,193,196,83,68,219,120,205,248,42,38,105,28,97,232,253,13,0,165,166,197,245,158,224,193,38,79,51,252,29,216,105,211,21,20,138,104,98,52,195,238,214,160,38,98,243,41,220,132,34,64,86,35,182,132,134,185,236,17,52,46,168,67,224,171,80,186,66,120,172,130,192,91,3,196,144,247,151,235,203,113,39,65,20,86,19,52,116,184,204,17,125,177,167,189,62,143,131,59,33,243,225,13,83,15,150,124,158,25,185,122,135,221,37,56,207,141,229,157,187,43,154,129,27,163,113,148,118,22,49,29,32,232,132,35,204,97,189,51,193,2,114,201,119,201,240,158,171,119,5,161,148,65,10,250,22,46,238,148,178,179,117,141,201,180,117,39,152,169,227,206,41,70,183,174,34,32,42,132,174,38,207,66,22,193,92,56,147,8,205,113,36,92,188,66,33,185,192,40,128,44,153,175,211,66,161,138,39,106,137,110,145,139,80,2,33,5,121,234,52,8,215,224,66,8,34,175,197,54,49,223,88,178,21,73,45,72,176,129,197,164,169,44,251,224,149,177,225,228,246,128,119,231,224,149,221,58,59,93,126,176,194,192,107,129,150,104,10,3,33,86,175,130,14,72,78,143,150,31,64,23,178,200,217,105,154,32,34,250,68,229,78,138,137,232,99,137,223,183,27,138,146,2,109,141,162,12,11,52,142,113,118,122,4,252,207,236,234,108,101,161,180,99,186,16,149,47,156,110,227,220,96,106,243,140,49,72,58,49,89,197,89,138,33,15,16,175,37,101,34,243,181,75,19,62,180,248,186,115,90,74,61,75,30,85,150,217,167,245,152,181,11,251,72,156,194,66,220,24,246,65,222,71,230,98,22,103,254,82,164,199,110,177,176,156,123,220,73,40,230,113,121,46,247,72,88,137,38,169,28,226,80,80,135,76,219,12,13,38,127,9,112,158,100,183,15,22,133,170,106,187,124,201,9,219,123,246,169,140,21,110,146,52,210,36,14,138,88,146,139,220,2,54,144,164,3,176,148,72,2,33,73,50,230,149,226,33,36,224,92,33,183,120,50,240,90,162,203,60,254,46,13,158,86,66,53,38,125,206,90,13,192,183,145,146,183,72,22,69,224,171,124,92,229,7,23,246,178,194,81,158,103,31,219,206,253,243,166,194,167,240,63,163,175,121,111,95,53,172,166,135,193,248,67,23,24,180,228,66,49,87,209,38,4,188,197,110,200,7,252,172,211,85,121,255,20,41,130,144,212,23,33,93,181,202,171,53,65,129,140,71,121,56,56,180,118,46,217,93,61,212,186,93,82,188,240,90,175,165,67,18,191,178,132,47,191,156,30,161,51,91,147,230,156,127,84,194,126,220,192,246,41,65,107,195,72,124,231,107,201,134,44,183,115,20,158,129,29,235,237,6,7,177,231,24,173,78,189,222,98,139,138,197,254,243,175,127,115,147,65,0,132,187,89,137,131,223,115,120,213,119,168,103,70,247,97,198,207,167,207,33,86,171,95,207,43,218,135,23,63,68,63,135,87,173,126,61,175,249,46,94,71,50,208,100,60,11,1,132,100,121,137,243,181,193,67,226,133,91,90,41,254,234,247,180,122,244,39,236,106,181,216,198,182,38,192,163,253,169,139,72,104,224,30,237,203,189,4,94,37,79,247,39,47,215,87,3,123,186,47,251,50,122,149,254,124,127,250,34,218,27,216,207,247,101,95,2,175,146,247,247,39,159,111,84,13,244,253,125,233,87,224,27,10,58,227,150,83,108,116,102,196,22,199,25,81,40,213,117,212,213,96,77,73,165,139,26,69,225,216,113,49,144,176,30,52,156,60,40,50,197,141,231,75,119,94,254,242,54,141,194,96,231,46,44,114,217,179,143,78,162,151,42,187,210,134,227,19,189,93,105,18,242,239,109,16,166,62,197,12,171,247,39,112,51,107,73,202,111,63,90,214,42,132,253,225,24,254,209,119,175,117,114,12,79,210,230,173,234,241,163,101,166,234,106,210,174,152,21,136,133,9,3,53,155,243,158,137,161,174,5,203,118,235,117,185,16,118,92,208,130,163,95,222,163,54,42,164,251,228,52,103,252,50,172,205,199,52,112,156,195,130,234,161,13,179,176,85,128,150,3,65,222,104,255,152,18,108,15,231,75,183,164,236,46,226,53,62,135,236,206,35,228,223,186,114,107,37,217,170,18,20,210,131,68,220,10,55,59,12,118,116,121,247,165,84,229,91,249,84,0,251,248,81,115,157,109,126,8,16,102,19,92,244,146,134,161,26,87,180,249,177,224,255,205,226,252,118,173,98,242,39,24,153,247,122,169,137,43,182,146,4,118,216,89,221,119,171,91,55,113,35,215,182,111,200,45,129,170,79,4,137,21,83,75,100,116,61,232,214,188,195,91,225,52,69,64,221,184,194,50,111,111,75,81,35,95,228,13,29,208,240,116,52,228,13,130,100,113,19,154,10,47,19,65,184,107,31,192,255,129,221,213,202,174,121,83,8,109,165,75,99,195,142,117,131,74,219,60,54,42,123,201,168,230,181,133,212,63,12,83,94,62,139,203,234,252,177,60,206,156,203,12,120,177,29,85,84,178,196,80,48,134,211,27,43,119,211,97,101,192,252,22,211,184,192,148,219,111,183,137,73,226,239,166,146,248,37,46,70,12,66,73,219,140,188,120,2,242,162,9,153,23,61,205,208,79,176,95,50,111,130,230,181,108,51,116,244,4,232,168,9,90,20,154,205,216,244,9,216,180,6,187,244,5,67,21,45,185,231,161,214,99,88,68,154,120,242,60,113,89,195,223,197,86,87,196,123,152,182,237,110,126,3,101,59,250,35,82,62,110,138,0,126,141,141,161,213,78,169,250,27,31,24,20,136,187,229,51,168,34,133,100,39,86,249,203,84,1,39,240,172,98,46,39,252,107,218,110,26,85,157,132,198,73,209,174,42,218,10,246,241,67,236,6,90,59,224,117,69,251,95,108,56,37,174,236,31,0,0 };
