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
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");}
break;case UI_BUTTON:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</button></div>");$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");rangeSlider(!sliderContinuous);}
break;case UI_NUMBER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");}
break;case UI_TEXT_INPUT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
break;case UI_SELECT:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_GAUGE:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
if(data.visible){parent.append("<div id='id"+
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
"</div>");}
break;case UI_ACCEL:if(hasAccel)break;var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
hasAccel=true;if(data.visible){parent.append("<div id='id"+
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
"</div>");requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);break;case UPDATE_SLIDER:slider_move($("#id"+data.id),data.value,"100",false);break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}
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

const uint8_t JS_CONTROLS_GZIP[3420] PROGMEM = { 31,139,8,0,27,207,208,97,2,255,237,27,107,115,219,56,238,123,127,133,162,118,106,233,226,248,209,54,221,174,29,165,227,58,110,235,91,55,201,37,206,118,231,186,189,12,45,209,177,166,178,164,149,168,60,54,235,255,126,224,75,34,101,217,113,147,125,221,244,190,180,22,8,2,32,0,2,32,137,184,81,152,18,227,108,120,62,60,28,142,135,189,209,249,187,179,161,243,172,213,234,186,114,224,100,48,58,234,29,0,172,45,97,199,7,189,241,224,252,232,237,219,211,193,216,105,171,184,131,159,198,131,195,3,78,163,173,192,199,195,241,96,228,40,128,99,160,88,162,199,64,5,147,225,121,159,66,158,233,72,125,142,245,172,192,122,115,54,30,31,29,58,207,117,60,1,109,183,158,23,152,163,222,27,16,226,133,142,200,129,237,214,139,2,239,244,227,112,220,127,63,56,113,118,117,212,28,222,110,237,42,216,163,225,1,192,94,150,112,57,180,221,122,89,96,30,158,125,120,3,176,239,116,76,1,109,183,190,83,212,5,122,4,131,28,159,141,157,87,58,182,50,210,110,189,42,102,188,59,233,29,191,119,190,23,128,222,193,1,135,156,31,31,13,15,41,170,24,232,143,6,189,19,129,220,110,125,175,112,236,189,113,218,37,131,112,152,98,144,211,193,104,208,7,106,37,155,72,112,91,177,202,209,241,120,72,245,95,50,139,4,183,21,179,124,24,2,160,100,21,6,107,43,54,249,208,251,201,105,151,236,193,96,109,213,20,227,193,177,211,46,91,130,1,219,138,29,222,245,206,222,13,156,118,201,14,2,218,86,204,208,235,247,169,111,20,38,24,51,76,1,86,8,228,158,125,112,244,241,48,247,235,209,224,237,56,247,223,147,225,187,247,227,220,75,251,131,195,49,24,93,46,176,127,62,62,59,249,215,217,209,240,116,144,147,234,159,15,62,12,78,122,163,98,159,244,207,143,7,48,235,100,248,35,76,125,150,3,63,14,198,189,211,227,247,189,145,66,254,252,244,236,240,237,232,232,163,198,163,223,59,57,57,26,231,94,221,63,239,141,134,255,238,157,128,170,95,230,160,131,222,201,15,185,135,246,207,15,143,14,7,206,179,221,221,238,37,74,140,139,4,197,179,3,68,144,19,226,43,163,151,36,232,198,178,217,200,12,165,61,215,197,129,51,69,65,138,25,40,13,124,15,39,253,40,36,126,152,69,89,42,134,166,89,232,18,63,10,13,55,10,162,164,31,160,52,181,216,207,161,103,223,138,31,206,97,54,159,224,36,135,119,211,43,159,184,51,5,15,165,88,213,89,39,193,36,75,66,19,254,249,37,139,252,20,155,93,129,34,84,40,17,240,28,39,40,240,242,225,66,159,18,35,198,4,39,137,127,137,147,28,169,208,175,68,186,194,4,165,241,12,5,36,71,202,245,45,113,210,44,156,6,209,149,66,135,171,95,142,187,40,73,162,98,190,52,133,28,70,129,255,43,74,252,48,71,160,150,144,131,30,74,190,152,93,15,79,81,22,16,9,52,187,139,197,35,170,249,43,60,73,35,247,75,87,249,13,102,8,177,75,176,87,182,66,130,127,201,112,74,142,18,31,135,4,81,208,49,78,230,126,154,194,47,203,190,93,60,202,49,83,116,137,223,73,7,128,161,32,114,81,112,74,162,4,93,224,70,138,201,144,224,185,101,226,52,206,124,230,39,169,89,255,231,233,209,97,35,37,176,138,11,127,122,99,229,238,99,219,93,133,112,2,252,163,68,161,237,131,133,153,7,1,71,143,185,155,198,235,162,146,87,65,188,235,79,173,124,234,150,19,102,65,96,223,22,180,152,80,49,74,82,92,96,217,93,174,195,130,229,39,223,251,12,66,114,240,167,207,101,121,81,66,64,5,79,44,47,114,179,57,104,206,110,32,207,179,204,127,152,118,35,154,78,97,83,60,177,204,199,73,116,5,223,51,50,15,44,211,180,187,194,20,13,55,136,128,55,56,53,167,162,146,134,93,119,10,86,200,210,1,248,70,2,28,86,152,143,82,207,113,129,71,130,231,209,37,230,155,201,100,187,100,231,34,193,56,52,237,37,84,144,83,195,75,176,87,129,197,133,102,82,24,77,227,48,50,132,4,84,200,167,143,95,189,124,190,219,173,152,85,44,93,131,134,214,173,27,248,238,151,142,208,92,125,161,45,122,134,66,47,192,63,250,169,63,241,3,159,220,244,1,112,1,10,186,5,59,110,149,21,240,244,233,150,212,121,99,230,123,30,14,237,219,220,32,116,3,20,254,42,140,148,163,195,202,7,151,240,99,228,167,4,135,16,94,204,203,156,167,203,120,154,245,106,89,234,76,235,204,177,174,252,208,139,174,26,212,33,41,151,70,28,37,100,203,49,205,223,126,171,30,121,213,90,53,242,226,197,243,220,190,44,158,126,196,147,83,248,141,137,101,94,165,157,102,211,220,46,79,156,69,41,9,209,28,111,155,157,229,65,74,117,219,108,94,129,202,187,11,12,242,222,155,184,160,241,72,58,108,20,70,49,14,29,169,89,11,95,18,26,170,195,52,10,48,204,191,0,146,28,211,160,120,27,248,220,42,223,36,248,26,228,203,109,93,236,153,194,255,73,146,225,238,162,91,136,198,118,211,38,178,49,68,32,89,222,99,26,53,76,97,107,168,81,64,21,5,158,3,169,239,48,23,187,139,2,69,247,74,193,8,6,26,30,139,69,116,20,59,185,223,78,34,239,134,193,92,248,194,9,56,155,204,135,20,189,65,110,98,44,50,162,94,196,119,42,98,16,5,17,52,73,67,116,89,5,6,65,97,107,16,117,8,124,158,177,41,167,115,8,169,229,4,95,137,7,126,196,224,148,116,18,5,105,99,26,37,3,4,194,227,0,211,229,57,251,44,214,67,38,187,230,170,187,165,232,157,82,234,16,200,118,125,209,85,212,108,229,179,192,6,118,119,146,96,244,165,43,53,81,156,68,58,127,141,0,252,216,212,41,239,179,4,7,17,242,172,50,54,59,32,117,114,163,19,159,4,152,107,52,64,19,28,48,19,205,145,31,190,199,8,244,43,45,84,32,148,233,177,99,77,135,174,12,220,11,40,230,134,228,159,125,174,13,251,150,127,58,194,3,204,237,10,28,17,78,20,76,230,85,96,89,73,147,5,210,0,75,106,13,20,67,32,128,124,184,231,249,151,134,239,57,53,223,51,183,185,31,248,222,246,35,179,6,187,17,226,129,83,35,87,17,45,4,179,121,152,26,80,16,121,6,225,78,110,0,186,82,32,10,3,194,183,77,103,239,195,168,185,55,219,221,151,68,153,6,40,172,9,192,189,89,210,228,24,105,140,66,198,62,168,230,206,166,25,236,223,157,43,168,31,106,57,193,75,20,100,152,17,164,52,56,181,38,44,102,159,45,91,215,52,63,105,254,237,85,13,193,237,15,84,245,36,35,36,226,202,158,144,176,36,1,197,136,194,57,68,3,12,91,33,4,20,134,205,138,2,75,195,173,27,52,192,219,218,148,44,94,59,129,231,230,42,211,241,73,251,185,225,168,58,153,112,98,58,47,77,72,148,185,51,86,44,116,138,168,109,223,226,70,156,96,186,179,15,120,149,77,55,172,34,132,32,81,103,226,118,23,117,70,5,12,113,31,26,162,186,88,240,186,72,119,46,121,229,240,141,187,23,223,169,148,127,186,98,47,243,148,72,185,89,133,27,56,142,217,54,95,155,96,25,40,125,60,168,154,76,133,43,93,145,152,236,135,181,253,61,63,140,51,98,208,116,234,212,216,140,73,116,93,227,60,75,44,163,176,79,13,40,153,210,82,82,243,73,118,228,168,109,36,75,83,186,39,91,51,91,166,30,109,114,22,210,91,10,138,203,222,66,47,199,58,242,131,254,254,182,227,63,84,57,146,145,200,254,98,106,38,148,28,248,251,123,200,208,66,83,140,60,190,59,207,142,235,70,117,108,82,226,210,58,108,17,152,216,250,227,169,190,254,253,167,143,191,127,185,219,234,238,53,17,56,0,136,113,167,56,236,250,232,107,36,170,158,160,9,149,60,84,40,122,193,245,53,50,85,226,107,34,5,15,21,137,94,192,125,141,72,149,248,154,72,147,77,68,106,114,151,42,74,114,199,17,251,241,181,9,178,22,94,56,245,147,121,109,133,236,252,82,176,190,185,240,98,194,90,241,221,146,248,71,63,80,201,101,252,217,107,194,38,209,35,14,221,224,212,95,239,153,37,213,45,113,223,44,89,69,67,205,146,76,196,224,225,34,50,127,124,176,144,26,149,37,49,147,135,139,201,183,242,131,229,212,201,44,9,58,121,184,160,108,51,61,88,78,141,202,146,152,238,195,197,20,219,230,193,130,150,232,172,173,227,216,115,208,255,110,62,166,31,59,252,108,255,251,228,102,165,250,74,232,29,155,32,46,230,243,90,172,178,222,227,5,26,155,83,51,230,62,196,207,22,252,143,174,157,90,187,5,191,120,173,85,43,159,6,106,85,204,206,207,57,25,229,180,88,137,197,136,108,114,56,84,195,40,35,113,202,40,88,91,75,183,39,75,14,194,95,1,191,241,130,141,155,61,37,55,1,173,193,233,244,206,36,64,238,151,46,207,101,97,54,175,116,134,144,61,23,173,177,61,156,184,216,69,174,68,229,95,122,213,14,249,178,185,254,160,95,60,189,254,223,76,235,204,68,175,113,75,98,109,96,25,58,235,158,118,233,189,233,44,105,76,191,233,204,21,167,149,142,180,150,2,28,121,173,32,121,82,150,179,4,79,157,154,98,58,10,167,10,82,228,55,139,18,176,234,10,117,201,88,101,98,249,130,232,60,106,255,134,78,0,190,38,216,235,243,111,235,54,136,162,184,67,115,213,194,102,23,197,150,137,98,159,115,70,128,61,245,3,48,176,149,231,46,250,46,194,222,180,158,88,100,230,167,32,15,33,112,80,165,11,51,109,7,78,163,143,217,186,119,72,180,195,207,152,102,119,193,50,170,201,224,102,93,205,130,212,223,65,28,39,78,162,121,76,44,115,140,38,6,137,12,113,232,134,95,22,223,88,70,148,192,82,237,215,252,226,120,139,174,167,193,145,96,134,5,159,182,125,139,2,156,80,18,51,68,40,77,195,139,112,106,132,17,49,240,181,159,18,163,243,243,207,204,198,21,137,183,42,175,178,214,131,111,253,158,19,7,216,93,179,33,249,248,210,37,134,220,121,124,248,126,123,143,247,115,116,86,40,189,48,11,83,167,148,163,74,247,101,141,70,49,123,197,163,226,243,159,155,70,20,9,225,204,48,197,174,208,31,167,89,181,162,15,195,77,151,67,141,189,140,182,109,26,44,62,242,45,32,150,149,250,191,98,203,46,252,134,109,69,40,94,76,253,58,103,73,150,222,79,127,150,44,232,250,14,89,104,27,205,159,36,76,74,112,124,135,52,172,131,233,27,223,246,83,255,34,75,48,19,128,117,63,148,14,250,18,199,69,194,215,151,168,233,131,28,0,20,245,29,159,55,86,124,18,196,63,59,75,29,27,242,64,214,5,245,64,157,203,6,78,47,47,172,229,185,117,83,136,154,79,209,44,91,234,91,99,6,38,169,243,1,145,89,35,137,50,80,58,125,204,6,138,224,52,180,17,100,236,207,225,87,19,42,255,86,149,164,141,56,75,103,214,237,117,135,164,245,155,78,225,80,144,72,74,237,44,247,145,92,145,91,105,171,235,84,40,236,211,231,223,153,159,108,98,251,182,119,192,199,225,104,100,188,25,24,61,131,41,195,184,171,48,189,64,217,5,254,43,79,16,172,113,144,6,81,217,47,103,243,241,63,216,138,121,119,30,235,158,248,123,219,148,169,200,104,210,166,35,98,248,243,152,191,187,99,207,152,102,65,112,179,165,222,27,32,186,38,12,21,41,109,217,227,22,102,160,146,212,251,234,156,9,10,202,87,204,220,90,123,80,109,86,18,142,50,66,147,86,121,78,92,142,147,235,59,233,74,174,160,180,30,179,54,13,245,46,179,120,216,23,201,111,121,98,254,42,184,230,117,8,42,252,150,249,186,213,105,87,18,224,215,81,226,130,131,54,143,89,90,226,6,57,20,82,117,19,2,172,41,239,183,150,169,137,187,11,74,129,29,209,243,165,192,236,59,86,162,156,169,153,111,179,163,227,87,204,23,245,255,82,117,185,217,108,241,116,175,130,138,23,180,162,223,188,179,60,147,71,95,202,86,4,149,175,224,202,163,0,135,203,30,78,217,34,196,26,144,44,243,44,252,18,70,87,33,139,78,244,76,197,14,66,166,36,86,196,101,58,190,239,104,253,248,79,159,230,35,123,122,67,16,175,211,100,219,75,217,222,93,149,166,227,104,142,98,223,138,89,122,167,33,119,159,157,188,235,86,244,252,238,136,46,91,249,89,180,212,74,72,209,63,43,33,121,183,172,4,240,246,88,249,149,119,195,218,93,41,73,209,84,38,112,204,237,234,224,35,3,99,229,18,10,217,165,208,138,180,170,152,185,124,66,176,117,18,173,148,99,241,136,158,242,43,174,32,225,252,142,105,79,146,114,122,23,199,246,79,173,207,98,63,235,231,120,6,99,167,127,14,13,105,239,156,8,30,149,152,252,248,172,244,186,205,113,154,34,200,104,74,63,147,218,150,201,229,19,121,142,39,61,238,64,64,143,159,230,32,104,9,56,115,250,162,195,53,101,121,36,13,24,231,142,185,13,255,179,150,69,129,173,114,209,178,105,21,23,22,80,214,177,9,55,224,162,220,45,85,241,224,81,103,29,19,178,9,19,121,147,84,201,130,38,237,181,28,208,100,3,30,218,81,189,210,40,34,8,174,53,204,6,140,212,62,21,62,92,247,83,250,92,201,218,114,197,79,157,236,132,194,20,106,116,211,25,37,148,44,174,102,151,63,169,208,224,83,47,49,20,221,142,74,163,35,127,120,233,172,146,36,118,239,22,37,118,53,89,180,40,189,154,240,116,3,194,211,85,132,233,187,214,106,210,27,168,47,158,172,34,77,159,32,87,147,14,54,32,29,172,34,205,94,13,87,211,78,54,160,157,84,208,214,90,181,101,21,35,12,159,66,253,132,153,163,177,95,142,232,225,135,111,230,226,133,119,251,16,193,59,178,163,198,206,219,169,165,159,35,32,127,137,21,214,229,176,165,52,38,75,34,122,239,180,164,228,135,119,210,210,123,240,11,114,140,158,81,172,165,205,239,166,239,18,163,140,19,39,81,92,140,203,55,203,18,237,214,50,237,21,98,221,65,94,62,101,242,63,40,81,158,178,138,206,102,63,61,240,83,55,193,68,92,15,51,36,103,101,126,83,254,38,200,157,56,74,158,211,242,140,158,185,160,60,161,11,136,225,8,135,173,230,127,126,62,216,110,94,212,77,150,202,248,243,26,123,0,102,135,190,206,114,230,212,82,34,133,136,138,108,177,16,143,115,171,82,238,170,137,93,230,128,170,128,46,156,38,38,112,190,60,197,80,156,109,57,38,181,138,201,255,112,65,85,143,156,84,72,43,21,177,144,254,166,162,112,93,104,56,143,86,243,173,11,174,221,5,203,239,255,5,247,243,3,32,207,57,0,0 };
