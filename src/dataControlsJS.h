const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_TITEL=0;const UI_BUTTON=1;const UI_PAD=2;const UI_CPAD=3;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const FOR=0;const BACK=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
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
e.preventDefault();});break;case UI_SELECT:var element;if(data.parentControl){element=$("#tab"+data.parentControl);}else{element=$("#row")}
element.append("<div class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5 id='"+
data.id+
"'>"+
data.label+
"</h5><hr />"+
"<select style='color:black;' id='select"+
data.id+
"' onchange='selectchange("+
data.id+
")' />"+
"</div>");break;case UI_OPTION:if(data.parentControl){var element=$("#select"+data.parentControl);element.append("<option id='option"+data.id+"' value='"+data.value+"'>"+data.label+"</option>");}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);break;default:console.error("Unknown type or event");break;}};websock.onmessage=handleEvent;}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);console.log(val);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);console.log("tvalue:"+val+":"+number);console.log(val);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);console.log("svalue:"+val+":"+number);console.log(val);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case FOR:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case BACK:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
)=====";

const uint8_t JS_CONTROLS_GZIP[2514] PROGMEM = { 31,139,8,0,92,132,125,92,2,255,237,90,255,114,219,184,17,254,223,79,193,99,50,33,89,203,178,148,156,115,57,201,244,141,44,43,137,46,58,201,149,229,75,167,115,51,26,136,132,34,142,41,146,37,32,43,169,71,175,113,15,210,71,234,147,116,1,16,32,72,83,182,28,77,211,222,180,255,216,196,98,241,225,195,238,98,241,75,94,28,17,106,92,247,167,253,97,127,210,239,12,166,239,174,251,238,203,70,163,237,201,138,73,127,210,27,184,154,224,252,122,50,25,13,221,102,46,185,236,92,184,47,243,98,151,149,95,229,229,65,231,28,16,190,151,130,203,139,206,164,151,9,155,141,239,115,189,171,143,253,73,247,125,111,236,158,20,85,149,188,217,56,209,180,7,253,11,144,189,46,233,10,105,179,241,58,215,28,94,255,114,14,178,31,138,154,153,180,217,248,65,27,107,239,47,19,176,196,229,245,196,125,83,212,214,106,154,141,55,121,139,119,227,206,229,123,247,199,76,208,185,184,16,146,233,229,168,63,100,170,89,69,119,208,235,140,51,229,102,227,71,173,199,206,185,219,108,150,250,226,50,205,190,87,189,65,175,11,104,47,75,99,205,196,77,205,246,163,203,73,159,57,231,85,81,85,138,149,252,237,104,172,124,122,222,233,126,80,238,28,244,222,78,148,51,199,253,119,239,39,202,149,221,222,112,2,22,147,30,235,78,39,215,227,63,95,143,250,87,61,5,213,157,246,126,233,141,59,131,11,133,215,157,94,246,160,213,184,255,43,52,125,169,132,31,123,147,206,213,229,251,206,64,131,159,94,93,15,223,14,70,31,11,125,116,59,227,241,104,162,66,162,59,237,12,250,127,237,140,251,67,229,249,238,244,162,51,254,160,220,219,157,14,71,195,158,251,242,228,164,61,95,69,30,13,226,200,240,226,48,78,187,33,34,196,230,159,125,223,185,203,62,220,225,106,57,195,169,146,183,201,58,160,222,66,211,67,4,235,99,109,165,152,174,210,200,132,63,127,91,197,1,193,102,59,83,201,134,46,21,240,18,167,40,244,85,117,110,7,169,145,96,138,211,52,184,197,169,82,202,237,34,149,214,152,34,146,44,80,72,149,146,178,147,212,33,171,104,30,198,107,13,71,152,77,214,123,40,77,227,188,189,52,161,172,70,97,240,119,148,6,145,82,96,22,148,149,62,74,111,204,182,143,231,104,21,82,41,52,219,155,205,193,45,74,141,53,158,145,216,187,105,107,223,221,56,138,176,71,177,239,206,81,72,112,238,133,20,19,138,82,106,59,119,207,109,63,246,86,75,28,81,167,142,124,223,54,255,100,58,245,120,62,183,157,246,115,219,124,150,198,107,40,47,232,50,180,77,211,105,103,184,117,47,140,9,6,149,12,165,189,57,208,28,28,93,81,68,87,164,7,3,77,161,135,45,92,24,186,210,133,62,82,188,140,111,177,136,12,147,187,252,232,83,138,113,100,58,247,84,129,103,65,47,197,126,133,150,32,205,89,24,199,198,48,54,50,6,140,228,139,103,111,94,191,58,105,87,180,202,135,94,144,70,246,157,23,6,222,77,43,179,220,166,48,230,5,138,252,16,255,26,144,96,22,132,1,253,210,5,193,39,176,207,93,48,183,191,43,143,255,197,139,239,164,201,235,139,192,247,113,228,220,41,127,48,103,42,88,233,35,165,14,3,239,221,194,199,32,32,20,71,48,85,204,91,213,167,199,251,52,107,213,92,106,220,232,202,129,110,132,215,198,71,60,187,130,111,76,109,115,77,90,199,199,230,225,58,136,252,120,93,15,99,15,49,2,245,69,76,104,132,150,248,208,60,94,19,205,253,113,20,39,56,114,37,81,27,223,82,54,139,35,18,135,24,90,127,2,64,161,105,48,189,29,60,184,205,211,20,127,6,118,202,116,57,133,60,154,104,186,194,237,141,70,141,199,230,46,220,184,34,64,150,35,182,128,134,153,236,1,52,38,168,66,96,179,80,184,130,123,172,132,192,106,125,68,145,251,243,213,104,88,79,80,10,179,9,42,234,76,230,240,182,216,85,94,159,197,254,23,46,243,160,132,83,23,166,124,150,25,153,122,157,126,73,112,150,27,139,155,135,22,175,6,110,52,141,67,82,159,199,105,15,65,35,28,98,6,235,158,113,22,144,75,62,11,134,119,76,189,197,9,17,10,41,232,83,48,255,34,149,157,77,91,27,140,173,26,193,72,157,246,44,197,232,166,45,9,240,77,74,75,145,167,1,13,97,44,140,73,136,102,56,228,46,94,162,32,122,143,145,15,89,50,155,167,185,66,25,143,239,78,90,220,36,130,75,27,38,21,87,7,179,65,177,43,198,231,220,201,113,177,14,40,154,153,135,21,74,237,13,134,105,80,80,229,25,110,115,144,137,234,40,129,136,133,52,120,234,7,183,16,33,16,163,174,69,215,49,91,183,86,203,136,24,144,191,125,131,10,79,24,230,225,129,182,158,101,230,134,178,115,120,96,90,103,167,139,19,35,240,93,11,180,120,85,224,115,177,44,242,209,130,228,244,120,113,2,186,144,164,206,78,73,130,34,222,38,44,54,146,76,120,27,131,255,61,90,167,40,201,209,110,81,184,194,28,141,97,156,157,30,3,255,51,179,108,76,177,97,252,207,90,19,230,212,87,90,115,171,233,192,114,179,21,165,144,50,227,104,25,175,8,134,44,22,185,150,144,241,188,109,23,236,89,51,88,214,112,44,169,190,74,30,84,22,185,211,122,200,153,185,249,5,78,238,0,102,149,204,122,129,47,86,18,26,175,188,5,79,238,173,60,45,128,201,235,73,138,217,172,186,16,43,60,228,17,157,84,6,81,227,212,97,157,216,14,13,38,255,26,224,108,137,216,220,155,210,114,215,207,227,134,219,222,53,79,69,40,50,147,144,80,145,56,204,67,85,164,40,3,216,192,18,227,131,165,120,10,11,162,100,69,221,66,60,4,17,56,151,203,13,150,202,92,139,55,153,197,159,133,193,73,105,38,196,81,151,177,150,29,176,69,176,224,173,104,21,134,224,171,172,95,233,7,21,230,220,81,174,107,54,76,231,238,105,67,97,67,248,102,244,21,111,177,183,251,246,147,245,126,172,255,91,231,47,212,100,66,110,74,94,199,5,172,198,220,146,205,216,233,182,37,23,197,83,36,9,194,138,55,15,210,165,85,76,6,9,242,69,184,139,179,83,205,120,52,35,60,214,66,166,133,69,138,231,174,245,76,248,59,241,74,25,98,244,225,244,24,157,153,138,52,227,252,7,93,205,30,246,159,121,26,161,91,205,7,140,162,37,42,86,153,27,195,224,12,220,84,237,22,56,6,63,197,39,85,234,213,14,153,151,28,242,207,223,255,193,60,2,241,21,60,206,138,31,187,159,194,171,186,65,53,179,116,31,102,236,118,224,41,196,42,245,171,121,133,251,240,98,87,24,79,225,85,169,95,205,107,246,24,175,99,17,104,34,158,185,0,66,178,152,65,216,36,97,33,241,149,11,114,33,254,74,43,114,109,215,197,183,18,68,91,125,57,199,112,127,142,220,229,123,147,44,160,148,89,166,251,179,20,51,102,111,154,69,152,50,207,217,254,60,121,160,238,77,179,128,82,102,233,237,207,50,91,171,246,230,89,194,217,178,55,212,46,144,91,108,4,122,208,230,231,58,190,231,170,106,168,54,150,21,187,51,181,63,146,20,26,78,155,45,119,198,189,138,230,189,253,42,191,121,254,47,93,101,89,225,136,132,129,255,232,138,203,243,214,147,207,144,188,149,220,193,145,45,231,200,244,102,169,72,136,127,71,126,64,188,20,83,44,203,59,112,211,119,189,41,187,101,178,140,101,0,107,65,3,254,163,207,174,213,108,192,151,112,169,85,62,40,89,122,90,214,19,244,183,247,26,140,59,72,40,176,48,153,89,166,124,36,87,220,8,182,245,172,120,34,112,218,160,5,71,236,172,69,101,76,139,224,19,86,156,178,59,77,155,117,174,225,56,181,220,18,53,19,140,100,202,233,85,12,99,241,44,242,135,220,44,238,17,186,34,168,8,253,18,178,179,20,131,108,205,66,228,221,180,197,38,32,90,45,75,33,45,226,47,226,111,7,219,195,13,246,30,226,134,84,170,138,82,241,244,5,59,142,227,237,7,14,253,177,138,103,59,206,69,229,59,232,106,107,186,211,223,179,254,239,208,130,67,217,21,111,201,163,59,248,144,181,250,90,15,106,174,224,246,229,4,118,117,99,231,188,149,57,133,192,222,146,93,101,75,35,139,77,113,182,99,205,157,198,243,198,89,86,16,67,49,243,93,171,88,248,25,26,59,49,129,39,116,68,153,197,203,96,106,108,172,29,56,222,170,107,0,150,195,74,51,236,119,69,217,190,11,227,56,105,177,141,192,198,225,23,204,182,133,146,192,98,61,91,8,180,231,65,8,254,183,213,198,128,189,70,176,151,37,227,185,77,23,1,1,62,148,66,38,100,3,179,28,215,133,193,241,29,194,17,141,143,178,139,145,246,134,111,87,44,46,183,106,250,22,131,197,58,208,113,147,52,94,38,212,182,38,104,102,208,56,91,195,217,151,45,102,163,17,167,48,84,231,39,160,197,222,76,216,120,234,66,9,90,216,80,116,156,59,20,226,148,65,44,16,101,152,134,31,99,98,68,49,53,240,231,128,80,163,245,219,111,22,123,149,169,216,213,220,191,209,226,239,181,255,107,51,145,0,29,239,129,169,40,234,239,93,88,201,57,39,170,159,62,235,228,83,120,107,139,137,53,47,112,219,73,26,85,150,190,103,208,56,225,79,101,140,190,248,44,92,224,169,76,162,207,62,53,27,133,157,128,180,104,201,120,111,14,42,86,116,17,44,247,168,109,207,22,242,137,86,62,20,241,71,36,219,188,142,110,34,56,44,243,21,139,5,60,143,82,101,172,141,254,236,180,196,132,32,48,186,246,234,162,63,56,22,150,48,81,16,118,4,26,174,90,154,178,10,78,50,127,188,35,220,112,17,39,220,50,15,225,255,161,217,82,202,109,253,113,11,234,10,239,156,90,214,173,234,84,100,210,135,122,165,187,244,186,155,86,153,91,33,58,171,216,73,223,61,196,143,236,196,143,124,13,63,253,198,93,232,215,2,194,238,78,248,43,113,246,89,100,51,99,50,13,158,31,127,74,42,171,68,83,208,186,83,7,57,22,108,181,82,135,217,243,161,246,114,40,142,123,173,109,76,18,239,113,42,137,87,224,162,205,163,183,163,7,144,231,59,32,207,183,33,179,211,244,118,232,29,236,151,204,182,65,179,107,143,237,208,225,14,208,225,54,104,126,85,177,29,59,221,1,59,173,192,46,252,116,64,30,146,51,207,19,138,40,230,145,198,191,92,151,191,51,176,50,159,26,249,172,8,136,109,182,178,199,19,211,81,191,222,144,243,3,1,252,45,214,186,150,103,27,217,94,123,217,151,32,217,242,88,66,10,162,71,177,138,63,9,201,225,56,158,145,143,165,201,126,198,242,56,141,178,14,236,73,146,188,94,94,149,148,176,27,247,177,183,208,122,4,94,221,160,252,11,4,101,189,176,232,39,0,0 };
