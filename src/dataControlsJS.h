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
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}};websock.onmessage=handleEvent;}
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

const uint8_t JS_CONTROLS_GZIP[3375] PROGMEM = { 31,139,8,0,13,56,204,97,2,255,237,91,109,115,219,54,18,254,158,95,65,51,153,136,60,203,122,73,226,52,21,77,103,20,89,73,212,42,182,207,150,155,206,165,57,15,68,66,22,39,20,201,146,160,101,87,213,127,191,197,27,9,82,148,172,216,109,175,55,185,47,177,184,88,60,187,216,93,0,11,96,227,132,65,66,180,139,193,229,224,120,48,26,116,135,151,239,46,6,246,179,86,203,114,100,195,89,127,120,210,61,2,90,91,210,78,143,186,163,254,229,201,219,183,231,253,145,221,86,121,71,131,81,127,104,43,132,83,232,89,234,199,72,57,216,224,178,71,41,207,138,76,61,206,245,44,231,122,115,49,26,157,28,219,207,139,124,130,218,110,61,207,57,135,221,55,160,196,139,34,35,39,182,91,47,114,190,243,143,131,81,239,125,255,204,222,47,178,102,244,118,107,95,225,30,14,142,128,246,178,196,203,169,237,214,203,156,243,248,226,195,27,160,125,87,228,20,212,118,235,59,197,92,253,159,71,96,248,211,139,145,253,170,200,173,180,180,91,175,242,30,239,206,186,167,239,237,239,5,161,123,116,196,41,151,167,39,131,99,202,42,26,122,195,126,247,76,48,183,91,223,43,18,187,111,236,118,201,33,156,166,56,228,188,63,236,247,0,173,228,19,73,110,43,94,57,57,29,13,168,253,75,110,145,228,182,226,150,15,3,32,148,188,194,104,109,197,39,31,186,63,219,237,146,63,24,173,173,186,98,212,63,181,219,101,79,48,98,91,241,195,187,238,197,187,190,221,46,249,65,80,219,138,27,186,189,30,141,141,220,5,35,198,41,200,10,64,22,217,71,39,31,143,179,184,30,246,223,142,178,248,61,27,188,123,63,202,162,180,215,63,30,129,211,229,0,123,151,163,139,179,127,94,156,12,206,251,25,84,239,178,255,161,127,214,29,230,243,164,119,121,218,135,94,103,131,159,160,235,179,140,248,177,63,234,158,159,190,239,14,21,248,203,243,139,227,183,195,147,143,5,25,189,238,217,217,201,40,139,234,222,101,119,56,248,87,247,12,76,253,50,35,29,117,207,126,204,34,180,119,121,124,114,220,183,159,237,239,91,215,40,214,174,98,20,77,143,16,65,118,128,231,90,55,142,209,173,97,178,150,41,74,186,142,131,125,123,130,252,4,51,82,226,123,46,142,123,97,64,188,32,13,211,68,52,77,210,192,33,94,24,104,78,232,135,113,207,71,73,98,176,159,3,215,92,136,31,246,113,58,27,227,56,163,91,201,220,35,206,84,225,67,9,86,109,214,137,49,73,227,64,135,127,126,77,67,47,193,186,37,88,132,9,37,3,158,225,24,249,110,214,156,219,83,114,68,152,224,56,246,174,113,156,49,229,246,149,76,115,76,80,18,77,145,79,50,166,204,222,146,39,73,131,137,31,206,21,28,110,126,217,238,160,56,14,243,254,210,21,178,25,249,222,111,40,246,130,140,129,122,66,54,186,40,254,162,91,46,158,160,212,39,146,168,91,203,229,35,106,249,57,30,39,161,243,197,82,126,131,27,2,236,16,236,150,189,16,227,95,83,156,144,147,216,195,1,65,148,116,138,227,153,151,36,240,203,48,23,203,71,25,103,130,174,241,59,25,0,208,228,135,14,242,207,73,24,163,43,220,72,48,25,16,60,51,116,156,68,169,199,226,36,209,235,63,156,159,28,55,18,2,163,184,242,38,183,70,22,62,166,105,41,192,49,200,15,99,5,219,3,15,179,8,2,137,46,11,183,130,172,171,74,89,57,184,229,77,140,172,235,142,29,164,190,111,46,114,44,166,84,132,226,4,231,92,166,197,109,152,139,252,228,185,159,65,73,78,254,244,185,172,47,138,9,152,224,137,225,134,78,58,3,203,153,13,228,186,134,254,15,221,108,132,147,9,76,138,39,134,254,56,14,231,240,61,37,51,223,208,117,211,18,174,104,56,126,8,178,33,168,57,138,10,13,179,238,28,188,144,38,125,136,141,24,36,172,113,31,69,207,120,65,70,140,103,225,53,230,147,73,103,179,100,239,42,198,56,208,205,21,86,208,179,192,23,99,183,130,139,43,205,180,208,154,218,113,168,9,13,168,146,79,31,191,122,249,124,223,170,232,149,15,189,64,13,140,133,227,123,206,151,142,176,92,125,89,24,244,20,5,174,143,127,242,18,111,236,249,30,185,237,1,225,10,12,180,0,63,238,148,13,240,244,233,142,180,121,99,234,185,46,14,204,69,230,16,58,1,242,120,21,78,202,216,97,228,253,107,248,49,244,18,130,3,88,94,244,235,76,166,195,100,234,245,106,93,234,204,234,44,176,230,94,224,134,243,6,13,72,42,165,17,133,49,217,177,117,253,247,223,171,91,94,181,214,181,188,120,241,60,243,47,91,79,63,226,241,57,252,198,196,208,231,73,167,217,212,119,203,29,167,97,66,2,52,195,187,122,103,181,145,162,238,234,205,57,152,220,90,98,208,247,222,224,2,227,145,12,216,48,8,35,28,216,210,178,6,190,38,116,169,14,146,208,199,208,255,10,32,57,167,70,249,182,136,185,117,177,73,240,13,232,151,249,58,159,51,121,252,147,56,197,214,210,202,85,99,179,105,27,221,24,35,64,150,231,88,1,13,83,218,6,52,74,168,66,224,123,32,141,29,22,98,119,33,80,118,183,180,24,65,67,195,101,107,17,109,197,118,22,183,227,208,189,101,52,7,190,112,12,193,38,247,67,202,222,32,183,17,22,59,98,49,89,239,84,172,65,148,68,208,56,9,208,117,21,25,20,133,169,65,212,38,136,121,38,166,188,157,195,146,90,222,224,43,249,32,142,24,157,66,199,161,159,52,38,97,220,71,160,60,246,49,29,158,125,200,214,122,216,201,110,184,233,22,148,189,83,218,58,4,179,89,95,90,138,153,141,172,23,248,192,180,198,49,70,95,44,105,9,126,58,233,148,195,60,198,126,136,92,163,204,205,206,39,157,204,230,196,35,62,230,3,242,209,24,251,204,66,51,228,5,239,49,130,225,73,3,229,12,101,60,118,170,232,208,145,129,119,1,49,179,35,255,236,113,107,152,11,254,105,11,7,232,187,21,60,98,54,43,156,204,169,96,88,137,201,214,49,31,75,180,6,138,96,30,194,118,116,224,122,215,154,231,218,53,207,213,119,185,27,60,119,247,145,94,131,201,0,211,209,174,145,121,72,243,176,116,22,36,26,228,35,174,70,120,140,105,192,174,228,103,194,129,240,109,210,222,135,208,170,31,76,247,15,37,40,179,0,165,53,129,120,48,141,155,156,35,137,80,192,196,251,213,210,89,55,141,253,187,55,135,237,187,150,1,94,35,63,197,12,144,98,112,180,38,12,230,144,13,187,104,105,126,208,251,219,155,26,214,150,63,209,212,227,148,144,144,27,123,76,130,146,6,148,35,12,102,48,25,49,76,133,0,88,24,55,219,147,141,2,111,93,163,235,171,89,232,146,70,27,59,240,173,177,202,117,188,211,97,230,56,106,78,166,156,232,206,51,3,18,166,206,148,237,213,157,124,209,52,23,184,17,197,152,206,236,35,158,228,210,9,171,40,33,32,234,76,93,107,89,103,40,224,136,251,96,136,205,125,201,211,146,98,112,201,19,255,55,30,94,124,166,82,249,201,154,185,204,119,36,42,205,200,195,192,182,245,182,254,90,7,207,64,230,225,66,210,162,43,82,233,136,68,103,47,168,29,30,120,65,148,18,141,238,102,118,141,245,24,135,55,53,46,179,36,50,12,122,212,129,82,40,205,228,10,49,201,50,254,218,86,186,52,101,120,178,49,179,97,22,87,155,76,132,140,150,28,113,53,90,232,221,84,71,126,208,223,223,246,250,15,73,134,20,36,118,127,209,53,21,70,246,189,195,3,164,21,150,166,8,185,124,118,94,156,214,181,234,181,73,89,151,54,113,139,133,137,141,63,154,20,199,127,248,244,241,247,47,247,91,214,65,19,65,0,128,26,119,170,195,110,111,190,70,163,234,14,5,165,226,135,42,69,239,151,190,70,167,74,254,130,74,254,67,85,162,247,95,95,163,82,37,127,65,165,241,54,42,53,121,72,229,25,177,109,139,249,248,90,7,93,243,40,156,120,241,172,182,70,119,126,39,87,223,94,121,209,97,163,250,78,73,253,147,31,169,230,114,253,57,104,194,36,41,174,56,116,130,211,120,189,231,46,169,78,137,251,238,146,85,24,234,46,201,84,244,31,174,34,139,199,7,43,89,64,89,81,51,126,184,154,124,42,63,88,207,34,204,138,162,227,135,43,202,38,211,131,245,44,160,172,168,233,60,92,77,49,109,30,172,104,9,103,99,30,199,94,99,254,119,247,99,250,177,199,143,214,127,204,222,172,100,95,49,189,226,18,224,162,63,207,197,42,243,61,158,160,177,62,53,109,230,193,250,217,130,191,232,198,174,181,91,240,139,231,90,181,242,105,160,86,37,236,242,146,195,40,167,197,74,46,6,178,205,225,80,93,70,25,196,57,67,48,118,86,46,47,86,2,132,63,194,125,227,9,27,119,123,66,110,125,154,131,211,238,157,177,143,156,47,22,223,203,130,116,86,25,12,1,123,173,217,224,123,56,113,177,123,84,201,202,191,138,89,59,236,151,205,205,7,253,252,229,243,255,110,218,228,38,122,139,90,82,107,11,207,208,94,247,244,75,247,77,103,197,98,197,139,198,204,112,133,212,145,230,82,192,35,175,21,164,76,42,114,26,227,137,93,83,92,71,233,212,64,138,254,122,158,2,86,221,96,174,56,171,12,150,13,136,246,163,254,111,20,1,224,107,140,221,30,255,54,22,126,24,70,29,186,87,45,77,118,79,107,232,40,242,184,100,4,220,19,207,7,7,27,217,222,69,159,37,216,147,210,19,131,76,189,4,244,33,4,14,170,116,96,186,105,195,105,244,49,27,247,30,9,247,248,25,83,183,150,108,71,213,25,93,175,171,187,32,141,119,80,199,142,226,112,22,17,67,31,161,177,70,66,77,28,186,225,151,193,39,150,22,198,48,84,243,53,191,183,221,161,227,105,112,38,232,97,192,167,105,46,144,143,99,10,49,69,132,98,106,110,136,19,45,8,137,134,111,188,132,104,157,95,126,97,62,174,216,120,171,246,85,246,242,255,173,223,115,98,31,59,27,38,36,111,95,185,196,144,51,143,55,223,111,238,241,114,138,206,26,163,231,110,97,230,148,122,84,217,190,108,209,48,98,143,104,84,125,254,115,219,21,69,82,184,48,76,185,43,236,199,49,171,70,244,97,176,237,112,168,179,87,217,118,117,141,173,143,124,10,136,97,37,222,111,216,48,243,184,97,83,17,146,23,189,120,157,179,162,75,247,231,191,74,23,116,115,135,46,180,138,229,47,82,38,33,56,186,67,27,86,64,244,141,79,251,137,119,149,198,152,41,192,138,15,74,7,125,201,227,32,17,235,43,104,197,70,78,0,196,226,140,207,234,26,62,9,240,207,246,74,193,132,60,144,89,96,30,200,115,89,195,249,245,149,177,218,183,174,11,85,179,46,5,207,150,202,198,152,131,73,98,127,64,100,218,136,195,20,140,78,223,146,1,17,130,134,214,97,140,188,25,252,106,66,230,223,170,210,180,17,165,201,212,88,220,116,72,82,191,237,228,1,5,27,73,169,154,228,62,154,43,122,43,85,109,157,10,131,125,250,252,7,203,147,53,100,223,246,12,248,56,24,14,181,55,125,173,171,49,99,104,119,37,166,87,40,189,194,255,205,19,4,171,219,163,139,168,44,87,51,121,251,159,236,197,172,56,142,21,47,252,189,125,202,76,164,53,105,205,15,209,188,89,196,223,221,177,171,77,82,223,191,221,81,239,13,16,29,19,134,140,148,86,204,113,15,51,82,73,235,67,181,207,24,249,229,43,102,238,173,3,200,54,43,129,195,148,208,77,171,220,39,42,175,147,155,11,217,74,161,160,84,254,178,42,9,245,46,51,127,216,23,155,223,106,199,236,85,112,195,235,16,100,248,45,253,117,171,211,174,4,224,215,81,226,130,131,214,110,25,133,141,27,244,80,160,234,58,44,176,186,188,223,90,69,19,119,23,20,129,29,209,179,161,64,239,59,70,162,156,169,89,108,179,163,227,87,244,23,249,255,74,118,185,93,111,241,116,175,146,242,23,180,188,220,187,179,218,147,175,190,84,172,88,84,190,66,42,95,5,56,93,150,80,202,10,29,86,255,99,232,23,193,151,32,156,7,108,117,162,103,42,118,16,210,37,88,190,46,211,246,67,187,80,246,254,244,105,214,114,80,172,199,225,121,154,44,123,41,251,219,82,49,109,187,16,40,230,66,244,42,22,250,241,240,217,203,138,94,69,201,237,158,40,114,149,159,121,69,171,164,228,229,171,146,146,21,171,74,2,175,78,149,95,89,49,170,105,73,77,242,154,46,193,163,239,86,47,62,114,97,172,28,66,174,187,84,90,209,86,85,51,211,79,40,182,73,163,181,122,44,213,82,175,25,78,18,4,59,138,82,79,164,86,37,242,81,137,125,134,111,58,220,129,16,87,252,52,5,139,134,160,179,160,203,11,60,19,182,142,39,62,139,192,142,190,11,127,89,197,158,224,86,165,20,118,179,42,41,108,66,111,18,19,108,33,69,185,219,169,146,193,103,253,38,33,100,27,33,242,38,167,82,4,221,52,55,74,64,227,45,100,20,142,202,149,78,17,139,208,70,199,108,33,72,173,19,225,205,117,47,161,207,133,172,42,85,252,44,194,142,41,77,65,163,65,175,149,88,210,168,90,92,246,164,65,39,127,189,36,80,20,251,41,117,126,252,225,163,179,78,147,200,185,91,149,200,41,232,82,88,37,215,3,79,182,0,158,172,3,166,239,74,235,161,183,48,95,52,94,7,77,159,0,215,67,251,91,64,251,235,160,217,171,221,122,236,120,11,236,184,2,187,80,169,44,179,8,225,248,4,242,23,204,2,141,253,178,69,9,59,124,179,16,207,163,219,131,21,180,35,43,90,204,172,154,88,198,57,2,248,107,172,136,46,47,91,74,93,174,4,41,150,14,75,36,47,184,19,171,88,130,158,195,49,60,45,31,75,155,223,13,223,165,70,153,39,138,195,40,111,151,111,134,37,236,214,42,246,26,181,238,128,151,79,137,252,255,83,40,79,73,121,97,175,151,28,121,137,19,99,34,174,103,25,19,187,69,174,120,226,210,77,229,191,196,56,99,91,185,37,46,236,51,197,187,98,72,15,232,0,34,56,66,97,163,249,239,95,142,118,155,87,117,93,167,59,25,127,222,98,15,176,236,208,213,81,16,37,72,64,171,169,69,62,75,41,34,35,90,46,197,227,88,3,211,50,220,237,59,90,44,0,85,5,29,200,230,199,112,190,59,199,144,28,237,216,58,245,138,206,235,246,85,243,200,78,185,182,210,16,75,25,111,42,11,183,69,129,231,209,122,185,117,33,213,162,227,90,90,255,1,200,233,10,84,182,56,0,0 };
