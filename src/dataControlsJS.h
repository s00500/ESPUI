const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_TITEL=0;const UI_LABEL=1;const UPDATE_LABEL=6;const UI_BUTTON=2;const UI_SWITCHER=3;const UPDATE_SWITCHER=7;const UI_PAD=4;const UI_CPAD=5;const UI_SLIDER=8;const UPDATE_SLIDER=9;const UI_NUMBER=10;const UPDATE_NUMBER=11;const UI_TEXT_INPUT=12;const UPDATE_TEXT_INPUT=13;const UI_GRAPH=14;const CLEAR_GRAPH=15;const ADD_GRAPH_POINT=16;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_NONE=7;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";break;case C_EMERALD:return"emerald";break;case C_PETERRIVER:return"peterriver";break;case C_WETASPHALT:return"wetasphalt";break;case C_SUNFLOWER:return"sunflower";break;case C_CARROT:return"carrot";break;case C_ALIZARIN:return"alizarin";break;case C_NONE:return"";break;default:return"";}}
var websock;var websockConnected=false;function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};websock.onmessage=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:$("#row").append("<div class='two columns card tcenter "+
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
"</div>");break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;default:console.error("Unknown type or event");break;}};}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);console.log(val);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);console.log(val);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case FOR:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case BACK:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
)=====";

