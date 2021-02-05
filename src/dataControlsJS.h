const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<span id='l"+
data.id+
"' class='label label-wrap'>"+
data.value+
"</span>"+
"</div>");break;case UI_BUTTON:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='one columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<button id='btn"+
data.id+
"' "+
"onmousedown='buttonclick("+
data.id+
", true)' "+
"onmouseup='buttonclick("+
data.id+
", false)'>"+
data.value+
"</button></div>");$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});break;case UI_SWITCHER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='one columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<label id='sl"+
data.id+
"' class='switch "+
(data.value=="1"?"checked":"")+
"'>"+
"<div class='in'><input type='checkbox' id='s"+
data.id+
"' onClick='switcher("+
data.id+
",null)' "+
(data.value=="1"?"checked":"")+
"/></div>"+
"</label>"+
"</div>");switcher(data.id,data.value);break;case UI_CPAD:case UI_PAD:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<nav class='control'>"+
"<ul>"+
"<li><a onmousedown='padclick(UP, "+
data.id+
", true)' onmouseup='padclick(UP, "+
data.id+
", false)' id='pf"+
data.id+
"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+
data.id+
", true)' onmouseup='padclick(RIGHT, "+
data.id+
", false)' id='pr"+
data.id+
"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+
data.id+
", true)' onmouseup='padclick(LEFT, "+
data.id+
", false)' id='pl"+
data.id+
"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+
data.id+
", true)' onmouseup='padclick(DOWN, "+
data.id+
", false)' id='pb"+
data.id+
"'>&#9650;</a></li>"+
"</ul>"+
(data.type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+
data.id+
", true)' onmouseup='padclick(CENTER, "+
data.id+
", false)' id='pc"+
data.id+
"'>OK</a>":"")+
"</nav>"+
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});break;case UI_SLIDER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter card-slider "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<div class='range-slider'>"+
"<input id='sl"+
data.id+
"' type='range' min='0' max='100' value='"+
data.value+
"' class='range-slider__range'>"+
"<span class='range-slider__value'>"+
data.value+
"</span>"+
"</div>"+
"</div>");rangeSlider(!sliderContinuous);break;case UI_NUMBER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<input style='color:black;' id='num"+
data.id+
"' type='number' value='"+
data.value+
"' onchange='numberchange("+
data.id+
")' />"+
"</div>");break;case UI_TEXT_INPUT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<input style='color:black;' id='text"+
data.id+
"' value='"+
data.value+
"' onchange='textchange("+
data.id+
")' />"+
"</div>");break;case UI_TAB:$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});break;case UI_SELECT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<select style='color:black;' id='select"+
data.id+
"' onchange='selectchange("+
data.id+
")' />"+
"</div>");break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);parent.append("<option id='option"+
data.id+
"' value='"+
data.value+
"' "+
data.selected+
">"+
data.label+
"</option>");}
break;case UI_MIN:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("max",data.value);}}
break;case UI_STEP:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){parent.attr("step",data.value);}}
break;case UI_GRAPH:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<figure id='graph"+
data.id+
"'>"+
"<figcaption>"+
data.label+
"</figcaption>"+
"</figure>"+
"</div>");graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_GAUGE:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"WILL BE A GAUGE <input style='color:black;' id='gauge"+
data.id+
"' type='number' value='"+
data.value+
"' onchange='numberchange("+
data.id+
")' />"+
"</div>");break;case UI_ACCEL:if(hasAccel)break;var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
hasAccel=true;parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+
data.id+
"' ><div class='ball"+
data.id+
"'></div><pre class='accelerometeroutput"+
data.id+
"'></pre>"+
"</div>");requestOrientationPermission();break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);break;case UPDATE_SLIDER:slider_move($("#id"+data.id),data.value,"100",false);break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}};websock.onmessage=handleEvent;}
var sliderCache={};function sliderchange(number){var val=$("#sl"+number).val();sliderCache[number]!==val&&websock.send("slvalue:"+val+":"+number);sliderCache[number]=val;}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3393] PROGMEM = { 31,139,8,0,102,184,29,96,2,255,237,90,235,115,219,54,18,255,158,191,130,102,58,22,121,86,244,72,226,52,21,77,103,20,89,73,116,85,108,159,45,55,157,75,115,30,72,132,44,78,40,146,5,65,63,170,234,127,191,197,139,4,41,74,86,226,182,119,55,185,15,113,196,197,226,183,139,221,197,226,181,147,40,76,168,113,49,184,28,28,15,70,131,238,240,242,237,197,192,125,218,106,57,19,213,112,214,31,158,116,143,128,214,86,180,211,163,238,168,127,121,242,230,205,121,127,228,182,117,222,209,96,212,31,186,26,225,20,122,150,250,113,82,14,54,184,236,49,202,211,34,83,79,112,61,205,185,94,95,140,70,39,199,238,179,34,159,164,182,91,207,114,206,97,247,53,40,241,188,200,40,136,237,214,243,156,239,252,195,96,212,123,215,63,115,247,139,172,25,189,221,218,215,184,135,131,35,160,189,40,241,10,106,187,245,34,231,60,190,120,255,26,104,223,23,57,37,181,221,250,94,51,87,255,231,17,24,254,244,98,228,190,44,114,107,45,237,214,203,188,199,219,179,238,233,59,247,7,73,232,30,29,9,202,229,233,201,224,152,177,202,134,222,176,223,61,147,204,237,214,15,154,196,238,107,183,93,114,136,160,105,14,57,239,15,251,61,64,43,249,68,145,219,154,87,78,78,71,3,102,255,146,91,20,185,173,185,229,253,0,8,37,175,112,90,91,243,201,251,238,207,110,187,228,15,78,107,235,174,24,245,79,221,118,217,19,156,216,214,252,240,182,123,241,182,239,182,75,126,144,212,182,230,134,110,175,199,98,35,119,193,136,115,74,178,6,144,69,246,209,201,135,227,44,174,135,253,55,163,44,126,207,6,111,223,141,178,40,237,245,143,71,224,116,53,192,222,229,232,226,236,31,23,39,131,243,126,6,213,187,236,191,239,159,117,135,249,60,233,93,158,246,161,215,217,224,39,232,250,52,35,126,232,143,186,231,167,239,186,67,13,254,242,252,226,248,205,240,228,67,65,70,175,123,118,118,50,202,162,186,119,217,29,14,254,217,61,3,83,191,200,72,71,221,179,31,179,8,237,93,30,159,28,247,221,167,251,251,206,53,34,198,21,65,241,236,8,81,228,134,248,198,232,18,130,238,44,155,183,204,80,210,157,76,112,224,78,81,144,96,78,74,2,223,195,164,23,133,212,15,211,40,77,100,211,52,13,39,212,143,66,99,18,5,17,233,5,40,73,44,254,115,224,217,11,249,195,61,78,231,99,76,50,186,147,220,248,116,50,211,248,80,130,117,155,117,8,166,41,9,77,248,243,107,26,249,9,54,29,201,34,77,168,24,240,28,19,20,120,89,115,110,79,197,17,99,138,9,241,175,49,201,152,114,251,42,166,27,76,81,18,207,80,64,51,166,204,222,138,39,73,195,105,16,221,104,56,194,252,170,125,130,8,137,242,254,202,21,170,25,5,254,111,136,248,97,198,192,60,161,26,61,68,62,155,142,135,167,40,13,168,34,154,206,114,249,136,89,254,6,143,147,104,242,217,209,126,131,27,66,60,161,216,43,123,129,224,95,83,156,208,19,226,227,144,34,70,58,197,100,238,39,9,252,178,236,197,242,81,198,153,160,107,252,86,5,0,52,5,209,4,5,231,52,34,232,10,55,18,76,7,20,207,45,19,39,113,234,243,56,73,204,250,223,207,79,142,27,9,133,81,92,249,211,59,43,11,31,219,118,52,96,2,242,35,162,97,251,224,97,30,65,32,209,227,225,86,144,117,85,41,43,7,119,252,169,149,117,221,113,195,52,8,236,69,142,197,149,138,17,73,112,206,101,59,194,134,185,200,143,190,247,9,148,20,228,143,159,202,250,34,66,193,4,223,89,94,52,73,231,96,57,187,129,60,207,50,255,102,218,141,104,58,133,73,241,157,101,62,38,209,13,124,207,232,60,176,76,211,118,164,43,26,147,32,2,217,16,212,2,69,135,134,89,119,14,94,72,147,62,196,6,1,9,107,220,199,208,51,94,144,65,240,60,186,198,98,50,153,124,150,60,185,34,24,135,166,189,194,10,122,22,248,8,246,42,184,132,210,92,11,163,105,28,71,134,212,128,41,185,251,248,229,139,103,251,78,69,175,124,232,5,106,104,45,38,129,63,249,220,145,150,171,47,11,131,158,161,208,11,240,79,126,226,143,253,192,167,119,61,32,92,129,129,22,224,199,157,178,1,118,119,119,148,205,27,51,223,243,112,104,47,50,135,176,9,144,199,171,116,82,198,14,35,239,95,195,143,161,159,80,28,66,122,49,175,51,153,19,46,211,172,87,235,82,231,86,231,129,117,227,135,94,116,211,96,1,201,164,52,226,136,208,29,215,52,127,255,189,186,229,101,107,93,203,243,231,207,50,255,242,124,250,1,143,207,225,55,166,150,121,147,116,154,77,115,175,220,113,22,37,52,68,115,188,103,118,86,27,25,234,158,217,188,1,147,59,75,12,250,126,53,184,196,120,164,2,54,10,163,24,135,174,178,172,133,175,41,75,213,97,18,5,24,250,95,1,164,224,52,24,223,22,49,183,46,54,41,190,5,253,50,95,231,115,38,143,127,74,82,236,44,157,92,53,62,155,182,209,141,51,2,100,121,142,21,208,48,163,109,64,99,132,42,4,177,6,178,216,225,33,118,31,2,99,247,74,201,8,26,26,30,207,69,172,21,187,89,220,142,35,239,142,211,38,240,133,9,4,155,90,15,25,123,131,222,197,88,174,136,197,205,122,167,34,7,49,18,69,227,36,68,215,85,100,80,20,166,6,213,155,32,230,185,152,242,114,14,41,181,188,192,87,242,65,28,113,58,131,38,81,144,52,166,17,233,35,80,30,7,152,13,207,61,228,185,30,86,178,91,97,186,5,99,239,148,150,14,201,108,215,151,142,102,102,43,235,5,62,176,157,49,193,232,179,163,44,33,78,39,157,114,152,19,28,68,200,179,202,220,252,124,210,201,108,78,125,26,96,49,160,0,141,113,192,45,52,71,126,248,14,35,24,158,50,80,206,80,198,227,167,138,14,27,25,120,23,16,51,59,138,207,158,176,134,189,16,159,174,116,128,185,87,193,35,103,179,198,201,157,10,134,21,164,6,138,97,218,193,234,115,224,249,215,134,239,185,53,223,51,247,132,213,125,111,239,145,89,131,216,135,217,231,214,232,77,196,182,93,233,60,76,12,216,126,120,6,21,33,101,0,187,182,29,147,254,130,111,155,245,62,132,86,243,96,182,127,168,64,249,128,25,173,9,196,131,25,105,10,142,36,70,33,23,31,84,75,231,221,12,254,247,201,13,172,214,181,12,240,26,5,41,230,128,12,67,160,53,97,48,135,102,217,172,226,84,247,223,102,87,200,27,127,162,93,199,41,165,145,176,236,152,134,37,13,24,71,20,206,97,162,97,8,243,16,88,56,55,95,111,173,2,111,221,96,185,211,46,116,73,227,141,29,196,178,87,229,39,209,233,48,243,18,179,30,87,78,118,23,171,62,141,210,201,140,175,195,157,60,33,218,11,220,136,9,102,179,246,72,108,96,217,100,212,148,144,16,117,174,174,179,172,115,20,112,196,215,96,200,133,123,89,95,73,15,234,44,255,109,197,146,152,131,76,126,178,102,150,138,165,133,73,179,114,159,187,174,217,54,95,153,224,6,216,66,120,176,251,48,53,169,108,68,178,179,31,214,14,15,252,48,78,169,193,150,37,183,198,123,140,163,219,154,144,89,18,25,133,61,230,45,37,148,109,201,10,1,200,183,238,181,173,116,105,170,88,228,99,230,195,44,230,145,76,132,10,141,28,177,28,26,236,138,169,163,62,216,239,111,42,141,195,214,64,9,146,107,182,236,154,74,139,6,254,225,1,50,10,73,39,70,158,152,119,23,167,117,163,58,235,104,25,103,19,183,76,57,124,252,241,180,56,254,195,221,199,63,188,216,111,57,7,77,4,222,6,53,238,85,135,223,185,124,137,70,213,29,10,74,145,135,42,197,110,133,190,68,167,74,254,130,74,193,67,85,98,183,86,95,162,82,37,127,65,165,241,54,42,53,69,72,229,251,88,215,149,211,239,149,9,186,230,81,56,245,201,188,182,70,119,113,147,86,223,94,121,217,97,163,250,147,146,250,39,63,50,205,85,178,57,104,194,36,41,166,23,54,159,89,188,126,229,250,167,79,137,175,93,255,170,48,244,245,143,171,24,60,92,69,30,143,15,86,178,128,178,162,38,121,184,154,98,42,63,88,207,34,204,138,162,227,135,43,202,39,211,131,245,44,160,172,168,57,121,184,154,114,218,60,88,209,18,206,134,29,26,127,65,249,159,89,124,217,199,19,113,250,253,99,22,98,109,95,69,216,45,148,4,151,253,197,46,171,114,39,39,182,94,188,79,205,152,251,144,44,91,240,63,186,117,107,237,22,252,18,187,168,90,121,83,95,171,18,118,121,41,96,180,19,94,37,23,7,217,230,64,167,231,76,14,113,206,17,172,157,149,251,133,82,52,136,87,178,111,107,43,38,124,156,208,187,128,109,165,89,247,206,56,64,147,207,142,88,165,194,116,94,233,249,144,191,158,108,112,52,156,146,248,189,166,98,21,95,197,205,55,172,132,205,77,39,241,252,29,242,255,62,209,124,194,174,48,75,106,109,225,6,214,235,171,156,208,125,221,41,94,233,101,86,42,108,247,216,254,7,120,212,33,95,9,96,248,51,130,167,110,77,115,11,163,51,107,104,202,154,249,182,173,234,174,112,197,51,101,176,76,123,214,143,249,182,81,4,128,175,49,246,122,226,219,90,4,81,20,119,216,250,178,180,249,141,168,101,162,216,23,146,17,112,79,253,0,188,105,101,235,13,123,0,224,143,55,223,89,116,230,39,160,15,165,112,146,100,3,51,109,23,142,139,143,249,184,159,208,232,137,56,4,154,206,146,175,130,38,167,155,117,125,229,98,177,12,234,184,49,137,230,49,181,204,17,26,27,52,50,228,169,24,126,89,98,202,24,17,129,161,218,175,196,13,233,14,27,79,67,48,65,15,11,62,109,123,129,2,76,24,196,12,81,134,105,120,17,78,140,48,162,6,190,245,19,106,116,126,249,133,199,119,197,98,185,186,22,242,23,246,111,236,62,17,7,120,178,97,170,137,246,149,43,5,53,167,68,243,215,204,42,81,163,208,89,99,225,220,7,220,118,74,139,42,67,151,237,25,197,252,101,138,41,47,126,110,155,41,20,69,8,195,140,187,194,122,2,243,144,187,178,56,162,247,131,109,135,195,92,189,202,182,103,26,60,239,137,104,151,195,74,252,223,176,101,171,144,147,179,14,182,27,102,225,106,101,185,162,75,247,231,191,74,23,116,123,143,46,172,52,228,47,82,38,161,56,190,71,27,94,149,243,109,205,241,169,127,149,18,204,21,224,15,248,165,99,183,226,153,32,25,218,43,104,197,70,65,0,196,226,244,206,106,3,62,74,240,79,238,74,209,129,58,30,57,96,30,216,136,242,134,243,235,43,107,181,111,221,148,170,102,93,52,55,150,10,175,184,55,105,226,190,71,116,214,32,81,10,38,103,175,177,128,7,17,194,42,25,70,254,28,126,53,97,99,222,170,210,179,17,167,201,204,90,220,118,104,82,191,235,228,209,3,11,68,169,30,227,129,122,107,117,97,157,10,115,125,252,244,7,203,83,85,88,223,84,184,127,24,12,135,198,235,190,209,53,248,216,141,251,54,147,87,40,189,194,255,185,45,62,47,115,99,233,81,85,119,217,162,253,79,118,89,86,75,198,223,250,255,171,28,200,45,98,52,89,69,12,156,189,231,177,120,149,198,158,49,77,131,224,110,71,63,178,35,54,4,12,187,72,86,79,38,220,201,73,37,173,15,245,62,99,20,148,175,114,133,107,14,96,135,88,9,28,165,148,173,62,229,62,113,57,3,110,46,243,42,248,93,171,138,229,103,11,253,198,48,127,244,174,120,56,41,214,200,118,54,60,184,192,158,188,101,190,106,117,218,149,0,226,218,71,222,44,176,186,38,171,176,254,130,30,26,84,221,132,212,105,170,91,164,85,52,121,109,192,16,248,113,57,27,10,244,190,103,36,218,9,151,7,50,63,217,125,65,127,185,103,95,217,36,110,215,91,190,116,235,164,252,89,42,47,133,238,172,246,20,121,149,137,149,249,227,11,164,138,41,47,232,170,188,80,85,175,240,218,24,203,188,8,63,135,209,77,200,19,17,59,5,241,163,75,150,59,150,143,84,74,96,237,135,110,161,36,124,119,55,107,57,40,214,170,136,237,150,42,9,41,251,219,209,49,93,183,16,40,246,66,246,42,22,193,137,240,121,146,21,132,202,114,212,39,178,0,84,125,230,213,158,138,146,151,118,42,74,86,200,169,8,162,114,83,125,101,133,154,182,163,52,201,235,157,36,143,185,87,157,122,84,22,172,28,66,174,187,82,90,211,86,87,51,211,79,42,182,73,163,181,122,44,245,50,168,57,78,18,4,139,135,86,107,227,136,218,82,121,77,135,96,102,187,139,101,94,71,42,232,114,149,17,75,142,240,41,132,154,56,39,65,30,145,116,30,135,176,129,201,161,62,138,150,79,59,174,11,109,187,187,74,147,132,231,252,36,224,241,218,49,247,224,127,94,251,38,129,170,32,24,128,94,93,88,88,254,170,20,227,105,161,168,89,81,124,184,78,186,38,69,187,192,169,146,33,114,199,38,33,116,27,33,234,6,167,82,4,91,103,55,74,64,227,45,100,20,78,205,149,126,148,169,108,147,168,100,11,65,122,181,134,104,174,251,9,123,218,227,117,159,242,103,17,118,204,104,26,26,155,58,70,137,37,141,171,197,101,207,15,44,133,212,75,2,101,57,157,86,73,39,30,41,58,235,52,137,39,247,171,18,79,10,186,20,114,237,122,224,233,22,192,211,117,192,236,13,104,61,244,22,230,139,199,235,160,217,115,221,122,232,96,11,232,96,29,52,127,97,91,143,77,182,192,38,21,216,133,90,96,181,23,145,142,79,96,15,132,121,160,241,95,174,44,18,135,111,30,226,121,116,251,144,135,59,170,212,196,206,234,117,85,156,35,128,191,198,154,232,114,166,211,42,95,21,72,177,56,87,33,249,225,189,88,197,34,239,28,142,227,25,249,88,218,172,48,253,126,53,202,60,49,137,226,188,93,189,239,149,176,91,171,216,107,212,186,7,94,61,251,137,85,69,123,9,202,75,103,253,228,200,79,38,4,83,121,45,43,146,61,191,62,214,31,159,76,187,206,63,87,26,228,219,21,180,139,77,231,106,59,167,155,106,25,105,96,86,150,170,93,43,243,230,10,42,49,20,160,184,111,102,215,183,150,186,118,86,152,170,145,111,153,213,141,143,184,44,214,7,198,21,226,79,178,252,44,216,209,4,21,150,212,226,221,54,108,142,152,225,99,56,43,98,171,249,175,95,142,246,154,87,117,211,180,229,59,170,136,176,13,208,10,45,100,5,215,82,71,70,201,174,166,234,66,236,31,164,14,251,231,252,27,20,235,110,1,80,56,0,0 };
