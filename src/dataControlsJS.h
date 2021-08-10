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
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3372] PROGMEM = { 31,139,8,0,116,127,224,96,2,255,237,91,235,115,219,54,18,255,158,191,130,102,58,17,121,150,245,72,226,52,149,76,103,20,89,73,116,85,108,159,45,55,157,75,115,30,72,132,44,78,40,146,37,65,63,234,250,127,191,197,2,32,65,138,146,149,184,237,221,77,238,67,18,113,177,248,237,98,31,120,110,166,97,144,48,227,108,120,62,60,28,142,135,189,209,249,219,179,161,243,180,213,234,78,85,195,201,96,116,212,59,0,90,91,209,142,15,122,227,193,249,209,155,55,167,131,177,211,214,121,199,195,241,96,228,104,132,99,232,89,234,135,164,28,108,120,222,231,148,167,69,166,190,224,122,154,115,189,62,27,143,143,14,157,103,69,62,73,109,183,158,229,156,163,222,107,80,226,121,145,81,16,219,173,231,57,223,233,135,225,184,255,110,112,226,236,22,89,51,122,187,181,171,113,143,134,7,64,123,81,226,21,212,118,235,69,206,121,120,246,254,53,208,190,47,114,74,106,187,245,189,102,174,193,207,99,48,252,241,217,216,121,89,228,214,90,218,173,151,121,143,183,39,189,227,119,206,15,146,208,59,56,16,148,243,227,163,225,33,103,149,13,253,209,160,119,34,153,219,173,31,52,137,189,215,78,187,228,16,65,211,28,114,58,24,13,250,128,86,242,137,34,183,53,175,28,29,143,135,220,254,37,183,40,114,91,115,203,251,33,16,74,94,65,90,91,243,201,251,222,207,78,187,228,15,164,181,117,87,140,7,199,78,187,236,9,36,182,53,63,188,237,157,189,29,56,237,146,31,36,181,173,185,161,215,239,243,216,200,93,48,70,78,73,214,0,178,200,62,56,250,112,152,197,245,104,240,102,156,197,239,201,240,237,187,113,22,165,253,193,225,24,156,174,6,216,63,31,159,157,252,227,236,104,120,58,200,160,250,231,131,247,131,147,222,40,207,147,254,249,241,0,122,157,12,127,130,174,79,51,226,135,193,184,119,122,252,174,55,210,224,207,79,207,14,223,140,142,62,20,100,244,123,39,39,71,227,44,170,251,231,189,209,240,159,189,19,48,245,139,140,116,208,59,249,49,139,208,254,249,225,209,225,192,121,186,187,219,189,36,177,113,17,147,104,126,64,24,113,2,122,101,244,226,152,220,88,54,182,204,73,210,155,78,169,239,204,136,159,80,36,37,190,231,210,184,31,6,204,11,210,48,77,100,211,44,13,166,204,11,3,99,26,250,97,220,247,73,146,88,248,115,232,218,183,242,135,115,152,46,38,52,206,232,221,228,202,99,211,185,198,71,18,170,219,172,19,83,150,198,129,9,127,253,154,134,94,66,205,174,100,145,38,84,12,116,65,99,226,187,89,115,110,79,197,17,81,70,227,216,187,164,113,198,148,219,87,49,93,81,70,146,104,78,124,150,49,101,246,86,60,73,26,204,252,240,74,195,17,230,87,237,83,18,199,97,222,95,185,66,53,19,223,251,141,196,94,144,49,112,79,168,70,151,196,159,205,174,75,103,36,245,153,34,154,221,187,187,71,220,242,87,116,146,132,211,207,93,237,55,184,33,160,83,70,221,178,23,98,250,107,74,19,118,20,123,52,96,132,147,142,105,188,240,146,4,126,89,246,237,221,163,140,51,33,151,244,173,10,0,104,242,195,41,241,79,89,24,147,11,218,72,40,27,50,186,176,76,154,68,169,135,113,146,152,245,191,159,30,29,54,18,6,163,184,240,102,55,86,22,62,182,221,213,128,99,144,31,198,26,182,7,30,198,8,2,137,46,134,91,65,214,69,165,172,28,188,235,205,172,172,235,150,19,164,190,111,223,230,88,168,84,68,226,132,230,92,118,87,216,48,23,249,209,115,63,129,146,130,252,241,83,89,95,18,51,48,193,119,150,27,78,211,5,88,206,110,16,215,181,204,191,153,118,35,156,205,32,41,190,179,204,199,113,120,5,223,115,182,240,45,211,180,187,210,21,141,169,31,130,108,8,106,129,162,67,67,214,157,130,23,210,100,0,177,17,131,132,21,238,227,232,25,47,200,136,233,34,188,164,34,153,76,204,146,157,139,152,210,192,180,151,88,65,207,2,95,76,221,10,46,161,52,106,97,52,141,195,208,144,26,112,37,159,60,126,249,226,217,110,183,162,87,62,244,2,53,176,110,167,190,55,253,220,145,150,171,223,21,6,61,39,129,235,211,159,188,196,155,120,190,199,110,250,64,184,0,3,221,130,31,183,202,6,120,242,100,75,217,188,49,247,92,151,6,246,109,230,16,158,0,121,188,74,39,101,236,48,242,193,37,252,24,121,9,163,1,76,47,230,101,38,115,138,50,205,122,181,46,117,180,58,6,214,149,23,184,225,85,131,7,36,151,210,136,194,152,109,57,166,249,251,239,213,45,47,91,171,90,158,63,127,150,249,23,231,211,15,116,114,10,191,41,179,204,171,164,211,108,154,219,229,142,243,48,97,1,89,208,109,179,179,220,200,81,183,205,230,21,152,188,123,71,65,223,175,6,151,24,143,84,192,134,65,24,209,192,81,150,181,232,37,227,83,117,144,132,62,133,254,23,0,41,56,13,206,183,65,204,173,138,77,70,175,65,191,204,215,121,206,228,241,207,226,148,118,239,186,185,106,152,77,155,232,134,140,0,89,206,177,2,26,229,180,53,104,156,80,133,32,214,64,30,59,24,98,247,33,112,118,183,52,25,65,67,195,197,185,136,183,82,39,139,219,73,232,222,32,109,10,95,52,134,96,83,235,33,103,111,176,155,136,202,21,177,184,89,239,84,204,65,156,196,200,36,9,200,101,21,25,20,133,212,96,122,19,196,60,138,41,47,231,48,165,150,23,248,74,62,136,35,164,115,232,56,244,147,198,44,140,7,4,148,167,62,229,195,115,246,113,174,135,149,236,90,152,238,150,179,119,74,75,135,100,182,235,119,93,205,204,86,214,11,124,96,119,39,49,37,159,187,202,18,226,116,210,41,135,121,76,253,144,184,86,153,27,207,39,157,204,230,204,99,62,21,3,242,201,132,250,104,161,5,241,130,119,148,192,240,148,129,114,134,50,30,158,42,58,124,100,224,93,64,204,236,40,62,251,194,26,246,173,248,116,164,3,204,237,10,30,153,205,26,39,58,21,12,171,48,113,30,243,169,66,107,144,8,242,16,150,163,61,215,187,52,60,215,169,121,174,185,45,220,224,185,219,143,204,26,36,3,164,163,83,99,87,33,223,135,165,139,32,49,96,63,226,26,76,196,152,1,236,218,254,76,58,16,190,109,222,123,31,90,205,189,249,238,190,2,69,11,112,90,19,136,123,243,184,41,56,146,136,4,40,222,175,150,142,221,12,252,123,231,10,150,239,90,6,120,73,252,148,34,32,199,16,104,77,24,204,62,14,187,104,105,113,208,251,175,55,53,204,45,127,162,169,39,41,99,161,48,246,132,5,37,13,56,71,24,44,32,25,41,164,66,0,44,200,141,107,178,85,224,173,27,124,126,181,11,93,210,104,109,7,177,52,86,185,78,116,218,207,28,199,205,137,202,201,238,98,103,192,194,116,58,199,181,186,147,79,154,246,45,109,68,49,229,153,125,32,54,185,60,97,53,37,36,68,29,213,237,222,213,17,5,28,241,53,24,114,113,191,19,219,146,98,112,169,19,255,55,30,94,34,83,185,252,100,69,46,139,21,137,75,179,242,48,112,28,179,109,190,50,193,51,176,243,112,97,211,98,106,82,249,136,100,103,47,168,237,239,121,65,148,50,131,175,102,78,13,123,76,194,235,154,144,89,18,25,6,125,238,64,37,148,239,228,10,49,137,59,254,218,70,186,52,85,120,226,152,113,152,197,217,38,19,161,162,37,71,92,142,22,126,55,213,81,31,252,247,183,61,255,195,38,67,9,146,171,191,236,154,74,35,251,222,254,30,49,10,83,83,68,92,145,157,103,199,117,163,122,110,210,230,165,117,220,114,98,194,241,71,179,226,248,247,159,60,254,225,197,110,171,187,215,36,16,0,160,198,189,234,224,237,205,151,104,84,221,161,160,84,252,80,165,248,253,210,151,232,84,201,95,80,201,127,168,74,252,254,235,75,84,170,228,47,168,52,217,68,165,166,8,169,124,71,236,56,50,31,95,153,160,107,30,133,51,47,94,212,86,232,46,238,228,234,155,43,47,59,172,85,127,90,82,255,232,71,174,185,154,127,246,154,144,36,197,25,135,39,56,143,215,175,92,37,245,148,248,218,85,178,10,67,95,37,81,69,255,225,42,98,60,62,88,201,2,202,146,154,241,195,213,20,169,252,96,61,139,48,75,138,78,30,174,40,38,211,131,245,44,160,44,169,57,125,184,154,50,109,30,172,104,9,103,237,62,14,95,99,254,119,215,99,254,177,35,142,214,127,204,218,172,237,190,98,126,197,37,193,101,127,177,23,171,220,239,137,13,26,246,169,25,11,15,230,207,22,252,75,174,157,90,187,5,191,196,94,171,86,62,13,212,170,132,157,159,11,24,237,180,88,201,133,32,155,28,14,245,105,20,33,78,17,193,218,90,186,188,88,10,16,241,8,247,141,111,216,132,219,19,118,227,243,61,56,239,222,153,248,100,250,185,43,214,178,32,93,84,6,67,128,175,53,107,124,15,39,46,188,71,85,172,226,171,184,107,135,245,178,185,254,160,159,191,124,254,223,77,235,220,196,111,81,75,106,109,224,25,222,235,43,253,210,123,221,89,178,88,241,162,49,51,92,97,235,200,247,82,192,163,174,21,148,76,46,114,30,211,153,83,211,92,199,233,220,64,154,254,102,190,5,172,186,193,92,114,86,25,44,27,16,239,199,253,223,40,2,192,215,132,186,125,241,109,221,250,97,24,117,248,90,117,103,227,61,173,101,146,200,19,146,9,112,207,60,31,28,108,101,107,23,127,150,192,39,165,239,44,54,247,18,208,135,49,56,168,242,129,153,182,3,167,209,199,56,238,29,22,238,136,51,166,217,189,195,21,213,68,186,89,215,87,65,30,239,160,142,19,197,225,34,98,150,57,38,19,131,133,134,60,116,195,47,75,36,150,17,198,48,84,251,149,184,183,221,226,227,105,8,38,232,97,193,167,109,223,18,159,198,28,98,78,24,199,52,220,144,38,70,16,50,131,94,123,9,51,58,191,252,130,62,174,88,120,171,214,85,124,249,255,214,239,57,169,79,167,107,18,82,180,47,93,98,168,204,19,205,95,151,123,162,156,162,179,194,232,185,91,208,156,74,143,42,219,151,45,26,70,248,136,198,213,23,63,55,157,81,20,69,8,163,156,187,194,126,2,179,106,68,239,135,155,14,135,59,123,153,109,219,52,112,126,20,41,32,135,149,120,191,81,203,206,227,6,83,17,54,47,102,241,58,103,73,151,222,207,127,149,46,228,250,30,93,120,21,203,95,164,76,194,104,116,143,54,88,64,244,141,167,253,204,187,72,99,138,10,96,241,65,233,160,175,120,166,68,198,250,18,90,177,81,16,0,177,152,241,89,93,195,71,9,254,201,89,42,152,80,7,178,46,152,7,246,185,216,112,122,121,97,45,247,173,155,82,213,172,75,193,179,165,178,49,116,48,75,156,247,132,205,27,113,152,130,209,249,91,50,32,66,208,240,58,140,177,183,128,95,77,216,249,183,170,52,109,68,105,50,183,110,175,59,44,169,223,116,242,128,130,133,164,84,77,242,53,154,107,122,107,85,109,157,10,131,125,252,244,7,203,83,53,100,223,118,6,124,24,142,70,198,235,129,209,51,208,24,198,125,27,211,11,146,94,208,255,228,9,2,235,246,248,36,170,202,213,108,209,254,39,123,49,43,142,195,226,133,255,110,159,162,137,140,38,175,249,97,134,183,136,196,187,59,117,141,89,234,251,55,91,250,189,1,225,99,162,176,35,229,21,115,194,195,72,42,105,189,175,247,153,16,191,124,197,44,188,181,7,187,205,74,224,48,101,124,209,42,247,137,202,243,228,250,66,182,82,40,104,149,191,88,37,161,223,101,230,15,251,114,241,91,238,152,189,10,174,121,29,130,29,126,203,124,213,234,180,43,1,196,117,148,188,224,224,181,91,86,97,225,6,61,52,168,186,9,19,172,169,238,183,150,209,228,221,5,71,192,35,122,54,20,232,125,207,72,180,51,53,198,54,30,29,191,160,191,220,255,47,237,46,55,235,45,159,238,117,82,254,130,150,151,123,119,150,123,138,217,151,139,149,147,202,23,72,21,179,128,160,171,18,74,85,161,131,245,63,150,121,22,124,14,194,171,0,103,39,126,166,194,131,144,169,192,242,121,153,183,239,59,133,178,247,39,79,178,150,189,98,61,142,216,167,169,178,151,178,191,187,58,166,227,20,2,197,190,149,189,138,133,126,34,124,118,178,162,87,89,114,187,35,139,92,213,103,94,209,170,40,121,249,170,162,100,197,170,138,32,170,83,213,87,86,140,106,119,149,38,121,77,151,228,49,183,171,39,31,53,49,86,14,33,215,93,41,173,105,171,171,153,233,39,21,91,167,209,74,61,238,244,82,175,5,77,18,2,43,138,86,79,164,87,37,138,81,201,117,70,44,58,194,129,16,87,226,52,5,147,134,164,99,208,229,5,158,9,206,227,137,143,17,216,49,183,225,95,172,216,147,220,186,148,194,106,86,37,5,19,122,157,152,96,3,41,218,221,78,149,12,145,245,235,132,176,77,132,168,155,156,74,17,124,209,92,43,129,76,54,144,81,56,42,87,58,69,78,66,107,29,179,129,32,189,78,68,52,215,189,132,63,23,98,85,170,252,89,132,157,112,154,134,198,131,222,40,177,164,81,181,184,236,73,131,39,127,189,36,80,22,251,105,117,126,226,225,163,179,74,147,104,122,191,42,209,180,160,75,97,150,92,13,60,219,0,120,182,10,152,191,43,173,134,222,192,124,209,100,21,52,127,2,92,13,237,111,0,237,175,130,198,87,187,213,216,241,6,216,113,5,118,161,82,89,237,34,164,227,19,216,191,80,12,52,252,229,200,18,118,248,198,16,207,163,219,131,25,180,163,42,90,236,172,154,88,197,57,1,248,75,170,137,46,79,91,90,93,174,2,41,150,14,43,36,47,184,23,171,88,130,158,195,33,158,145,143,165,45,238,134,239,83,163,204,19,197,97,148,183,171,55,195,18,118,107,25,123,133,90,247,192,171,167,68,241,255,41,180,167,164,188,176,215,75,14,188,100,26,83,38,175,103,197,122,129,215,200,250,235,149,105,215,241,115,169,65,62,126,65,187,216,46,46,183,35,157,151,32,225,119,131,242,162,89,237,122,25,155,43,168,177,161,0,197,189,51,191,198,181,212,245,179,194,84,141,184,217,85,151,60,226,210,88,31,24,42,132,207,188,120,180,235,104,130,10,235,99,241,142,27,182,53,220,240,17,28,253,168,213,252,215,47,7,219,205,139,186,105,218,234,109,22,35,108,13,180,66,11,120,57,184,212,145,83,178,219,168,186,16,251,7,169,195,255,116,255,13,215,88,250,95,238,56,0,0 };
