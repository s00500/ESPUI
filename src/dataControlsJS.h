const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"'":"";elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"'":"";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<span id='l"+
data.id+
"' "+elementStyle+" class='label label-wrap'>"+
data.value+
"</span>"+
"</div>");}
break;case UI_BUTTON:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='one columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<button id='btn"+
data.id+
"' "+elementStyle+" "+
"onmousedown='buttonclick("+
data.id+
", true)' "+
"onmouseup='buttonclick("+
data.id+
", false)'>"+
data.value+
"</button></div>");$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='one columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<label id='sl"+
data.id+
"' "+elementStyle+" class='switch "+
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
"</div>");switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
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
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter card-slider "+
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
"' "+elementStyle+" class='range-slider__range'>"+
"<span class='range-slider__value'>"+
data.value+
"</span>"+
"</div>"+
"</div>");rangeSlider(!sliderContinuous);}
break;case UI_NUMBER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<input style='color:black;' "+elementStyle+" id='num"+
data.id+
"' type='number' value='"+
data.value+
"' onchange='numberchange("+
data.id+
")' />"+
"</div>");}
break;case UI_TEXT_INPUT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<input style='color:black;' "+elementStyle+" id='text"+
data.id+
"' value='"+
data.value+
"' onchange='textchange("+
data.id+
")' />"+
"</div>");}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_SELECT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"<select style='color:black;' "+elementStyle+" id='select"+
data.id+
"' onchange='selectchange("+
data.id+
")' />"+
"</div>");}
break;case UI_OPTION:if(data.parentControl){var parent=$("#select"+data.parentControl);parent.append("<option id='option"+
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
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
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
"</div>");graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_GAUGE:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
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
"</div>");}
break;case UI_ACCEL:if(hasAccel)break;var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
hasAccel=true;if(data.visible){parent.append("<div id='id"+
data.id+
"' "+panelStyle+" class='two columns card tcenter "+
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
"</div>");requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:slider_move($("#id"+data.id),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}
$(".range-slider__range").each(function(){$(this)[0].value=$(this).attr("value");$(this).next().html($(this).attr("value"));});};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var slidercb=function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));};range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){$(this).next().html(this.value);if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:slidercb});}else{$(this).on({change:slidercb});}
$(this).attr("callbackSet","true");}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3564] PROGMEM = { 31,139,8,0,182,30,210,97,2,255,237,91,109,115,219,54,18,254,158,95,65,51,157,136,60,43,122,73,226,52,149,76,103,20,89,73,212,42,182,207,150,155,206,165,57,15,68,66,22,199,20,201,146,160,95,234,234,191,223,226,141,4,40,74,86,236,180,157,222,228,139,45,46,22,139,7,187,11,236,2,92,186,81,152,18,227,116,120,54,60,24,142,135,189,209,217,187,211,161,243,172,213,234,186,178,225,120,48,58,236,237,3,173,45,105,71,251,189,241,224,236,240,237,219,147,193,216,105,171,188,131,95,198,131,131,125,46,163,173,208,199,195,241,96,228,40,132,35,144,88,146,199,72,197,32,195,179,62,165,60,211,153,250,156,235,89,193,245,230,116,60,62,60,112,158,235,124,130,218,110,61,47,56,71,189,55,0,226,133,206,200,137,237,214,139,130,239,228,227,112,220,127,63,56,118,118,116,214,156,222,110,237,40,220,163,225,62,208,94,150,120,57,181,221,122,89,112,30,156,126,120,3,180,239,117,78,65,109,183,190,87,212,5,122,4,131,28,157,142,157,87,58,183,210,210,110,189,42,122,188,59,238,29,189,119,126,16,132,222,254,62,167,156,29,29,14,15,40,171,104,232,143,6,189,99,193,220,110,253,160,140,216,123,227,180,75,6,225,52,197,32,39,131,209,160,15,210,74,54,145,228,182,98,149,195,163,241,144,234,191,100,22,73,110,43,102,249,48,4,66,201,42,140,214,86,108,242,161,247,139,211,46,217,131,209,218,170,41,198,131,35,167,93,182,4,35,182,21,59,188,235,157,190,27,56,237,146,29,4,181,173,152,161,215,239,83,223,40,76,48,102,156,130,172,8,200,61,123,255,240,227,65,238,215,163,193,219,113,238,191,199,195,119,239,199,185,151,246,7,7,99,48,186,156,96,255,108,124,122,252,239,211,195,225,201,32,23,213,63,27,124,24,28,247,70,197,58,233,159,29,13,160,215,241,240,103,232,250,44,39,126,28,140,123,39,71,239,123,35,69,252,217,201,233,193,219,209,225,71,109,140,126,239,248,248,112,156,123,117,255,172,55,26,254,167,119,12,170,126,153,147,246,123,199,63,229,30,218,63,59,56,60,24,56,207,118,118,186,151,40,49,206,19,20,207,246,17,65,78,136,175,140,94,146,160,27,203,102,45,51,148,246,92,23,7,206,20,5,41,102,164,52,240,61,156,244,163,144,248,97,22,101,169,104,154,102,161,75,252,40,52,220,40,136,146,126,128,210,212,98,63,135,158,125,43,126,56,7,217,124,130,147,156,222,77,175,124,226,206,20,62,148,98,85,103,157,4,147,44,9,77,248,243,91,22,249,41,54,187,130,69,168,80,50,224,57,78,80,224,229,205,133,62,37,71,140,9,78,18,255,18,39,57,83,161,95,201,116,133,9,74,227,25,10,72,206,148,235,91,242,164,89,56,13,162,43,69,14,87,191,108,119,81,146,68,69,127,105,10,217,140,2,255,119,148,248,97,206,64,45,33,27,61,148,92,152,93,15,79,81,22,16,73,52,187,139,197,35,170,249,43,60,73,35,247,162,171,252,6,51,132,216,37,216,43,91,33,193,191,101,56,37,135,137,143,67,130,40,233,8,39,115,63,77,225,151,101,223,46,30,229,156,41,186,196,239,164,3,64,83,16,185,40,56,33,81,130,206,113,35,197,100,72,240,220,50,113,26,103,62,243,147,212,172,255,120,114,120,208,72,9,204,226,220,159,222,88,185,251,216,118,87,17,156,192,248,81,162,200,246,193,194,204,131,96,68,143,185,155,54,214,121,229,88,133,240,174,63,181,242,174,91,78,152,5,129,125,91,200,98,160,98,148,164,184,224,178,187,92,135,197,144,159,124,239,51,128,228,228,79,159,203,120,81,66,64,5,223,89,94,228,102,115,208,156,221,64,158,103,153,255,50,237,70,52,157,194,162,248,206,50,31,39,209,21,60,207,200,60,176,76,211,238,10,83,52,220,32,130,177,193,169,185,20,85,52,172,186,19,176,66,150,14,192,55,18,24,97,133,249,168,244,156,23,198,72,240,60,186,196,124,49,153,108,149,60,61,79,48,14,77,123,137,21,112,106,124,9,246,42,184,56,104,134,194,104,26,7,145,33,16,80,144,79,30,191,122,249,124,167,91,209,171,152,186,70,13,173,91,55,240,221,139,142,208,92,125,161,77,122,134,66,47,192,63,251,169,63,241,3,159,220,244,129,112,14,10,186,5,59,110,149,21,240,228,201,150,212,121,99,230,123,30,14,237,219,220,32,116,1,20,254,42,140,148,179,195,204,7,151,240,99,228,167,4,135,176,189,152,151,249,152,46,27,211,172,87,99,169,51,173,51,199,186,242,67,47,186,106,80,135,164,163,52,226,40,33,91,142,105,254,241,71,117,203,171,214,170,150,23,47,158,231,246,101,251,233,71,60,57,129,223,152,88,230,85,218,105,54,205,237,114,199,89,148,146,16,205,241,182,217,89,110,164,82,183,205,230,21,168,188,187,192,128,247,222,194,133,140,71,210,97,163,48,138,113,232,72,205,90,248,146,208,173,58,76,163,0,67,255,115,16,201,57,13,202,183,129,207,173,242,77,130,175,1,95,110,235,98,205,20,254,79,146,12,119,23,221,2,26,91,77,155,96,99,140,32,178,188,198,52,105,152,210,214,72,163,132,42,9,60,6,82,223,97,46,118,151,4,202,238,149,54,35,104,104,120,108,47,162,173,216,201,253,118,18,121,55,140,230,194,19,78,192,217,186,49,10,49,236,135,55,1,176,65,151,6,132,223,195,171,240,40,1,253,39,228,198,170,21,237,53,251,181,9,75,129,114,214,204,109,198,92,52,110,155,53,240,36,179,139,3,76,71,90,35,80,229,168,16,169,54,75,161,34,102,179,118,114,19,99,17,181,245,131,70,167,98,159,164,36,130,38,105,136,46,171,200,160,76,88,190,68,109,130,117,201,134,41,167,28,176,237,151,147,144,74,62,240,117,70,167,162,147,40,72,27,211,40,25,32,0,47,230,229,236,177,120,4,209,246,154,155,247,150,178,119,74,225,77,48,219,245,69,87,113,5,43,239,5,126,98,119,39,9,70,23,93,169,137,226,180,212,249,123,0,240,163,93,167,188,23,36,56,136,144,103,149,185,217,33,174,147,59,38,241,137,244,151,0,77,112,192,76,52,71,126,248,30,35,208,175,180,80,193,80,150,199,142,94,29,58,51,88,2,32,49,55,36,127,236,115,109,216,183,252,209,17,30,144,187,177,202,35,182,60,133,147,121,21,88,86,202,100,155,125,128,165,180,6,138,97,179,130,152,189,235,249,151,134,239,57,53,223,51,183,185,31,248,222,246,35,179,102,152,219,234,82,129,13,4,182,48,167,70,174,34,154,187,102,243,48,53,32,135,243,12,194,215,37,176,63,82,114,90,97,79,120,182,169,176,61,104,53,119,103,59,123,114,12,166,16,74,107,2,113,119,150,52,57,71,10,67,50,52,193,18,24,125,145,73,56,76,142,193,254,62,189,130,28,168,150,143,112,137,130,12,179,17,168,80,46,190,9,147,221,99,106,209,45,193,79,203,255,52,83,192,126,253,39,154,98,146,17,18,113,99,76,72,120,151,57,104,151,40,156,195,110,130,97,41,133,208,135,117,103,137,143,165,117,174,27,52,136,217,53,181,75,22,175,237,192,243,143,42,211,242,78,123,185,97,169,186,25,90,209,157,167,95,36,202,220,25,75,136,58,69,100,178,111,113,35,78,48,221,25,246,249,73,130,46,120,5,132,16,81,103,112,187,139,58,147,2,134,186,143,12,145,65,45,120,238,167,59,159,188,86,249,230,126,170,251,241,133,77,225,164,155,238,5,60,228,210,225,173,194,77,28,199,108,155,175,77,176,28,164,127,30,13,205,10,12,58,99,209,217,15,107,123,187,126,24,103,196,160,225,218,169,177,30,147,232,186,198,65,148,48,68,97,159,26,88,14,74,211,105,205,103,217,177,171,182,17,150,166,116,95,166,4,54,111,125,183,202,135,144,222,84,72,92,246,38,122,65,216,145,15,244,247,183,248,162,120,21,36,85,114,32,145,108,136,174,153,208,121,224,239,237,34,67,219,201,98,228,241,197,124,122,84,55,170,183,50,101,27,91,199,45,246,49,166,142,120,170,171,99,239,201,227,31,94,238,180,186,187,77,4,254,0,48,238,132,195,110,212,190,4,81,117,7,13,84,242,80,80,244,206,239,75,48,85,242,107,144,130,135,66,162,119,146,95,2,169,146,95,131,52,217,4,82,147,187,84,113,2,112,28,177,60,95,155,128,181,240,194,169,159,204,107,43,176,243,123,210,250,230,224,69,135,181,240,221,18,252,195,159,40,114,185,29,237,54,97,145,232,27,16,93,239,212,95,239,25,84,213,37,113,223,160,90,37,67,13,170,12,98,240,112,136,204,31,31,12,82,147,178,4,51,121,56,76,190,148,31,140,83,23,179,4,116,242,112,160,108,49,61,24,167,38,101,9,166,251,112,152,98,217,60,24,104,73,206,218,180,143,189,33,251,191,9,207,244,225,41,191,89,248,58,161,90,201,205,18,122,11,41,132,139,254,60,83,171,76,15,121,250,198,250,212,140,185,15,219,105,11,254,163,107,167,214,110,193,47,158,137,213,202,103,137,213,105,165,58,250,217,25,151,171,156,86,43,185,152,212,77,206,162,234,54,203,68,156,48,9,214,214,210,101,206,146,3,241,23,167,223,242,59,213,105,184,91,136,219,57,214,189,51,9,144,123,209,173,48,47,5,29,102,243,74,239,9,217,27,184,53,206,2,7,60,118,55,46,89,249,147,126,8,128,120,219,92,127,239,80,188,205,254,102,198,135,152,145,222,156,151,96,111,96,57,218,235,158,118,235,189,233,44,105,84,191,184,205,21,171,165,166,52,87,3,30,121,203,33,199,164,67,206,18,60,117,106,138,105,41,157,106,76,193,111,22,41,102,213,141,240,146,49,203,194,242,9,209,126,212,63,26,186,0,120,154,96,175,207,159,173,219,32,138,226,14,141,133,11,155,221,205,91,38,138,125,62,50,2,238,169,31,128,197,173,60,54,210,87,81,236,53,226,119,22,153,249,41,224,33,4,206,197,116,98,166,237,192,225,247,49,155,247,83,18,61,229,71,90,179,187,96,17,219,100,116,179,174,70,89,186,30,0,142,19,39,209,60,38,150,57,70,19,131,68,134,56,227,195,47,139,47,60,35,74,96,170,246,107,126,15,190,69,231,211,224,76,208,195,130,71,219,190,69,1,78,168,136,25,34,84,166,225,69,56,53,194,136,24,248,218,79,137,209,249,245,87,102,227,138,192,94,21,183,89,181,199,183,245,170,93,219,194,154,116,191,100,193,242,14,75,119,42,114,101,242,230,251,173,77,94,98,211,89,97,148,194,108,76,221,18,71,149,109,202,26,143,98,246,98,149,194,231,63,55,221,113,36,133,15,134,41,119,133,66,185,204,170,25,125,24,110,58,29,234,12,203,108,84,231,116,67,229,75,68,76,43,245,127,199,150,93,248,21,91,170,144,45,153,250,237,210,18,150,222,47,127,21,22,116,125,7,22,90,217,244,23,129,73,9,142,239,64,195,138,202,190,109,11,234,182,48,245,207,179,4,51,60,172,62,165,116,239,32,121,92,36,92,127,73,154,222,200,9,32,81,223,0,242,210,151,79,66,248,103,103,169,166,70,158,15,187,160,45,72,171,89,195,201,229,185,181,220,183,110,10,168,121,23,205,208,165,202,66,102,111,146,58,31,16,153,53,146,40,3,27,208,114,3,144,8,62,68,75,117,198,254,28,126,53,225,228,209,170,66,218,136,179,116,102,221,94,119,72,90,191,233,20,254,5,113,167,84,112,116,31,228,10,110,165,240,177,83,161,176,79,159,191,242,120,178,204,240,219,130,80,22,196,199,225,104,100,188,25,24,61,131,233,198,88,151,231,178,69,131,178,115,252,119,158,79,88,165,39,221,98,101,129,163,205,219,255,100,163,230,229,148,172,220,229,31,101,98,166,49,163,73,139,198,136,225,207,99,158,250,96,207,152,102,65,112,179,165,222,106,32,58,69,12,233,45,45,185,228,6,103,164,210,36,246,212,62,19,20,148,239,195,185,241,118,33,117,173,20,28,101,132,70,184,114,159,184,188,139,174,175,132,44,121,134,82,58,206,74,88,212,139,215,162,232,65,68,74,105,191,245,69,53,252,252,164,10,146,145,23,154,69,232,85,123,84,67,202,223,165,174,121,103,6,7,145,150,249,186,213,105,127,25,180,244,193,216,248,133,159,184,34,162,21,139,150,150,154,128,92,5,101,221,132,152,97,42,165,119,127,29,80,113,177,68,101,177,251,145,92,24,0,187,183,101,117,73,247,129,165,92,150,176,109,133,157,249,191,10,182,146,168,123,25,151,159,10,151,206,20,15,133,182,36,236,62,224,68,121,139,74,42,222,18,23,223,149,116,150,123,242,24,78,129,136,88,244,85,38,85,150,117,159,57,241,208,196,233,178,18,92,22,26,178,50,70,203,60,13,47,194,232,42,100,33,147,94,19,176,179,189,41,211,148,34,119,160,237,123,142,246,85,207,147,39,121,203,174,94,178,199,143,22,178,48,173,188,128,87,170,65,45,73,228,74,80,187,85,104,160,232,96,151,160,58,142,182,161,216,183,2,140,94,6,205,183,153,167,249,39,1,226,131,132,167,226,19,0,249,88,212,251,75,74,81,220,47,41,121,41,191,36,240,218,125,249,148,151,234,219,178,146,82,169,120,21,60,230,118,117,100,149,73,64,229,20,10,236,18,180,130,86,133,153,227,19,192,214,33,90,137,99,241,136,222,135,85,92,246,155,118,3,211,98,68,229,158,75,92,112,125,106,125,22,33,69,191,241,98,52,118,79,198,169,33,45,236,21,145,177,146,147,95,52,41,133,184,115,156,166,8,178,55,165,144,81,173,25,231,248,68,78,199,19,60,238,151,32,207,145,49,64,208,217,74,45,202,239,83,150,51,165,1,27,185,99,110,195,127,86,79,45,184,213,81,180,204,177,106,20,182,167,175,27,38,220,96,20,229,22,182,106,12,190,55,175,27,132,108,50,136,188,115,173,28,130,38,168,107,71,64,147,13,198,208,46,173,42,141,34,246,242,181,134,217,96,32,181,192,140,55,215,253,148,22,14,176,111,6,196,79,93,236,132,210,20,105,116,209,25,37,150,44,174,30,46,127,185,73,55,159,122,105,64,81,230,172,84,56,243,87,160,157,85,72,98,247,110,40,177,171,97,209,54,255,213,130,167,27,8,158,174,18,76,223,48,175,22,189,129,250,226,201,42,209,180,24,96,181,232,96,3,209,193,42,209,236,253,253,106,217,201,6,178,147,10,217,218,119,36,50,145,22,134,79,225,112,128,153,163,177,95,142,248,192,8,158,153,139,23,222,237,195,14,222,145,165,110,118,254,173,135,244,115,4,226,47,177,50,116,121,219,82,190,154,144,66,244,15,59,164,36,63,188,83,150,254,129,80,33,142,201,51,138,185,180,139,12,122,29,140,50,79,12,97,190,104,151,213,3,37,217,173,101,217,43,96,221,33,94,22,21,240,175,221,148,151,198,197,103,23,126,186,239,167,110,130,137,120,145,194,152,156,149,241,77,249,96,209,157,56,74,156,211,226,140,30,185,32,125,161,19,136,3,228,98,171,249,223,95,247,183,155,231,117,147,133,50,254,34,155,149,98,176,11,142,206,114,228,212,66,34,165,136,52,114,177,16,175,193,87,133,220,85,29,187,204,1,85,128,46,28,149,39,200,189,56,193,144,243,109,57,38,181,138,201,191,170,82,213,35,59,21,104,165,34,22,210,223,84,22,174,11,141,231,209,234,113,235,98,212,238,130,197,247,255,1,115,164,240,143,108,62,0,0 };
