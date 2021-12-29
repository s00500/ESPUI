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

const uint8_t JS_CONTROLS_GZIP[3405] PROGMEM = { 31,139,8,0,23,149,204,97,2,255,237,91,235,115,219,54,18,255,158,191,130,102,50,17,121,150,245,72,226,52,149,76,103,20,89,73,212,42,182,207,150,155,206,165,57,15,68,66,22,39,20,201,146,160,31,117,245,191,223,226,69,2,20,37,43,118,219,235,77,238,75,34,46,22,191,93,236,46,176,120,172,221,40,76,137,113,54,60,31,30,14,199,195,222,232,252,221,217,208,121,214,106,117,93,217,112,50,24,29,245,14,128,214,150,180,227,131,222,120,112,126,244,246,237,233,96,236,180,85,222,241,112,60,24,57,10,225,24,122,150,250,49,82,1,54,60,239,83,202,51,157,169,207,185,158,21,92,111,206,198,227,163,67,231,185,206,39,168,237,214,243,130,115,212,123,3,74,188,208,25,57,177,221,122,81,240,157,126,28,142,251,239,7,39,206,174,206,154,211,219,173,93,133,123,52,60,0,218,203,18,47,167,182,91,47,11,206,195,179,15,111,128,246,157,206,41,168,237,214,119,138,185,6,63,143,193,240,199,103,99,231,149,206,173,180,180,91,175,138,30,239,78,122,199,239,157,239,5,161,119,112,192,41,231,199,71,195,67,202,42,26,250,163,65,239,68,48,183,91,223,43,18,123,111,156,118,201,33,156,166,56,228,116,48,26,244,1,173,228,19,73,110,43,94,57,58,30,15,169,253,75,110,145,228,182,226,150,15,67,32,148,188,194,104,109,197,39,31,122,63,59,237,146,63,24,173,173,186,98,60,56,118,218,101,79,48,98,91,241,195,187,222,217,187,129,211,46,249,65,80,219,138,27,122,253,62,141,141,194,5,99,198,41,200,10,64,30,217,7,71,31,15,243,184,30,13,222,142,243,248,61,25,190,123,63,206,163,180,63,56,28,131,211,229,0,251,231,227,179,147,127,158,29,13,79,7,57,84,255,124,240,97,112,210,27,21,243,164,127,126,60,128,94,39,195,159,160,235,179,156,248,113,48,238,157,30,191,239,141,20,248,243,211,179,195,183,163,163,143,154,140,126,239,228,228,104,156,71,117,255,188,55,26,254,171,119,2,166,126,153,147,14,122,39,63,230,17,218,63,63,60,58,28,56,207,118,119,187,151,40,49,46,18,20,207,14,16,65,78,136,175,140,94,146,160,27,203,102,45,51,148,246,92,23,7,206,20,5,41,102,164,52,240,61,156,244,163,144,248,97,22,101,169,104,154,102,161,75,252,40,52,220,40,136,146,126,128,210,212,98,63,135,158,125,43,126,56,135,217,124,130,147,156,222,77,175,124,226,206,20,62,148,98,213,102,157,4,147,44,9,77,248,231,215,44,242,83,108,118,5,139,48,161,100,192,115,156,160,192,203,155,11,123,74,142,24,19,156,36,254,37,78,114,166,194,190,146,233,10,19,148,198,51,20,144,156,41,183,183,228,73,179,112,26,68,87,10,14,55,191,108,119,81,146,68,69,127,233,10,217,140,2,255,55,148,248,97,206,64,61,33,27,61,148,124,49,187,30,158,162,44,32,146,104,118,23,139,71,212,242,87,120,146,70,238,151,174,242,27,220,16,98,151,96,175,236,133,4,255,154,225,148,28,37,62,14,9,162,164,99,156,204,253,52,133,95,150,125,187,120,148,115,166,232,18,191,147,1,0,77,65,228,162,224,148,68,9,186,192,141,20,147,33,193,115,203,196,105,156,249,44,78,82,179,254,195,233,209,97,35,37,48,138,11,127,122,99,229,225,99,219,93,5,56,1,249,81,162,96,251,224,97,22,65,32,209,99,225,166,201,186,168,148,85,128,119,253,169,149,119,221,114,194,44,8,236,219,2,139,41,21,163,36,197,5,151,221,229,54,44,68,126,242,189,207,160,36,39,127,250,92,214,23,37,4,76,240,196,242,34,55,155,131,229,236,6,242,60,203,252,135,105,55,162,233,20,38,197,19,203,124,156,68,87,240,61,35,243,192,50,77,187,43,92,209,112,131,8,100,67,80,115,20,21,26,102,221,41,120,33,75,7,16,27,9,72,88,225,62,138,158,243,130,140,4,207,163,75,204,39,147,201,102,201,206,69,130,113,104,218,75,172,160,167,198,151,96,175,130,139,43,205,180,48,154,198,97,100,8,13,168,146,79,31,191,122,249,124,183,91,209,171,24,186,70,13,173,91,55,240,221,47,29,97,185,250,66,27,244,12,133,94,128,127,242,83,127,226,7,62,185,233,3,225,2,12,116,11,126,220,42,27,224,233,211,45,105,243,198,204,247,60,28,218,183,185,67,232,4,40,226,85,56,41,103,135,145,15,46,225,199,200,79,9,14,97,121,49,47,115,153,46,147,105,214,171,117,169,51,171,179,192,186,242,67,47,186,106,208,128,164,82,26,113,148,144,45,199,52,127,255,189,186,229,85,107,85,203,139,23,207,115,255,178,245,244,35,158,156,194,111,76,44,243,42,237,52,155,230,118,185,227,44,74,73,136,230,120,219,236,44,55,82,212,109,179,121,5,38,239,46,48,232,123,111,112,129,241,72,6,108,20,70,49,14,29,105,89,11,95,18,186,84,135,105,20,96,232,127,1,144,156,211,160,124,27,196,220,170,216,36,248,26,244,203,125,93,204,153,34,254,73,146,225,238,162,91,168,198,102,211,38,186,49,70,128,44,207,49,13,13,83,218,26,52,74,168,66,224,57,144,198,14,11,177,187,16,40,187,87,90,140,160,161,225,177,181,136,182,98,39,143,219,73,228,221,48,154,11,95,56,129,96,147,249,144,178,55,200,77,140,69,70,212,55,235,157,138,53,136,146,8,154,164,33,186,172,34,131,162,48,53,136,218,4,49,207,196,148,211,57,44,169,229,4,95,201,7,113,196,232,20,58,137,130,180,49,141,146,1,2,229,113,128,233,240,156,125,182,214,67,38,187,230,166,187,165,236,157,82,234,16,204,118,125,209,85,204,108,229,189,192,7,118,119,146,96,244,165,43,45,193,79,39,157,114,152,39,56,136,144,103,149,185,217,249,164,147,219,156,248,36,192,124,64,1,154,224,128,89,104,142,252,240,61,70,48,60,105,160,130,161,140,199,78,21,29,58,50,240,46,32,230,118,228,159,125,110,13,251,150,127,58,194,1,230,118,5,143,152,205,10,39,115,42,24,86,98,178,117,44,192,18,173,129,98,152,135,144,142,246,60,255,210,240,61,167,230,123,230,54,119,131,239,109,63,50,107,48,25,96,58,58,53,114,21,209,125,88,54,15,83,3,246,35,158,65,120,140,25,192,174,236,207,132,3,225,219,166,189,247,161,213,220,155,237,238,75,80,102,1,74,107,2,113,111,150,52,57,71,26,163,144,137,15,170,165,179,110,6,251,119,231,10,210,119,45,7,188,68,65,134,25,32,197,224,104,77,24,204,62,27,182,110,105,126,208,251,219,155,26,214,150,63,209,212,147,140,144,136,27,123,66,194,146,6,148,35,10,231,48,25,49,76,133,16,88,24,55,203,201,150,198,91,55,232,250,106,107,93,178,120,109,7,158,26,171,92,199,59,237,231,142,163,230,100,202,137,238,124,103,64,162,204,157,177,92,221,41,22,77,251,22,55,226,4,211,153,125,192,55,185,116,194,42,74,8,136,58,83,183,187,168,51,20,112,196,125,48,68,114,95,240,109,137,30,92,242,196,255,141,135,23,159,169,84,126,186,98,46,243,140,68,165,89,69,24,56,142,217,54,95,155,224,25,216,121,120,176,105,49,21,169,116,68,162,179,31,214,246,247,252,48,206,136,65,179,153,83,99,61,38,209,117,141,203,44,137,140,194,62,117,160,20,74,119,114,90,76,178,29,127,109,35,93,154,50,60,217,152,217,48,245,213,38,23,33,163,165,64,92,142,22,122,55,213,145,31,244,247,183,189,254,195,38,67,10,18,217,95,116,205,132,145,3,127,127,15,25,218,210,20,35,143,207,206,179,227,186,81,189,54,41,235,210,58,110,177,48,177,241,199,83,125,252,251,79,31,127,255,114,183,213,221,107,34,8,0,80,227,78,117,216,237,205,215,104,84,221,65,83,42,121,168,82,244,126,233,107,116,170,228,215,84,10,30,170,18,189,255,250,26,149,42,249,53,149,38,155,168,212,228,33,85,236,136,29,71,204,199,215,38,232,90,68,225,212,79,230,181,21,186,243,59,185,250,230,202,139,14,107,213,119,75,234,31,253,72,53,151,235,207,94,19,38,137,190,226,208,9,78,227,245,158,89,82,157,18,247,205,146,85,24,106,150,100,42,6,15,87,145,197,227,131,149,212,80,150,212,76,30,174,38,159,202,15,214,83,135,89,82,116,242,112,69,217,100,122,176,158,26,202,146,154,238,195,213,20,211,230,193,138,150,112,214,238,227,216,107,204,255,110,62,166,31,59,252,104,253,199,228,102,101,247,149,208,43,46,1,46,250,243,189,88,229,126,143,111,208,88,159,154,49,247,97,253,108,193,255,232,218,169,181,91,240,139,239,181,106,229,211,64,173,74,216,249,57,135,81,78,139,149,92,12,100,147,195,161,186,140,50,136,83,134,96,109,45,93,94,44,5,8,127,132,251,198,55,108,220,237,41,185,9,232,30,156,118,239,76,2,228,126,233,242,92,22,102,243,202,96,8,217,107,205,26,223,195,137,139,221,163,74,86,254,165,239,218,33,95,54,215,31,244,139,151,207,255,187,105,157,155,232,45,106,73,173,13,60,67,123,221,211,47,189,55,157,37,139,233,23,141,185,225,180,173,35,221,75,1,143,188,86,144,50,169,200,89,130,167,78,77,113,29,165,83,3,41,250,155,197,22,176,234,6,115,201,89,101,176,124,64,180,31,245,127,67,7,128,175,9,246,250,252,219,186,13,162,40,238,208,92,181,176,217,61,173,101,162,216,231,146,17,112,79,253,0,28,108,229,185,139,62,75,176,39,165,39,22,153,249,41,232,67,8,28,84,233,192,76,219,129,211,232,99,54,238,29,18,237,240,51,166,217,93,176,140,106,50,186,89,87,179,32,141,119,80,199,137,147,104,30,19,203,28,163,137,65,34,67,28,186,225,151,197,39,150,17,37,48,84,251,53,191,183,221,162,227,105,112,38,232,97,193,167,109,223,162,0,39,20,98,134,8,197,52,188,8,167,70,24,17,3,95,251,41,49,58,191,252,194,124,92,145,120,171,242,42,123,249,255,214,239,57,113,128,221,53,19,146,183,47,93,98,200,153,199,155,239,55,247,120,57,69,103,133,209,11,183,48,115,74,61,170,108,95,182,104,20,179,71,52,170,62,255,185,233,138,34,41,92,24,166,220,21,246,227,152,85,35,250,48,220,116,56,212,217,203,108,219,166,193,214,71,62,5,196,176,82,255,55,108,217,69,220,176,169,8,155,23,83,191,206,89,210,165,247,243,95,165,11,186,190,67,23,90,197,242,23,41,147,18,28,223,161,13,43,32,250,198,167,253,212,191,200,18,204,20,96,197,7,165,131,190,228,113,145,136,245,37,52,189,145,19,0,81,159,241,121,93,195,39,1,254,217,89,42,152,144,7,178,46,152,7,246,185,172,225,244,242,194,90,238,91,55,133,170,121,23,205,179,165,178,49,230,96,146,58,31,16,153,53,146,40,3,163,211,183,100,64,132,160,161,117,24,99,127,14,191,154,176,243,111,85,105,218,136,179,116,102,221,94,119,72,90,191,233,20,1,5,137,164,84,77,114,31,205,21,189,149,170,182,78,133,193,62,125,254,131,229,201,26,178,111,123,6,124,28,142,70,198,155,129,209,51,152,49,140,187,54,166,23,40,187,192,255,205,19,4,171,219,163,139,168,44,87,179,121,251,159,236,197,188,56,142,21,47,252,189,125,202,76,100,52,105,205,15,49,252,121,204,223,221,177,103,76,179,32,184,217,82,239,13,16,29,19,134,29,41,173,152,227,30,102,164,146,214,251,106,159,9,10,202,87,204,220,91,123,176,219,172,4,142,50,66,147,86,185,79,92,94,39,215,23,178,149,66,65,169,252,101,85,18,234,93,102,241,176,47,146,223,114,199,252,85,112,205,235,16,236,240,91,230,235,86,167,93,9,192,175,163,196,5,7,173,221,178,180,196,13,122,40,80,117,19,22,88,83,222,111,45,163,137,187,11,138,192,142,232,249,80,160,247,29,35,81,206,212,44,182,217,209,241,43,250,139,253,255,210,238,114,179,222,226,233,94,37,21,47,104,69,185,119,103,185,39,95,125,169,88,177,168,124,133,84,190,10,112,186,44,161,148,21,58,172,254,199,50,207,194,47,97,116,21,178,213,137,158,169,216,65,200,148,96,197,186,76,219,247,29,173,236,253,233,211,188,101,79,175,199,225,251,52,89,246,82,246,119,87,197,116,28,45,80,236,91,209,75,47,244,227,225,179,147,23,189,138,146,219,29,81,228,42,63,139,138,86,73,41,202,87,37,37,47,86,149,4,94,157,42,191,242,98,84,187,43,53,41,106,186,4,143,185,93,189,248,200,133,177,114,8,133,238,82,105,69,91,85,205,92,63,161,216,58,141,86,234,177,120,68,79,249,21,87,144,112,126,199,180,38,73,57,189,139,99,251,167,214,103,49,159,245,115,60,163,177,211,63,167,134,180,116,77,44,30,149,156,252,248,172,148,154,205,113,154,34,200,104,74,61,147,90,21,201,245,19,121,142,39,61,30,64,128,199,79,115,176,104,9,58,11,250,162,192,52,101,121,36,13,152,228,142,185,13,255,179,138,65,193,173,74,209,178,105,149,20,182,160,172,19,19,110,32,69,185,91,170,146,193,87,157,117,66,200,38,66,228,77,82,165,8,154,180,215,74,64,147,13,100,104,71,245,74,167,136,69,112,173,99,54,16,164,214,169,240,230,186,159,210,231,74,86,21,43,126,234,176,19,74,83,208,232,164,51,74,44,89,92,45,46,127,82,161,139,79,189,36,80,20,27,42,117,134,252,225,165,179,74,147,216,189,91,149,216,213,116,209,86,233,213,192,211,13,128,167,171,128,233,187,214,106,232,13,204,23,79,86,65,211,39,200,213,208,193,6,208,193,42,104,246,106,184,26,59,217,0,59,169,192,214,42,165,229,46,70,56,62,133,253,19,102,129,198,126,57,162,132,30,190,89,136,23,209,237,195,10,222,145,21,53,118,94,205,44,227,28,1,252,37,86,68,151,151,45,165,46,88,130,232,165,203,18,201,15,239,196,210,75,224,11,56,134,103,20,99,105,243,187,233,187,212,40,243,196,73,20,23,237,242,205,178,132,221,90,198,94,161,214,29,240,242,41,147,255,61,135,242,148,85,20,22,251,233,129,159,186,9,38,226,122,152,49,57,43,243,155,242,39,57,238,196,81,242,156,150,103,244,204,5,219,19,58,128,24,142,112,216,106,254,251,151,131,237,230,69,221,100,169,140,63,175,177,7,96,118,232,235,44,103,78,45,37,82,138,216,145,45,22,226,113,110,85,202,93,213,177,203,2,80,85,208,133,211,196,4,206,151,167,24,54,103,91,142,73,189,98,242,191,27,80,205,35,59,21,218,74,67,44,100,188,169,44,220,22,26,207,163,213,114,235,66,106,119,193,242,251,127,0,85,102,181,200,54,57,0,0 };
