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
function sliderchange(number){var val=$("#sl"+number).val();console.log("sliderchange "+number+": "+val);websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function tabclick(number){var val=$("#tab"+number).val();websock.send("tabvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var range=$(".range-slider__range");var value=$(".range-slider__value");value.each(function(){$(this).html($(this).prev().attr("value"))});range.on({input:function(){$(this).next().html(this.value)}});range.each(function(){if($(this).attr("callbackSet")!="true"){if(!isDiscrete){$(this).on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});}else{$(this).on({change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""))}});}
$(this).attr("callbackSet","true");}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3406] PROGMEM = { 31,139,8,0,92,62,203,97,2,255,237,91,109,115,219,54,18,254,158,95,65,51,157,136,60,203,122,73,226,52,149,76,103,20,89,73,116,85,108,159,45,55,157,75,115,30,72,132,44,78,40,146,37,65,191,212,245,127,191,197,2,32,65,138,146,149,184,237,221,77,238,75,44,46,22,207,46,118,23,192,2,216,76,195,32,97,198,217,240,124,120,56,28,15,123,163,243,183,103,67,231,105,171,213,157,170,134,147,193,232,168,119,0,180,182,162,29,31,244,198,131,243,163,55,111,78,7,99,167,173,243,142,135,227,193,200,209,8,199,208,179,212,15,73,57,216,240,188,207,41,79,139,76,125,193,245,52,231,122,125,54,30,31,29,58,207,138,124,146,218,110,61,203,57,71,189,215,160,196,243,34,163,32,182,91,207,115,190,211,15,195,113,255,221,224,196,217,45,178,102,244,118,107,87,227,30,13,15,128,246,162,196,43,168,237,214,139,156,243,240,236,253,107,160,125,95,228,148,212,118,235,123,205,92,131,159,199,96,248,227,179,177,243,178,200,173,181,180,91,47,243,30,111,79,122,199,239,156,31,36,161,119,112,32,40,231,199,71,195,67,206,42,27,250,163,65,239,68,50,183,91,63,104,18,123,175,157,118,201,33,130,166,57,228,116,48,26,244,1,173,228,19,69,110,107,94,57,58,30,15,185,253,75,110,81,228,182,230,150,247,67,32,148,188,130,180,182,230,147,247,189,159,157,118,201,31,72,107,235,174,24,15,142,157,118,217,19,72,108,107,126,120,219,59,123,59,112,218,37,63,72,106,91,115,67,175,223,231,177,145,187,96,140,156,146,172,1,100,145,125,112,244,225,48,139,235,209,224,205,56,139,223,147,225,219,119,227,44,74,251,131,195,49,56,93,13,176,127,62,62,59,249,199,217,209,240,116,144,65,245,207,7,239,7,39,189,81,62,79,250,231,199,3,232,117,50,252,9,186,62,205,136,31,6,227,222,233,241,187,222,72,131,63,63,61,59,124,51,58,250,80,144,209,239,157,156,28,141,179,168,238,159,247,70,195,127,246,78,192,212,47,50,210,65,239,228,199,44,66,251,231,135,71,135,3,231,233,238,110,247,146,196,198,69,76,162,249,1,97,196,9,232,149,209,139,99,114,99,217,216,50,39,73,111,58,165,190,51,35,126,66,145,148,248,158,75,227,126,24,48,47,72,195,52,145,77,179,52,152,50,47,12,140,105,232,135,113,223,39,73,98,225,207,161,107,223,202,31,206,97,186,152,208,56,163,119,147,43,143,77,231,26,31,73,168,110,179,78,76,89,26,7,38,252,243,107,26,122,9,53,187,146,69,154,80,49,208,5,141,137,239,102,205,185,61,21,71,68,25,141,99,239,146,198,25,83,110,95,197,116,69,25,73,162,57,241,89,198,148,217,91,241,36,105,48,243,195,43,13,71,152,95,181,79,73,28,135,121,127,229,10,213,76,124,239,55,18,123,65,198,192,61,161,26,93,18,127,54,187,46,157,145,212,103,138,104,118,239,238,30,113,203,95,209,73,18,78,63,119,181,223,224,134,128,78,25,117,203,94,136,233,175,41,77,216,81,236,209,128,17,78,58,166,241,194,75,18,248,101,217,183,119,143,50,206,132,92,210,183,42,0,160,201,15,167,196,63,101,97,76,46,104,35,161,108,200,232,194,50,105,18,165,30,198,73,98,214,255,126,122,116,216,72,24,140,226,194,155,221,88,89,248,216,118,87,3,142,65,126,24,107,216,30,120,24,35,8,36,186,24,110,5,89,23,149,178,114,240,174,55,179,178,174,91,78,144,250,190,125,155,99,161,82,17,137,19,154,115,217,93,97,195,92,228,71,207,253,4,74,10,242,199,79,101,125,73,204,192,4,223,89,110,56,77,23,96,57,187,65,92,215,50,255,102,218,141,112,54,131,73,241,157,101,62,142,195,43,248,158,179,133,111,153,166,221,149,174,104,76,253,16,100,67,80,11,20,29,26,102,221,41,120,33,77,6,16,27,49,72,88,225,62,142,158,241,130,140,152,46,194,75,42,38,147,137,179,100,231,34,166,52,48,237,37,86,208,179,192,23,83,183,130,75,40,141,90,24,77,227,48,52,164,6,92,201,39,143,95,190,120,182,219,173,232,149,15,189,64,13,172,219,169,239,77,63,119,164,229,234,119,133,65,207,73,224,250,244,39,47,241,38,158,239,177,155,62,16,46,192,64,183,224,199,173,178,1,158,60,217,82,54,111,204,61,215,165,129,125,155,57,132,79,128,60,94,165,147,50,118,24,249,224,18,126,140,188,132,209,0,150,23,243,50,147,57,69,153,102,189,90,151,58,90,29,3,235,202,11,220,240,170,193,3,146,75,105,68,97,204,182,28,211,252,253,247,234,150,151,173,85,45,207,159,63,203,252,139,235,233,7,58,57,133,223,148,89,230,85,210,105,54,205,237,114,199,121,152,176,128,44,232,182,217,89,110,228,168,219,102,243,10,76,222,189,163,160,239,87,131,75,140,71,42,96,195,32,140,104,224,40,203,90,244,146,241,165,58,72,66,159,66,255,11,128,20,156,6,231,219,32,230,86,197,38,163,215,160,95,230,235,124,206,228,241,207,226,148,118,239,186,185,106,56,155,54,209,13,25,1,178,60,199,10,104,148,211,214,160,113,66,21,130,216,3,121,236,96,136,221,135,192,217,221,210,98,4,13,13,23,215,34,222,74,157,44,110,39,161,123,131,180,41,124,209,24,130,77,237,135,156,189,193,110,34,42,119,196,98,178,222,169,88,131,56,137,145,73,18,144,203,42,50,40,10,83,131,233,77,16,243,40,166,188,157,195,146,90,222,224,43,249,32,142,144,206,161,227,208,79,26,179,48,30,16,80,158,250,148,15,207,217,199,181,30,118,178,107,97,186,91,206,222,41,109,29,146,217,174,223,117,53,51,91,89,47,240,129,221,157,196,148,124,238,42,75,136,211,73,167,28,230,49,245,67,226,90,101,110,60,159,116,50,155,51,143,249,84,12,200,39,19,234,163,133,22,196,11,222,81,2,195,83,6,202,25,202,120,120,170,232,240,145,129,119,1,49,179,163,248,236,11,107,216,183,226,211,145,14,48,183,43,120,228,108,214,56,209,169,96,88,133,137,235,152,79,21,90,131,68,48,15,97,59,218,115,189,75,195,115,157,154,231,154,219,194,13,158,187,253,200,172,193,100,128,233,232,212,216,85,200,243,176,116,17,36,6,228,35,174,193,68,140,25,192,174,229,103,210,129,240,109,243,222,251,208,106,238,205,119,247,21,40,90,128,211,154,64,220,155,199,77,193,145,68,36,64,241,126,181,116,236,102,224,191,59,87,176,125,215,50,192,75,226,167,20,1,57,134,64,107,194,96,246,113,216,69,75,139,131,222,127,189,169,97,109,249,19,77,61,73,25,11,133,177,39,44,40,105,192,57,194,96,1,147,145,194,84,8,128,5,185,113,79,182,10,188,117,131,175,175,118,161,75,26,173,237,32,182,198,42,215,137,78,251,153,227,184,57,81,57,217,93,100,6,44,76,167,115,220,171,59,249,162,105,223,210,70,20,83,62,179,15,68,146,203,39,172,166,132,132,168,163,186,221,187,58,162,128,35,190,6,67,110,238,119,34,45,41,6,151,58,241,127,227,225,37,102,42,151,159,172,152,203,98,71,226,210,172,60,12,28,199,108,155,175,76,240,12,100,30,46,36,45,166,38,149,143,72,118,246,130,218,254,158,23,68,41,51,248,110,230,212,176,199,36,188,174,9,153,37,145,97,208,231,14,84,66,121,38,87,136,73,204,248,107,27,233,210,84,225,137,99,198,97,22,87,155,76,132,138,150,28,113,57,90,248,221,84,71,125,240,223,223,246,250,15,73,134,18,36,119,127,217,53,149,70,246,189,253,61,98,20,150,166,136,184,98,118,158,29,215,141,234,181,73,91,151,214,113,203,133,9,199,31,205,138,227,223,127,242,248,135,23,187,173,238,94,147,64,0,128,26,247,170,131,183,55,95,162,81,117,135,130,82,241,67,149,226,247,75,95,162,83,37,127,65,37,255,161,42,241,251,175,47,81,169,146,191,160,210,100,19,149,154,34,164,242,140,216,113,228,124,124,101,130,174,121,20,206,188,120,81,91,161,187,184,147,171,111,174,188,236,176,86,253,105,73,253,163,31,185,230,106,253,217,107,194,36,41,174,56,124,130,243,120,253,202,93,82,159,18,95,187,75,86,97,232,187,36,170,232,63,92,69,140,199,7,43,89,64,89,82,51,126,184,154,98,42,63,88,207,34,204,146,162,147,135,43,138,147,233,193,122,22,80,150,212,156,62,92,77,57,109,30,172,104,9,103,109,30,135,175,49,255,187,251,49,255,216,17,71,235,63,102,111,214,178,175,152,95,113,73,112,217,95,228,98,149,249,158,72,208,176,79,205,88,120,176,126,182,224,47,185,118,106,237,22,252,18,185,86,173,124,26,168,85,9,59,63,23,48,218,105,177,146,11,65,54,57,28,234,203,40,66,156,34,130,181,181,116,121,177,20,32,226,17,238,27,79,216,132,219,19,118,227,243,28,156,119,239,76,124,50,253,220,21,123,89,144,46,42,131,33,192,215,154,53,190,135,19,23,222,163,42,86,241,85,204,218,97,191,108,174,63,232,231,47,159,255,119,211,58,55,241,91,212,146,90,27,120,134,247,250,74,191,244,94,119,150,44,86,188,104,204,12,87,72,29,121,46,5,60,234,90,65,201,228,34,231,49,157,57,53,205,117,156,206,13,164,233,111,230,41,96,213,13,230,146,179,202,96,217,128,120,63,238,255,70,17,0,190,38,212,237,139,111,235,214,15,195,168,195,247,170,59,27,239,105,45,147,68,158,144,76,128,123,230,249,224,96,43,219,187,248,179,4,62,41,125,103,177,185,151,128,62,140,193,65,149,15,204,180,29,56,141,62,198,113,239,176,112,71,156,49,205,238,29,238,168,38,210,205,186,190,11,242,120,7,117,156,40,14,23,17,179,204,49,153,24,44,52,228,161,27,126,89,98,98,25,97,12,67,181,95,137,123,219,45,62,158,134,96,130,30,22,124,218,246,45,241,105,204,33,230,132,113,76,195,13,105,98,4,33,51,232,181,151,48,163,243,203,47,232,227,138,141,183,106,95,197,151,255,111,253,158,147,250,116,186,102,66,138,246,165,75,12,53,243,68,243,215,205,61,81,78,209,89,97,244,220,45,104,78,165,71,149,237,203,22,13,35,124,68,227,234,139,159,155,174,40,138,34,132,81,206,93,97,63,129,89,53,162,247,195,77,135,195,157,189,204,182,109,26,184,62,138,41,32,135,149,120,191,81,203,206,227,6,167,34,36,47,102,241,58,103,73,151,222,207,127,149,46,228,250,30,93,120,21,203,95,164,76,194,104,116,143,54,88,64,244,141,79,251,153,119,145,198,20,21,192,226,131,210,65,95,241,76,137,140,245,37,180,98,163,32,0,98,113,198,103,117,13,31,37,248,39,103,169,96,66,29,200,186,96,30,200,115,177,225,244,242,194,90,238,91,55,165,170,89,151,130,103,75,101,99,232,96,150,56,239,9,155,55,226,48,5,163,243,183,100,64,132,160,225,117,24,99,111,1,191,154,144,249,183,170,52,109,68,105,50,183,110,175,59,44,169,223,116,242,128,130,141,164,84,77,242,53,154,107,122,107,85,109,157,10,131,125,252,244,7,203,83,53,100,223,246,12,248,48,28,141,140,215,3,163,103,160,49,140,251,18,211,11,146,94,208,255,228,9,2,235,246,248,34,170,202,213,108,209,254,39,123,49,43,142,195,226,133,255,110,159,162,137,140,38,175,249,97,134,183,136,196,187,59,117,141,89,234,251,55,91,250,189,1,225,99,162,144,145,242,138,57,225,97,36,149,180,222,215,251,76,136,95,190,98,22,222,218,131,108,179,18,56,76,25,223,180,202,125,162,242,58,185,190,144,173,20,10,90,229,47,86,73,232,119,153,249,195,190,220,252,150,59,102,175,130,107,94,135,32,195,111,153,175,90,157,118,37,128,184,142,146,23,28,188,118,203,42,108,220,160,135,6,85,55,97,129,53,213,253,214,50,154,188,187,224,8,120,68,207,134,2,189,239,25,137,118,166,198,216,198,163,227,23,244,151,249,255,82,118,185,89,111,249,116,175,147,242,23,180,188,220,187,179,220,83,172,190,92,172,92,84,190,64,170,88,5,4,93,149,80,170,10,29,172,255,177,204,179,224,115,16,94,5,184,58,241,51,21,30,132,76,5,150,175,203,188,125,223,41,148,189,63,121,146,181,236,21,235,113,68,158,166,202,94,202,254,238,234,152,142,83,8,20,251,86,246,42,22,250,137,240,217,201,138,94,101,201,237,142,44,114,85,159,121,69,171,162,228,229,171,138,146,21,171,42,130,168,78,85,95,89,49,170,221,85,154,228,53,93,146,199,220,174,94,124,212,194,88,57,132,92,119,165,180,166,173,174,102,166,159,84,108,157,70,43,245,184,211,75,189,22,52,73,8,236,40,90,61,145,94,149,40,70,37,247,25,177,233,8,7,66,92,137,211,20,44,26,146,142,65,135,117,97,121,217,153,222,223,80,156,219,102,7,126,3,119,94,13,154,224,162,159,248,24,174,29,108,196,242,62,9,173,171,84,216,250,170,84,194,217,95,212,169,40,38,216,64,138,118,17,84,37,67,44,17,235,132,176,77,132,168,107,159,74,17,124,135,93,43,129,76,54,144,81,56,87,87,122,80,174,88,235,68,37,27,8,210,139,74,68,115,221,75,248,219,34,150,176,202,159,69,216,9,167,105,104,124,134,24,37,150,52,170,22,151,189,127,240,149,162,94,18,40,43,3,181,162,64,241,74,210,89,165,73,52,189,95,149,104,90,208,165,176,164,174,6,158,109,0,60,91,5,204,31,161,86,67,111,96,190,104,178,10,154,191,23,174,134,246,55,128,246,87,65,227,19,223,106,236,120,3,236,184,2,187,80,214,172,82,14,233,248,4,146,29,138,129,134,191,28,89,239,14,223,24,226,121,116,123,176,220,118,84,249,139,157,149,30,171,56,39,0,127,73,53,209,229,53,78,43,226,85,32,197,58,99,133,228,5,247,98,21,235,213,115,56,196,51,242,177,180,197,69,242,125,106,148,121,162,56,140,242,118,245,192,88,194,110,45,99,175,80,235,30,120,245,238,40,254,243,133,246,238,148,87,1,123,201,129,151,76,99,202,228,93,46,50,225,149,115,197,123,152,41,42,128,69,22,185,204,131,116,228,129,191,13,202,75,105,181,75,103,117,219,140,89,172,250,224,55,185,150,186,129,150,253,237,59,249,70,134,175,184,120,114,235,84,224,4,188,36,91,194,113,138,76,171,238,178,222,101,5,48,240,244,27,239,41,164,252,19,56,4,158,82,200,160,182,28,147,123,195,20,197,253,186,89,84,167,74,109,10,155,113,17,30,114,40,238,184,8,206,153,212,106,254,235,151,131,237,230,69,221,52,113,187,87,1,170,99,11,144,135,130,35,246,163,213,3,173,203,97,10,37,186,255,6,228,241,220,224,76,57,0,0 };
