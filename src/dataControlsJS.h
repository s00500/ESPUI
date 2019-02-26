const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=100;const UI_TITEL=0;const UI_LABEL=1;const UPDATE_LABEL=6;const UI_BUTTON=2;const UI_SWITCHER=3;const UPDATE_SWITCHER=7;const UI_PAD=4;const UI_CPAD=5;const UI_SLIDER=8;const UPDATE_SLIDER=9;const UI_NUMBER=10;const UPDATE_NUMBER=11;const UI_TEXT_INPUT=12;const UPDATE_TEXT_INPUT=13;const UI_GRAPH=14;const CLEAR_GRAPH=15;const ADD_GRAPH_POINT=16;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_NONE=7;const C_DARK=8;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"";case C_DARK:return"dark";default:return"";}}
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

const uint8_t JS_CONTROLS_GZIP[2122] PROGMEM = { 31,139,8,0,125,24,117,92,2,255,213,89,209,114,219,184,21,125,207,87,112,153,157,72,172,29,217,78,154,237,86,52,61,163,200,74,172,70,43,187,178,188,233,244,69,3,145,80,196,49,5,178,32,40,197,245,232,55,250,33,253,164,126,73,47,0,2,4,105,210,178,173,73,103,250,98,147,23,23,7,7,231,94,92,128,144,31,147,148,89,55,195,217,112,60,156,14,123,163,217,231,155,161,119,114,124,236,250,170,97,58,156,14,70,158,97,24,245,62,130,225,68,25,174,206,123,211,65,110,252,165,240,250,120,51,157,94,142,189,119,133,229,250,235,112,218,191,24,76,188,247,229,174,218,254,167,194,247,170,119,238,253,177,120,237,243,247,15,6,212,104,120,14,29,126,173,0,73,235,159,11,191,241,205,111,31,193,114,114,92,118,84,230,19,99,150,131,191,77,65,131,171,155,169,119,242,174,236,109,54,189,47,122,124,158,244,174,46,188,19,197,178,63,26,244,38,202,168,168,246,206,207,165,105,118,117,57,28,67,127,37,208,167,203,137,150,244,99,175,255,69,203,57,26,124,154,106,209,38,195,207,23,83,45,87,127,48,158,2,107,61,224,108,122,51,249,235,205,229,240,122,160,161,250,179,193,111,131,73,111,116,174,241,250,179,171,1,244,154,12,127,135,174,239,180,241,235,96,218,187,190,186,232,141,12,248,217,245,205,248,211,232,242,107,105,140,126,111,50,185,156,106,237,251,179,222,104,248,247,222,100,56,214,177,238,207,198,151,227,129,14,94,127,118,222,155,124,129,208,44,50,226,179,48,38,150,31,71,49,237,71,40,77,219,226,113,24,56,247,249,131,55,206,86,115,76,181,221,77,55,33,243,151,134,31,74,177,57,211,46,197,44,163,196,134,63,255,200,226,48,197,182,155,187,228,19,87,14,120,133,41,138,2,221,92,168,160,60,18,204,48,165,225,26,83,237,84,168,162,156,54,152,161,52,89,162,136,105,39,173,146,242,73,51,178,136,226,141,129,35,69,83,237,62,162,52,46,250,43,1,85,51,138,194,127,34,26,18,237,192,229,84,141,218,200,69,85,198,0,209,91,219,13,240,2,101,17,43,60,183,219,87,107,68,173,13,158,167,177,127,235,26,207,253,152,16,236,51,28,120,11,20,165,184,8,13,197,41,67,148,181,157,251,159,219,65,236,103,43,76,152,211,65,65,208,182,255,96,59,157,120,177,104,59,238,207,109,251,53,141,55,240,190,100,171,168,109,219,142,155,227,118,252,40,78,49,184,228,40,238,246,149,17,117,114,205,16,203,210,1,204,158,194,8,13,92,56,186,246,133,49,40,94,197,107,44,211,197,22,121,240,246,27,197,152,216,206,3,87,224,89,242,163,56,168,241,146,164,5,11,235,200,26,199,86,206,128,147,124,243,250,215,95,222,127,112,107,122,21,83,47,89,73,251,222,143,66,255,182,155,43,183,45,205,121,137,72,16,225,223,195,52,156,135,81,200,238,250,96,248,6,250,220,135,139,246,79,213,249,191,121,243,147,146,188,179,12,131,0,19,231,94,199,131,7,83,195,170,24,105,119,152,248,96,13,15,163,48,101,152,192,250,177,215,122,76,95,140,105,31,214,115,57,20,162,235,0,122,4,111,172,175,120,126,13,207,152,181,237,77,218,61,58,178,15,54,33,9,226,77,39,138,125,196,9,116,150,113,202,8,90,225,3,251,104,147,26,225,143,73,156,96,226,41,162,109,188,102,124,105,147,52,142,48,244,254,6,128,210,211,226,126,79,136,96,83,164,25,254,14,236,180,116,5,133,34,155,24,205,176,187,53,168,137,220,124,10,55,225,8,144,213,140,45,161,97,110,123,4,141,27,234,16,248,42,148,161,16,17,171,32,240,214,0,49,228,253,229,250,114,220,73,16,133,213,4,13,29,110,115,68,95,236,233,168,207,227,224,78,216,124,120,195,212,131,37,159,151,75,238,222,97,119,9,206,11,102,121,59,239,138,102,224,198,104,28,165,157,69,76,7,8,58,225,8,115,88,239,76,176,128,90,242,93,50,188,231,238,93,65,40,101,80,151,190,133,139,59,229,236,108,93,99,50,109,221,9,102,234,184,115,138,209,173,171,8,136,99,67,87,147,103,33,139,96,46,156,73,132,230,56,18,33,94,161,144,92,96,20,64,233,204,215,105,225,80,197,19,7,140,110,81,139,80,2,41,5,117,234,52,8,215,16,66,72,34,175,197,54,49,223,109,178,21,73,45,168,186,129,197,164,84,150,125,240,202,216,133,114,61,224,221,57,120,101,183,206,78,151,31,172,48,240,90,224,37,154,194,64,152,213,171,160,3,150,211,163,229,7,240,133,42,114,118,154,38,136,136,62,81,185,147,98,34,250,88,226,239,219,13,69,73,129,182,70,81,134,5,26,199,56,59,61,2,254,103,118,117,182,242,244,180,99,186,144,149,47,156,110,227,220,96,106,243,140,49,40,58,49,89,197,89,138,161,14,16,175,37,109,162,242,181,75,19,62,180,248,186,115,90,202,61,75,30,117,150,213,167,245,152,218,133,62,18,167,80,136,139,97,31,228,125,100,45,102,113,230,47,69,121,236,22,11,203,185,199,157,132,98,158,151,231,114,143,132,149,104,146,202,33,14,5,117,168,180,205,208,32,249,75,128,243,34,187,125,176,40,212,81,183,203,151,156,208,222,179,79,101,174,112,73,210,72,147,56,40,114,73,46,114,11,216,64,145,14,64,41,81,4,66,146,100,204,43,229,67,72,32,184,194,110,241,98,224,181,68,151,121,252,93,10,158,86,82,53,38,125,206,90,13,192,183,145,82,180,72,22,69,16,171,124,92,21,7,23,246,178,34,80,158,103,31,219,206,253,243,166,194,167,240,63,163,175,121,111,95,53,172,166,135,201,248,67,23,24,180,228,70,49,87,209,38,12,188,197,110,168,7,252,3,168,171,234,254,41,82,4,161,168,47,66,186,106,149,87,107,130,2,153,143,242,139,225,208,218,185,100,119,245,80,235,118,73,241,194,107,189,150,1,73,252,202,18,190,252,114,122,132,206,108,77,154,115,254,81,5,251,113,129,237,83,130,214,134,72,124,231,107,201,134,44,215,57,10,207,64,199,122,221,224,235,236,57,162,213,185,215,43,182,168,40,246,159,127,253,155,75,6,9,16,238,102,37,190,6,159,195,171,190,67,61,51,186,15,51,254,209,250,28,98,181,254,245,188,162,125,120,241,47,235,231,240,170,245,175,231,53,223,197,235,72,38,154,204,103,97,128,148,44,47,113,190,54,120,74,188,112,75,43,229,95,253,158,86,143,254,132,93,173,22,219,216,214,4,120,180,63,117,145,9,13,220,163,125,185,151,192,171,228,233,254,228,229,250,106,96,79,247,101,95,70,175,210,159,239,79,95,100,123,3,251,249,190,236,75,224,85,242,254,254,228,243,141,170,129,190,191,47,253,10,124,195,129,206,184,250,20,27,157,153,177,197,231,140,56,40,213,117,212,167,193,154,35,149,62,212,40,10,199,142,139,129,132,245,160,225,228,193,33,83,92,131,190,116,231,229,47,111,211,40,12,118,238,194,162,150,61,251,211,73,244,82,199,174,180,225,243,137,222,174,52,9,249,239,109,16,166,62,197,12,171,247,39,112,51,207,146,148,223,126,180,172,85,8,251,195,49,252,71,223,189,214,201,49,60,73,205,91,213,207,143,150,89,170,171,69,187,34,43,16,11,19,6,110,54,231,61,19,67,93,11,150,237,214,235,242,65,216,113,193,11,62,253,242,30,181,89,33,195,39,167,57,227,151,97,109,62,166,129,227,28,22,84,15,109,152,133,173,18,180,156,8,242,154,251,199,28,193,246,8,190,12,75,202,238,34,126,198,231,144,221,121,132,252,91,87,110,173,36,91,85,146,66,70,144,136,171,226,230,128,193,142,46,239,190,148,171,124,43,127,21,192,62,126,212,124,206,54,127,29,16,178,9,46,122,73,195,80,141,43,218,252,5,225,255,77,113,126,187,86,145,252,9,34,243,94,47,149,184,162,149,36,176,67,103,117,223,173,110,221,196,141,92,219,190,33,183,4,78,125,34,73,172,152,90,162,162,235,65,183,230,29,222,10,167,41,2,234,198,21,150,121,123,91,202,26,249,34,111,232,128,134,167,179,33,111,16,36,139,155,208,84,68,153,8,194,93,251,0,254,31,216,93,237,236,154,55,133,208,86,186,52,54,116,172,27,84,106,243,216,168,236,37,163,154,215,22,210,255,48,76,249,241,89,92,86,231,143,229,113,230,220,102,192,139,237,168,226,146,37,134,131,49,156,222,88,121,152,14,43,3,230,183,152,198,5,166,220,126,187,77,76,18,127,55,149,196,47,113,49,114,16,142,180,205,200,139,39,32,47,154,144,249,161,167,25,250,9,250,37,243,38,104,126,150,109,134,142,158,0,29,53,65,139,131,102,51,54,125,2,54,173,193,46,253,130,161,14,45,121,228,225,172,199,176,200,52,241,228,121,226,178,134,191,139,173,174,200,247,48,109,219,221,252,6,202,118,244,143,72,249,184,41,2,248,53,54,134,86,59,165,234,111,252,192,160,64,220,45,159,65,21,41,36,59,177,202,191,76,21,112,2,207,42,230,114,194,127,77,219,77,163,234,147,208,56,41,218,213,137,182,130,125,252,16,187,129,214,14,120,125,162,253,47,92,221,13,122,1,32,0,0 };
