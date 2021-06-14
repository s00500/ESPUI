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
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3392] PROGMEM = { 31,139,8,0,36,84,199,96,2,255,237,90,109,115,219,54,18,254,158,95,65,51,29,139,60,203,122,73,226,52,149,76,103,20,89,73,212,42,182,207,150,155,206,165,57,15,36,66,22,39,20,201,130,160,101,87,213,127,191,197,27,9,82,148,172,196,105,175,55,185,15,113,196,197,98,247,193,238,98,241,182,227,48,136,169,113,217,191,234,159,244,135,253,206,224,234,205,101,223,121,210,104,180,199,170,225,188,55,56,237,28,3,173,169,104,103,199,157,97,239,234,244,245,235,139,222,208,105,234,188,195,254,176,55,112,52,194,25,244,44,244,227,164,76,88,255,170,203,40,79,242,76,93,193,245,36,227,122,117,57,28,158,158,56,79,243,124,146,218,108,60,205,56,7,157,87,0,226,89,158,81,16,155,141,103,25,223,197,251,254,176,251,182,119,238,28,228,89,83,122,179,113,160,113,15,250,199,64,123,94,224,21,212,102,227,121,198,121,114,249,238,21,208,190,207,115,74,106,179,241,189,102,174,222,47,67,48,252,217,229,208,121,145,231,214,90,154,141,23,89,143,55,231,157,179,183,206,15,146,208,57,62,22,148,171,179,211,254,9,99,149,13,221,65,175,115,46,153,155,141,31,52,141,157,87,78,179,224,16,65,211,28,114,209,27,244,186,32,173,224,19,69,110,106,94,57,61,27,246,153,253,11,110,81,228,166,230,150,119,125,32,20,188,194,105,77,205,39,239,58,191,56,205,130,63,56,173,169,187,98,216,59,115,154,69,79,112,98,83,243,195,155,206,229,155,158,211,44,248,65,82,155,154,27,58,221,46,139,141,204,5,67,206,41,201,154,128,52,178,143,79,223,159,164,113,61,232,189,30,166,241,123,222,127,243,118,152,70,105,183,119,50,4,167,171,1,118,175,134,151,231,255,188,60,237,95,244,82,81,221,171,222,187,222,121,103,144,205,147,238,213,89,15,122,157,247,127,134,174,79,82,226,251,222,176,115,113,246,182,51,208,196,95,93,92,158,188,30,156,190,207,233,232,118,206,207,79,135,105,84,119,175,58,131,254,191,58,231,96,234,231,41,233,184,115,254,83,26,161,221,171,147,211,147,158,243,228,224,160,125,131,136,113,77,80,52,61,70,20,57,1,158,27,29,66,208,157,101,243,150,41,138,59,227,49,246,157,9,242,99,204,73,177,239,185,152,116,195,128,122,65,18,38,177,108,154,36,193,152,122,97,96,140,67,63,36,93,31,197,177,197,127,246,93,123,33,127,56,39,201,108,132,73,74,111,199,115,143,142,167,26,31,138,177,110,179,22,193,52,33,129,9,127,126,75,66,47,198,102,91,178,72,19,42,6,60,195,4,249,110,218,156,217,83,113,68,152,98,66,188,27,76,82,166,204,190,138,105,142,41,138,163,41,242,105,202,148,218,91,241,196,73,48,241,195,185,38,71,152,95,181,143,17,33,97,214,95,185,66,53,35,223,251,29,17,47,72,25,152,39,84,163,139,200,39,179,237,226,9,74,124,170,136,102,123,185,124,196,44,63,199,163,56,28,127,106,107,191,193,13,1,30,83,236,22,189,64,240,111,9,142,233,41,241,112,64,17,35,157,97,50,243,226,24,126,89,246,98,249,40,229,140,209,13,126,163,2,0,154,252,112,140,252,11,26,18,116,141,107,49,166,125,138,103,150,137,227,40,241,120,156,196,102,245,199,139,211,147,90,76,97,20,215,222,228,206,74,195,199,182,219,154,96,2,250,67,162,201,246,192,195,60,130,64,163,203,195,45,167,235,186,84,87,38,188,237,77,172,180,235,142,19,36,190,111,47,50,89,28,84,132,72,140,51,46,187,45,108,152,169,252,224,185,31,1,164,32,127,248,88,196,139,8,5,19,124,103,185,225,56,153,129,229,236,26,114,93,203,252,135,105,215,194,201,4,38,197,119,150,249,152,132,115,248,158,210,153,111,153,166,221,150,174,168,141,253,16,116,67,80,11,41,186,104,152,117,23,224,133,36,238,65,108,16,208,176,198,125,76,122,202,11,58,8,158,133,55,88,76,38,147,207,146,253,107,130,113,96,218,43,172,128,51,199,71,176,91,194,37,64,115,20,70,221,56,9,13,137,128,129,220,125,252,226,249,211,131,118,73,175,108,232,57,106,96,45,198,190,55,254,212,146,150,171,46,115,131,158,162,192,245,241,207,94,236,141,60,223,163,119,93,32,92,131,129,22,224,199,157,162,1,118,119,119,148,205,107,83,207,117,113,96,47,82,135,176,9,144,197,171,116,82,202,14,35,239,221,192,143,129,23,83,28,64,122,49,111,82,157,99,174,211,172,150,99,169,114,171,243,192,154,123,129,27,206,107,44,32,153,150,90,20,18,186,227,152,230,31,127,148,183,188,104,172,107,121,246,236,105,234,95,158,79,223,227,209,5,252,198,212,50,231,113,171,94,55,247,138,29,167,97,76,3,52,195,123,102,107,181,145,73,221,51,235,115,48,121,123,137,1,239,23,11,151,50,30,169,128,13,131,48,194,129,163,44,107,225,27,202,82,117,16,135,62,134,254,215,32,82,112,26,140,111,139,152,91,23,155,20,223,2,190,212,215,217,156,201,226,159,146,4,183,151,237,12,26,159,77,219,96,227,140,32,178,56,199,114,210,48,163,109,144,198,8,101,18,196,26,200,98,135,135,216,125,18,24,187,91,72,70,208,80,115,121,46,98,173,216,73,227,118,20,186,119,156,54,134,47,76,32,216,212,122,200,216,107,244,46,194,114,69,204,111,214,91,37,57,136,145,40,26,197,1,186,41,35,3,80,152,26,84,111,130,152,231,106,138,203,57,164,212,226,2,95,202,7,113,196,233,76,52,9,253,184,54,9,73,15,1,120,236,99,54,60,231,136,231,122,88,201,110,133,233,22,140,189,85,88,58,36,179,93,93,182,53,51,91,105,47,240,129,221,30,17,140,62,181,149,37,196,233,164,85,12,115,130,253,16,185,86,145,155,159,79,90,169,205,169,71,125,44,6,228,163,17,246,185,133,102,200,11,222,98,4,195,83,6,202,24,138,242,248,169,162,197,70,6,222,5,137,169,29,197,103,87,88,195,94,136,79,71,58,192,220,43,225,145,179,89,227,228,78,5,195,10,82,13,69,48,237,96,245,57,116,189,27,195,115,157,138,231,154,123,194,234,158,187,247,200,172,64,236,195,236,115,42,116,30,178,109,87,50,11,98,3,182,31,174,65,69,72,25,192,174,109,199,164,191,224,219,102,189,143,160,213,60,156,30,28,41,161,124,192,140,86,7,226,225,148,212,5,71,28,161,128,171,247,203,181,243,110,6,255,187,63,135,213,186,146,10,188,65,126,130,185,64,38,67,72,171,195,96,142,204,162,89,197,169,238,239,102,87,200,27,127,162,93,71,9,165,161,176,236,136,6,5,4,140,35,12,102,48,209,48,132,121,0,44,156,155,175,183,86,142,183,106,176,220,105,231,186,36,209,198,14,98,217,43,243,147,232,116,148,122,137,89,143,131,147,221,197,170,79,195,100,60,229,235,112,43,75,136,246,2,215,34,130,217,172,61,22,27,88,54,25,53,16,82,68,149,195,109,47,171,92,10,56,226,75,100,200,133,123,89,93,73,15,234,44,255,109,197,146,152,131,76,127,188,102,150,138,165,133,105,179,50,159,59,142,217,52,95,154,224,6,216,66,184,176,251,48,53,173,108,68,178,179,23,84,142,14,189,32,74,168,193,150,37,167,194,123,140,194,219,138,208,89,80,25,6,93,230,45,165,148,109,201,114,1,200,183,238,149,173,176,212,85,44,242,49,243,97,230,243,72,170,66,133,70,38,177,24,26,236,138,169,165,62,216,239,111,42,141,195,214,64,41,146,107,182,236,154,72,139,250,222,209,33,50,114,73,39,66,174,152,119,151,103,85,163,60,235,104,25,103,19,183,76,57,124,252,209,36,63,254,163,221,199,63,60,63,104,180,15,235,8,188,13,48,238,133,195,239,92,62,7,81,121,135,28,40,242,80,80,236,86,232,115,48,149,242,231,32,249,15,133,196,110,173,62,7,82,41,127,14,210,104,27,72,117,17,82,217,62,214,113,228,244,123,105,2,214,44,10,39,30,153,85,214,96,23,55,105,213,237,193,203,14,27,225,143,11,240,79,127,98,200,85,178,57,172,195,36,201,167,23,54,159,89,188,126,225,250,167,79,137,47,93,255,202,100,232,235,31,135,232,63,28,34,143,199,7,131,204,73,89,129,73,30,14,83,76,229,7,227,204,139,89,1,58,122,56,80,62,153,30,140,51,39,101,5,230,248,225,48,229,180,121,48,208,130,156,13,59,52,254,130,242,63,179,248,178,143,125,113,250,253,58,11,177,182,175,34,236,22,74,10,151,253,197,46,171,116,39,39,182,94,188,79,197,152,121,144,44,27,240,63,186,117,42,205,6,252,18,187,168,74,113,83,95,41,83,118,117,37,196,104,39,188,82,46,46,100,155,3,157,158,51,185,136,11,46,193,218,89,185,95,40,68,131,120,37,251,182,182,98,194,199,49,189,243,217,86,154,117,111,141,124,52,254,212,22,171,84,144,204,74,61,31,240,215,147,13,142,134,83,18,191,215,84,172,226,43,191,249,134,149,176,190,233,36,158,189,67,254,223,39,154,79,216,21,102,1,214,22,110,96,189,190,200,9,157,87,173,252,149,94,106,165,220,118,143,237,127,128,71,29,242,149,2,38,127,74,240,196,169,104,110,97,116,102,13,13,172,153,109,219,202,238,10,87,60,83,20,150,162,103,253,152,111,107,121,1,240,53,194,110,87,124,91,11,63,12,163,22,91,95,150,54,191,17,181,76,20,121,66,51,2,238,137,231,131,55,173,116,189,97,15,0,252,241,230,59,139,78,189,24,240,80,10,39,73,54,48,211,118,224,184,248,152,143,123,159,134,251,226,16,104,182,151,124,21,52,57,221,172,234,43,23,139,101,128,227,68,36,156,69,212,50,135,104,100,208,208,144,167,98,248,101,137,41,99,132,4,134,106,191,20,55,164,59,108,60,53,193,4,61,44,248,180,237,5,242,49,97,34,166,136,50,153,134,27,226,216,8,66,106,224,91,47,166,70,235,215,95,121,124,151,44,150,171,107,33,127,97,255,198,238,19,177,143,199,27,166,154,104,95,185,82,80,115,74,52,127,201,172,18,53,10,173,53,22,206,124,192,109,167,80,148,25,186,104,207,48,226,47,83,12,188,248,185,109,166,80,20,161,12,51,238,18,235,9,153,71,220,149,249,17,189,235,111,59,28,230,234,85,182,61,211,224,121,79,68,187,28,86,236,253,142,45,91,133,156,156,117,176,221,48,115,87,43,203,21,44,157,95,254,42,44,232,246,30,44,172,52,228,47,2,19,83,28,221,131,134,87,229,124,91,115,124,226,93,39,4,115,0,252,1,191,112,236,86,60,99,36,67,123,69,90,190,81,16,64,98,126,122,167,181,1,31,164,240,143,206,74,209,129,58,30,181,193,60,176,17,229,13,23,55,215,214,106,223,170,41,161,166,93,52,55,22,10,175,184,55,105,236,188,67,116,90,35,97,2,38,103,175,177,32,15,34,132,85,50,12,189,25,252,170,195,198,188,81,134,179,22,37,241,212,90,220,182,104,92,189,107,101,209,3,11,68,161,30,227,129,184,181,186,176,86,137,185,62,124,252,202,250,84,21,214,55,21,238,239,251,131,129,241,170,103,116,12,62,118,227,190,205,228,53,74,174,241,127,111,139,207,203,220,88,122,84,213,93,182,104,255,147,93,150,214,146,241,183,254,191,149,3,185,69,140,58,171,136,129,179,247,44,18,175,210,216,53,38,137,239,223,237,232,71,118,196,134,128,97,23,201,234,201,132,59,57,169,128,250,72,239,51,66,126,241,42,87,184,230,16,118,136,165,130,195,132,178,213,167,216,39,42,102,192,205,101,94,57,191,107,85,177,252,108,161,223,24,102,143,222,37,15,39,249,26,217,214,134,7,23,216,147,55,204,151,141,86,179,84,128,184,246,145,55,11,172,174,201,202,173,191,128,67,19,85,53,33,117,154,234,22,105,85,154,188,54,96,18,248,113,57,29,10,244,190,103,36,218,9,151,7,50,63,217,125,70,127,185,103,95,217,36,110,215,91,190,116,235,164,236,89,42,43,133,110,173,246,20,121,149,169,149,249,227,51,180,138,41,47,232,170,188,80,85,175,240,218,24,203,188,12,62,5,225,60,224,137,136,157,130,248,209,37,205,29,203,71,42,37,176,246,35,39,87,18,190,187,155,182,28,230,107,85,196,118,75,149,132,20,253,221,214,101,58,78,46,80,236,133,236,149,47,130,19,225,179,159,22,132,202,114,212,125,89,0,170,62,179,106,79,69,201,74,59,21,37,45,228,84,4,81,185,169,190,210,66,77,187,173,144,100,245,78,146,199,220,43,79,61,42,11,150,14,33,195,174,64,107,104,117,152,41,62,9,108,19,162,181,56,150,122,25,212,12,199,49,130,197,67,171,181,105,139,218,82,121,77,135,96,102,59,139,101,86,71,42,232,114,149,17,75,142,240,41,132,154,56,39,65,30,145,116,30,135,176,129,201,68,125,16,45,31,119,28,7,218,118,119,21,146,152,231,252,216,231,241,218,50,247,224,127,94,251,38,5,149,137,96,2,244,234,194,220,242,87,6,140,167,133,60,178,188,250,96,157,118,77,139,118,129,83,166,67,228,142,77,74,232,54,74,212,13,78,169,10,182,206,110,212,128,70,91,232,200,157,154,75,253,40,83,217,38,85,241,22,138,244,106,13,209,92,245,98,246,180,199,235,62,229,207,188,216,17,163,105,210,216,212,49,10,44,73,84,174,46,125,126,96,41,164,90,80,40,203,233,180,74,58,241,72,209,90,135,36,26,223,15,37,26,231,176,228,114,237,122,193,147,45,4,79,214,9,102,111,64,235,69,111,97,190,104,180,78,52,123,174,91,47,218,223,66,180,191,78,52,127,97,91,47,155,108,33,155,148,200,206,213,2,171,189,136,116,124,12,123,32,204,3,141,255,114,100,145,56,124,243,16,207,162,219,131,60,220,82,165,38,118,90,175,171,226,28,129,248,27,172,169,46,102,58,173,242,85,9,89,225,137,72,24,101,237,234,97,77,175,224,85,234,188,224,94,133,249,74,240,109,117,170,71,56,174,213,200,204,210,100,53,238,95,115,68,154,236,198,170,236,7,130,231,11,148,246,168,148,85,225,122,241,177,23,143,9,166,242,134,87,172,27,252,38,90,127,199,50,237,42,255,92,105,144,207,96,208,46,246,175,171,237,156,110,170,21,169,134,89,133,171,118,67,205,155,75,168,196,80,2,197,213,53,187,9,182,212,13,182,146,169,26,249,238,91,93,30,137,123,103,125,96,28,16,127,221,229,199,202,150,166,40,183,58,231,175,201,97,159,197,12,31,193,177,19,91,245,127,255,122,188,87,191,174,154,166,45,159,100,69,28,110,16,173,164,5,172,118,91,98,100,148,244,150,171,42,212,126,37,56,236,95,251,63,218,10,44,215,155,56,0,0 };