const uint8_t JS_CONTROLS_GZIP[2028] PROGMEM = { 31,139,8,0,56,43,252,91,2,255,213,89,237,114,226,54,23,254,159,171,112,189,157,5,55,44,36,187,239,246,3,112,102,88,194,54,188,165,144,18,210,237,244,15,35,108,177,120,98,100,87,146,97,183,25,110,227,189,144,94,82,175,228,61,146,44,127,197,14,36,204,118,166,127,18,251,232,232,209,163,243,165,99,225,4,132,113,227,118,56,159,13,103,131,145,125,214,113,180,96,212,123,7,130,115,45,184,190,236,205,6,177,240,219,84,235,221,237,108,54,25,219,175,83,201,205,135,225,172,127,53,152,218,111,242,83,19,249,119,169,238,117,239,210,254,79,250,218,23,239,111,51,80,163,225,37,76,248,190,0,164,164,63,164,122,227,219,159,223,129,228,252,44,175,168,197,231,169,230,108,240,219,108,62,28,95,223,206,236,243,215,121,237,236,208,155,116,198,143,211,222,245,149,125,174,89,246,71,131,222,84,11,53,213,222,229,165,18,205,175,39,195,49,204,215,6,122,63,153,38,38,125,215,235,255,148,152,115,52,120,63,75,140,54,29,254,120,53,75,204,213,31,140,103,192,58,89,112,62,187,157,254,114,59,25,222,12,18,168,254,124,240,243,96,218,27,93,38,120,253,249,245,0,102,77,135,191,194,212,215,137,240,195,96,214,187,185,190,234,141,50,240,243,155,219,241,251,209,228,67,110,141,126,111,58,157,204,18,219,247,231,189,209,240,247,222,116,56,78,124,221,159,143,39,227,1,56,111,25,17,135,123,1,49,156,192,15,104,223,71,140,213,229,227,208,181,238,227,7,123,28,173,23,152,38,242,14,219,122,220,89,101,244,16,195,217,173,181,41,230,17,37,38,252,249,35,10,60,134,205,206,130,98,116,215,137,21,227,253,106,53,188,198,20,249,110,65,41,53,129,214,11,49,199,148,122,27,76,11,170,169,97,180,234,22,115,196,194,21,242,121,65,53,49,151,214,100,17,89,250,193,246,1,166,178,161,214,114,16,165,65,17,75,91,85,43,33,223,251,19,81,143,20,212,132,165,181,138,30,114,241,18,69,62,79,197,187,221,201,6,81,99,139,23,44,112,238,58,153,231,126,64,8,118,56,118,237,37,242,25,78,61,70,49,227,136,242,186,117,255,117,221,13,156,104,141,9,183,154,200,117,235,230,55,166,213,12,150,203,186,213,249,186,110,190,160,193,22,222,87,124,237,215,77,211,234,196,184,77,199,15,24,6,149,24,165,179,59,201,4,3,185,225,136,71,108,0,219,166,176,66,5,23,129,158,232,194,26,20,175,131,13,86,81,100,202,240,120,245,145,98,76,76,235,129,42,240,204,233,81,236,150,104,41,210,146,133,209,50,198,129,17,51,16,36,95,190,248,254,219,55,111,59,37,179,210,173,231,164,164,126,239,248,158,115,215,142,45,183,203,237,121,133,136,235,227,95,61,230,45,60,223,227,159,251,32,248,8,246,185,247,150,245,175,138,251,127,249,242,43,109,242,230,202,115,93,76,172,251,196,31,194,153,9,172,246,81,162,14,27,31,108,224,97,228,49,142,9,164,149,185,73,214,116,228,154,102,163,156,75,67,26,61,113,160,77,240,214,248,128,23,55,240,140,121,221,220,178,118,171,101,158,110,61,226,6,219,166,31,56,72,16,104,174,2,198,9,90,227,83,179,181,101,25,247,7,36,8,49,177,53,209,58,222,112,145,241,132,5,62,134,217,31,1,80,105,26,66,239,0,15,86,121,154,227,79,192,46,49,93,74,33,141,38,78,35,220,217,101,168,201,216,60,132,155,84,4,200,98,196,230,208,176,144,61,130,38,4,143,35,172,49,99,232,35,222,135,33,210,214,69,28,217,255,189,153,140,155,33,162,144,96,48,208,20,50,53,138,237,36,16,22,129,251,89,202,28,120,195,212,134,42,16,23,86,161,222,228,159,67,28,151,86,125,160,183,147,185,220,227,62,64,9,61,31,45,176,47,141,190,70,30,185,194,200,133,90,22,103,78,170,96,101,107,146,238,7,218,105,117,64,33,56,25,42,71,215,245,54,96,84,112,171,93,227,219,64,28,11,209,154,48,3,10,160,107,112,197,212,48,79,79,50,199,133,92,68,190,91,167,39,102,237,162,187,122,107,120,174,93,3,45,57,228,185,82,172,95,37,29,144,116,91,171,183,160,11,121,125,209,101,33,34,114,142,159,159,164,153,200,57,134,252,251,106,75,81,152,162,109,144,31,97,137,38,48,46,186,45,224,127,97,22,119,171,250,154,61,219,133,56,121,230,118,43,247,6,91,91,68,156,67,25,128,16,10,34,134,33,51,137,93,83,50,89,139,234,185,13,55,12,145,9,86,77,171,71,225,163,202,170,30,212,30,179,118,106,31,133,147,90,72,24,195,60,141,231,168,234,200,131,200,89,201,130,213,78,3,221,186,199,205,144,98,81,178,46,213,169,5,185,145,37,21,67,52,36,117,168,125,213,208,96,242,231,0,199,101,79,32,231,221,170,155,208,182,200,34,105,123,219,236,170,88,17,38,97,126,66,226,52,141,37,149,99,6,176,129,178,233,130,165,100,14,122,36,140,184,157,139,7,143,128,115,165,220,16,185,104,215,228,148,69,240,73,25,156,21,66,53,32,125,193,90,47,32,10,123,206,91,36,242,125,240,85,188,174,246,67,7,78,151,212,81,182,109,158,153,214,253,211,182,34,182,240,143,209,79,120,239,78,42,178,233,97,48,126,209,4,131,145,88,40,247,42,199,164,64,140,152,21,245,64,124,154,180,117,217,237,34,77,16,234,249,210,163,235,90,62,91,67,228,170,120,84,189,124,195,216,155,178,251,102,232,188,93,81,188,180,107,47,148,67,66,167,144,194,147,159,186,45,4,150,214,164,5,231,47,85,176,31,55,176,217,37,104,147,49,18,167,129,95,83,3,81,108,103,223,187,0,59,150,219,13,190,155,158,98,180,50,245,114,139,45,11,22,251,251,127,127,9,147,65,0,120,251,89,201,239,180,167,240,42,159,80,206,140,30,195,76,124,78,62,133,88,169,126,57,47,255,24,94,226,155,247,41,188,74,245,203,121,45,246,241,106,169,64,83,241,44,5,16,146,249,20,23,185,33,66,226,153,71,90,46,254,202,207,180,114,244,3,78,181,82,236,204,177,38,193,253,227,169,203,72,168,224,238,31,203,61,7,94,36,79,143,39,175,242,171,130,61,61,150,125,30,189,72,127,113,60,125,25,237,21,236,23,199,178,207,129,23,201,59,199,147,143,15,170,10,250,206,177,244,11,240,21,13,93,230,82,82,30,116,217,136,77,63,103,100,163,84,54,49,233,6,75,90,170,164,169,209,20,206,172,14,6,18,198,131,129,243,7,77,166,188,160,124,238,201,43,94,94,49,223,115,247,158,194,178,150,61,249,211,73,206,210,109,23,171,248,124,162,119,235,132,132,250,247,202,245,152,67,49,199,250,253,0,110,217,94,146,138,251,136,154,177,246,224,124,56,131,255,232,147,93,59,63,131,39,101,243,90,241,243,163,150,45,213,197,162,93,48,43,16,243,66,14,106,166,224,61,151,75,221,72,150,245,218,139,124,35,108,117,64,11,62,253,226,25,165,81,161,220,167,182,57,23,215,83,117,177,102,6,199,106,164,84,27,38,236,194,212,1,154,15,4,117,1,253,101,90,176,35,156,175,220,194,248,103,95,244,248,2,178,189,240,145,115,215,81,71,43,137,214,133,160,80,30,36,242,78,183,218,97,112,162,171,219,40,173,170,222,242,95,5,112,142,183,170,251,236,236,189,189,52,155,228,146,164,52,44,85,153,209,217,187,253,127,155,197,197,125,87,193,228,7,24,89,204,122,174,137,11,182,82,4,246,216,89,223,64,235,59,44,121,71,86,55,111,201,29,129,174,79,6,137,17,80,67,86,244,100,209,221,46,123,99,154,139,11,245,98,221,139,143,105,88,200,78,252,29,15,72,26,233,237,35,147,126,36,146,82,219,60,133,255,167,102,59,81,238,100,111,214,96,44,119,81,155,177,84,217,162,106,247,143,173,202,159,179,106,246,98,66,233,55,60,38,26,100,121,65,28,63,230,215,89,8,89,6,94,30,56,5,149,40,204,40,100,150,75,142,78,225,136,70,97,193,248,154,48,115,67,168,14,216,118,21,147,208,217,79,37,116,114,92,50,81,6,77,107,53,242,242,0,228,101,21,178,104,107,170,161,15,176,95,184,168,130,22,221,106,53,180,127,0,180,95,5,45,91,201,106,108,122,0,54,45,193,206,253,106,160,219,146,216,243,208,205,113,44,35,77,62,217,182,188,142,17,239,242,48,75,227,221,99,117,179,29,223,49,153,86,242,195,77,188,46,67,0,191,193,153,165,245,89,168,231,103,46,245,53,72,103,39,118,80,68,242,200,94,172,252,175,65,41,156,196,51,210,189,156,139,95,176,246,211,40,234,132,52,8,211,113,221,179,22,176,207,30,98,87,208,218,3,159,244,172,255,7,65,240,22,173,100,31,0,0 };
