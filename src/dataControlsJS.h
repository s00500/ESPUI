const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_TITEL=0;const UI_BUTTON=1;const UI_PAD=2;const UI_CPAD=3;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element)};handleEvent(fauxEvent);});break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'><h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr /><span id='l"+
data.id+
"' class='label label-wrap'>"+
data.value+
"</span></div>");break;case UI_BUTTON:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='one columns card tcenter "+
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
var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div id='"+
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
"'>OK</a>";case UI_PAD:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter "+
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
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(FOR,data.id,true);},touchend:function(e){e.preventDefault();padclick(FOR,data.id,false);}});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);}});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);}});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(BACK,data.id,true);},touchend:function(e){e.preventDefault();padclick(BACK,data.id,false);}});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);}});break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:if(data.value=="0")switcher(data.id,0);else switcher(data.id,1);break;case UI_SLIDER:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter card-slider "+
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
"</div>");var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<script>"+"rkmd_rangeSlider('#sl"+data.id+"');"+"</script>");break;case UPDATE_SLIDER:slider_move($("#sl"+data.id),data.value,"100",false);break;case UI_NUMBER:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter "+
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
"</div>");break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UI_TEXT_INPUT:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter "+
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
"</div>");break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;case UI_TAB:$("#tabsnav").append("<li><a href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$('.tabscontent').tabbedContent({loop:true}).data('api');$('a').filter(function(){return $(this).attr('href')==='#click-to-switch';}).on('click',function(e){var tab=prompt('Tab to switch to (number or id)?');if(!tabs.switchTab(tab)){alert('That tab does not exist :\\');}
e.preventDefault();});break;default:console.error("Unknown type or event");break;}};websock.onmessage=handleEvent;}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);console.log(val);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);console.log(val);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case FOR:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case BACK:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
)=====";

const uint8_t JS_CONTROLS_GZIP[2388] PROGMEM = { 31,139,8,0,174,85,124,92,2,255,237,89,239,114,226,56,18,255,158,167,240,122,166,198,246,133,16,152,221,204,206,226,56,87,132,48,19,118,88,146,35,100,231,234,106,171,40,97,139,193,21,99,251,108,1,201,82,188,198,61,200,61,210,61,201,181,36,75,150,29,147,144,161,110,238,182,234,190,128,213,106,181,126,253,71,173,150,228,70,97,74,180,219,222,184,55,232,141,122,237,254,248,227,109,207,121,219,104,216,174,232,24,245,70,221,190,163,16,206,111,71,163,171,129,211,204,41,215,237,11,231,109,222,236,208,246,247,121,187,223,62,7,9,63,8,194,245,69,123,212,205,136,205,198,15,57,223,205,231,222,168,115,217,29,58,39,69,86,73,111,54,78,20,238,126,239,2,104,239,74,188,156,218,108,188,203,57,7,183,191,156,3,237,199,34,103,70,109,54,126,84,116,237,254,117,4,150,184,190,29,57,239,139,220,74,79,179,241,62,31,241,113,216,190,190,116,126,202,8,237,139,11,78,25,95,95,245,6,148,53,235,232,244,187,237,97,198,220,108,252,164,204,216,62,119,154,205,210,92,140,38,136,31,174,134,210,250,231,237,206,39,105,248,126,247,195,72,154,125,216,251,120,57,146,70,239,116,7,35,208,77,216,182,51,30,221,14,255,114,123,213,187,233,74,81,157,113,247,151,238,176,221,191,144,242,58,227,235,46,140,26,246,126,133,161,111,37,241,115,119,212,190,185,190,108,247,21,241,227,155,219,193,135,254,213,231,194,28,157,246,112,120,53,146,206,235,140,219,253,222,223,218,195,222,64,250,168,51,190,104,15,63,73,71,116,198,131,171,65,215,121,123,114,98,79,23,161,75,252,40,212,220,40,136,146,78,128,210,212,100,159,61,207,90,103,31,206,96,49,159,224,68,210,237,116,229,19,119,166,240,161,20,171,186,182,18,76,22,73,168,195,207,223,23,145,159,98,221,206,88,50,213,5,3,158,227,4,5,158,236,206,237,32,56,98,76,112,146,248,75,156,72,166,220,46,130,105,133,9,74,227,25,10,136,100,146,118,18,60,233,34,156,6,209,74,145,195,205,38,250,93,148,36,81,62,94,152,80,116,163,192,255,29,37,126,40,25,168,5,69,167,135,146,59,221,246,240,20,45,2,34,136,186,189,217,28,44,81,162,173,240,36,141,220,59,91,249,238,68,97,136,93,130,61,103,138,130,20,231,94,72,112,74,80,66,76,107,253,218,244,34,119,49,199,33,177,234,200,243,76,253,79,186,85,143,166,83,211,178,95,155,250,171,36,90,65,123,70,230,129,169,235,150,157,201,173,187,65,148,98,96,201,164,216,155,3,197,193,225,13,65,100,145,118,65,209,4,102,216,130,133,74,151,188,48,71,130,231,209,18,243,200,208,153,203,143,190,36,24,135,186,245,136,21,112,22,248,18,236,85,112,113,208,12,133,118,172,13,34,45,67,64,65,190,121,245,254,221,247,39,118,197,168,92,245,2,53,52,215,110,224,187,119,173,204,114,155,130,206,51,20,122,1,254,213,79,253,137,31,248,228,161,3,132,47,96,159,181,63,53,191,43,235,255,230,205,119,194,228,245,153,239,121,56,180,214,210,31,212,153,82,172,240,145,100,7,197,187,75,248,232,251,41,193,33,44,21,125,41,231,116,217,156,122,173,26,75,141,25,93,58,208,9,241,74,251,140,39,55,240,141,137,169,175,210,214,241,177,126,184,242,67,47,90,213,131,200,69,20,64,125,22,165,36,68,115,124,168,31,175,82,197,253,81,24,197,56,116,4,80,19,47,9,93,197,97,26,5,24,70,127,1,129,156,83,163,124,59,120,112,155,167,9,190,7,116,210,116,57,132,60,154,72,178,192,246,70,129,198,98,115,23,108,140,17,68,150,35,182,32,13,83,218,19,210,40,161,74,2,93,133,220,21,204,99,37,9,180,215,67,4,57,63,223,92,13,234,49,74,96,53,65,71,157,210,44,54,22,59,210,235,147,200,123,96,52,23,90,56,113,96,201,103,153,145,178,215,201,67,140,179,220,88,220,230,91,172,27,176,145,36,10,210,250,52,74,186,8,6,225,0,83,177,206,25,67,1,185,228,158,35,92,83,246,22,3,148,18,72,65,95,252,233,131,96,182,54,182,162,140,41,7,129,166,150,61,73,48,186,179,5,0,86,78,180,36,120,226,147,0,116,161,72,2,52,193,1,115,241,28,249,225,37,70,30,100,201,108,157,230,12,101,121,172,142,104,49,147,112,44,54,44,42,198,14,102,131,102,135,235,103,173,133,94,116,2,130,38,250,97,5,147,189,193,176,12,10,172,44,195,109,14,50,82,29,197,16,177,144,6,79,61,127,9,17,2,49,234,24,100,21,209,125,107,49,15,83,13,242,183,167,17,238,9,77,63,60,80,246,179,204,220,208,182,14,15,116,227,236,116,118,162,249,158,99,0,23,235,242,61,70,22,77,166,45,80,78,143,103,39,192,11,73,234,236,52,141,81,200,198,4,197,65,2,9,27,163,177,223,163,85,130,226,92,218,18,5,11,204,164,81,25,103,167,199,128,255,76,47,27,147,151,118,255,93,107,194,154,250,74,107,110,53,29,88,110,178,32,4,82,102,20,206,163,69,138,33,139,133,142,193,105,44,111,155,5,123,214,52,154,53,44,67,176,47,226,39,153,121,238,52,158,114,102,110,126,46,39,119,0,181,74,102,61,223,227,59,9,137,22,238,140,37,247,86,158,22,192,228,245,56,193,116,85,93,240,29,30,242,136,10,42,19,81,99,208,97,159,216,46,26,76,254,53,130,179,45,98,243,104,73,139,250,156,197,13,179,189,163,159,242,80,164,38,73,3,9,226,48,15,85,158,162,52,64,3,91,140,7,150,98,41,204,15,227,5,113,10,241,224,135,224,92,70,215,104,42,115,12,54,100,18,221,115,131,167,165,149,16,133,29,138,90,76,64,55,193,130,183,194,69,16,128,175,178,121,133,31,100,152,51,71,57,142,222,208,173,245,203,84,161,42,124,51,248,18,55,175,237,190,253,98,125,28,235,255,209,245,11,61,25,145,153,146,245,49,2,237,209,183,100,51,122,14,109,137,77,241,20,9,128,176,227,77,253,100,110,20,147,65,140,60,30,238,252,236,84,211,158,205,8,207,141,16,105,97,150,224,169,99,188,226,254,142,221,82,134,184,250,116,122,140,206,116,9,154,98,254,131,238,102,79,251,79,63,13,209,82,241,1,133,104,240,142,69,230,198,192,63,3,55,85,187,5,142,193,47,241,73,21,123,181,67,166,37,135,252,235,31,255,164,30,129,248,242,159,71,197,142,221,47,193,85,61,160,26,89,178,15,50,122,59,240,18,96,149,252,213,184,130,125,112,209,43,140,151,224,170,228,175,198,53,121,14,215,49,15,52,30,207,140,0,33,89,204,32,116,145,208,144,248,202,13,185,16,127,165,29,185,182,235,230,91,41,68,217,125,25,198,96,127,140,204,229,123,131,44,72,41,163,76,246,71,201,87,204,222,48,139,98,202,56,39,251,227,100,129,186,55,204,130,148,50,74,119,127,148,217,94,181,55,206,146,156,45,181,161,114,213,219,162,26,168,65,155,159,235,88,205,85,53,80,22,150,21,213,153,172,143,4,132,134,101,211,237,78,123,212,209,124,84,175,178,59,226,255,209,93,150,54,142,210,192,247,158,221,113,89,222,122,241,25,146,141,18,21,92,186,229,28,153,220,205,37,8,254,119,228,249,169,155,96,130,69,123,7,108,106,213,155,208,91,38,67,155,251,176,23,52,224,31,221,59,70,179,1,95,220,165,70,249,160,100,168,105,89,77,208,223,222,107,160,183,31,19,64,161,83,179,140,153,38,55,204,8,166,241,170,120,34,176,108,224,130,35,118,54,162,50,166,121,240,113,43,142,233,157,166,73,39,87,228,88,181,220,18,53,29,140,164,139,229,85,12,99,254,128,241,135,44,22,247,8,93,30,84,41,121,8,232,89,138,138,108,77,2,228,222,217,188,8,8,23,243,82,72,243,248,11,217,219,193,246,112,131,218,131,223,144,10,86,222,42,158,190,160,226,56,222,126,224,80,159,149,88,182,99,88,100,190,131,169,182,166,59,245,229,233,255,14,45,56,148,94,241,150,60,186,131,15,233,168,175,245,160,226,10,102,95,6,96,87,55,182,207,91,153,83,82,168,45,233,85,182,48,50,47,138,179,138,53,119,26,203,27,103,89,131,171,162,231,85,43,223,248,169,52,122,98,2,79,168,18,69,22,47,11,147,186,209,113,224,120,163,174,8,48,44,218,154,96,175,195,219,230,58,136,162,184,69,11,129,141,197,46,152,77,3,197,190,65,103,54,16,112,79,253,0,252,111,202,194,128,190,70,208,151,37,237,181,73,102,126,10,120,8,129,76,72,21,51,44,199,1,229,88,133,112,68,162,163,236,98,196,222,176,114,197,96,116,163,166,150,24,52,214,1,142,19,39,209,60,38,166,49,66,19,141,68,217,30,78,191,76,190,26,181,40,1,85,173,63,3,44,250,102,66,245,169,115,38,24,97,66,211,178,214,40,192,9,21,49,67,132,202,212,188,8,167,90,24,17,13,223,251,41,209,90,191,253,102,208,87,153,138,170,70,86,45,226,237,76,220,224,179,219,125,83,191,13,239,66,56,197,176,84,66,145,176,225,50,118,54,234,123,192,28,167,41,130,8,84,174,195,213,151,160,66,110,225,13,110,4,240,188,35,115,70,214,193,98,45,127,85,73,153,215,67,22,35,45,253,16,254,15,245,150,100,182,213,87,7,232,43,60,64,41,203,161,106,82,30,226,79,205,74,190,102,86,245,18,145,243,215,252,148,30,7,217,195,87,246,89,156,103,66,105,138,120,86,209,149,88,22,177,194,160,76,39,107,83,234,166,90,105,194,236,69,68,121,12,225,21,108,107,27,146,216,125,30,74,236,22,176,40,169,0,78,110,219,37,79,119,144,60,221,38,153,30,16,182,139,222,193,126,241,100,155,104,122,146,219,46,58,216,65,116,176,77,52,59,125,109,151,157,236,32,59,169,144,93,120,13,21,117,127,230,121,56,23,17,204,34,141,125,57,14,187,58,165,109,86,111,229,241,238,167,166,222,202,238,131,117,75,62,72,103,243,166,8,196,47,177,50,181,40,215,196,120,229,177,82,8,201,246,222,146,36,63,124,86,86,241,149,59,23,199,228,105,185,46,77,250,50,255,60,140,50,15,164,217,56,239,23,167,191,146,236,198,99,217,91,96,61,35,94,30,10,255,13,214,190,45,179,101,36,0,0 };
