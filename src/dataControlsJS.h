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
"'>▲</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+
data.id+
", true)' onmouseup='padclick(RIGHT, "+
data.id+
", false)' id='pr"+
data.id+
"'>▲</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+
data.id+
", true)' onmouseup='padclick(LEFT, "+
data.id+
", false)' id='pl"+
data.id+
"'>▲</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+
data.id+
", true)' onmouseup='padclick(DOWN, "+
data.id+
", false)' id='pb"+
data.id+
"'>▲</a></li>"+
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
"</div>");break;case UI_TAB:$("#tabsnav").append("<li><a href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
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
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));},});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3340] PROGMEM = { 31,139,8,0,150,187,70,95,2,255,237,90,235,118,219,198,17,254,239,167,128,224,28,19,168,40,94,124,139,67,10,242,161,41,218,102,67,139,170,68,197,57,117,92,157,37,177,20,113,12,2,200,98,33,74,97,248,26,125,144,62,82,159,164,179,55,96,1,130,20,109,37,105,122,220,31,150,137,217,217,111,191,157,153,189,207,36,12,98,106,92,244,47,251,39,253,81,191,51,184,124,115,209,119,30,55,26,237,137,42,56,235,13,134,157,99,144,53,149,236,244,184,51,234,93,14,95,191,62,239,141,156,166,174,59,234,143,122,3,71,19,156,66,205,66,61,46,202,192,250,151,93,38,121,156,87,234,10,173,199,153,214,171,139,209,104,120,226,60,201,235,73,105,179,241,36,211,28,116,94,1,137,167,121,69,33,108,54,158,102,122,231,239,251,163,238,219,222,153,243,44,175,154,202,155,141,103,154,246,160,127,12,178,231,5,93,33,109,54,158,103,154,39,23,239,94,129,236,219,188,166,148,54,27,223,106,230,234,253,56,2,195,159,94,140,156,23,121,109,173,164,217,120,145,213,120,115,214,57,125,235,124,39,5,157,227,99,33,185,60,29,246,79,152,170,44,232,14,122,157,51,169,220,108,124,167,181,216,121,229,52,11,14,17,50,205,33,231,189,65,175,11,104,5,159,40,113,83,243,202,240,116,212,103,246,47,184,69,137,155,154,91,222,245,65,80,240,10,151,53,53,159,188,235,252,232,52,11,254,224,178,166,238,138,81,239,212,105,22,61,193,133,77,205,15,111,58,23,111,122,78,179,224,7,41,109,106,110,232,116,187,44,54,50,23,140,184,166,20,107,0,105,100,31,15,223,159,164,113,61,232,189,30,165,241,123,214,127,243,118,148,70,105,183,119,50,2,167,171,14,118,47,71,23,103,127,187,24,246,207,123,41,84,247,178,247,174,119,214,25,100,227,164,123,121,218,131,90,103,253,31,160,234,227,84,248,190,55,234,156,159,190,237,12,52,248,203,243,139,147,215,131,225,251,92,27,221,206,217,217,112,148,70,117,247,178,51,232,255,189,115,6,166,126,158,138,142,59,103,223,167,17,218,189,60,25,158,244,156,199,207,158,181,175,17,49,174,8,138,102,199,136,34,39,192,11,163,67,8,186,181,108,94,50,67,113,103,50,193,190,51,69,126,140,185,40,246,61,23,147,110,24,80,47,72,194,36,150,69,211,36,152,80,47,12,140,73,232,135,164,235,163,56,182,248,207,190,107,47,229,15,231,36,153,143,49,73,229,237,120,225,209,201,76,211,67,49,214,109,214,34,152,38,36,48,225,207,207,73,232,197,216,108,75,21,105,66,165,128,231,152,32,223,77,139,51,123,42,141,8,83,76,136,119,141,73,170,148,217,87,41,45,48,69,113,52,67,62,77,149,82,123,43,157,56,9,166,126,184,208,112,132,249,85,249,4,17,18,102,245,149,43,84,49,242,189,95,16,241,130,84,129,121,66,21,186,136,124,50,219,46,158,162,196,167,74,104,182,87,171,7,204,242,11,60,142,195,201,167,182,246,27,220,16,224,9,197,110,209,11,4,255,156,224,152,14,137,135,3,138,152,232,20,147,185,23,199,240,203,178,151,171,7,169,102,140,174,241,27,21,0,80,228,135,19,228,159,211,144,160,43,92,139,49,237,83,60,183,76,28,71,137,199,227,36,54,171,127,61,31,158,212,98,10,189,184,242,166,183,86,26,62,182,221,214,128,9,180,31,18,13,219,3,15,243,8,130,22,93,30,110,185,182,174,74,219,202,192,219,222,212,74,171,238,57,65,226,251,246,50,195,226,164,34,68,98,156,105,217,109,97,195,172,201,15,158,251,17,72,10,241,135,143,69,190,136,80,48,193,55,150,27,78,146,57,88,206,174,33,215,181,204,191,152,118,45,156,78,97,80,124,99,153,15,73,184,128,239,25,157,251,150,105,218,109,233,138,218,196,15,161,109,8,106,129,162,67,195,168,59,7,47,36,113,15,98,131,64,11,27,220,199,208,83,93,104,131,224,121,120,141,197,96,50,249,40,57,184,34,24,7,166,189,166,10,60,115,122,4,187,37,90,130,52,103,97,212,141,147,208,144,12,24,201,71,15,95,60,127,242,172,93,82,43,235,122,78,26,88,203,137,239,77,62,181,164,229,170,171,92,167,103,40,112,125,252,131,23,123,99,207,247,232,109,23,4,87,96,160,37,248,113,175,104,128,71,143,246,148,205,107,51,207,117,113,96,47,83,135,176,1,144,197,171,116,82,170,14,61,239,93,195,143,129,23,83,28,192,244,98,94,167,109,78,120,155,102,181,156,75,149,91,157,7,214,194,11,220,112,81,99,1,201,90,169,69,33,161,123,142,105,254,250,107,121,201,139,198,166,146,167,79,159,164,254,229,243,233,123,60,62,135,223,152,90,230,34,110,213,235,230,126,177,226,44,140,105,128,230,120,223,108,173,23,50,212,125,179,190,0,147,183,87,24,248,126,49,184,196,120,160,2,54,12,194,8,7,142,178,172,133,175,41,155,170,131,56,244,49,212,191,2,72,161,105,48,189,29,98,110,83,108,82,124,3,252,82,95,103,99,38,139,127,74,18,220,94,181,51,106,124,52,237,194,141,43,2,100,113,140,229,208,48,147,109,65,99,130,50,4,177,6,178,216,225,33,118,23,2,83,119,11,147,17,20,212,92,62,23,177,82,236,164,113,59,14,221,91,46,155,192,23,38,16,108,106,61,100,234,53,122,27,97,185,34,230,55,235,173,146,57,136,137,40,26,199,1,186,46,19,3,81,24,26,84,47,130,152,231,205,20,151,115,152,82,139,11,124,169,30,196,17,151,51,104,18,250,113,109,26,146,30,2,242,216,199,172,123,206,17,159,235,97,37,187,17,166,91,50,245,86,97,233,144,202,118,117,213,214,204,108,165,181,192,7,118,123,76,48,250,212,86,150,16,167,147,86,49,204,9,246,67,228,90,69,109,126,62,105,165,54,167,30,245,177,232,144,143,198,216,231,22,154,35,47,120,139,17,116,79,25,40,83,40,226,241,83,69,139,245,12,188,11,136,169,29,197,103,87,88,195,94,138,79,71,58,192,220,47,209,145,163,89,211,228,78,5,195,10,81,13,69,48,236,96,245,57,116,189,107,195,115,157,138,231,154,251,194,234,158,187,255,192,172,64,236,195,232,115,42,116,17,178,109,87,50,15,98,3,182,31,174,65,69,72,25,160,174,109,199,164,191,224,219,102,181,143,160,212,60,156,61,59,82,160,188,195,76,86,7,225,225,140,212,133,70,28,161,128,55,239,151,183,206,171,25,252,239,193,2,86,235,74,10,120,141,252,4,115,64,134,33,208,234,208,153,35,179,104,86,113,170,251,179,217,21,230,141,223,209,174,227,132,210,80,88,118,76,131,2,3,166,17,6,115,24,104,24,194,60,0,21,174,205,215,91,43,167,91,53,216,220,105,231,170,36,209,214,10,98,217,43,243,147,168,116,148,122,137,89,143,147,147,213,197,170,79,195,100,50,227,235,112,43,155,16,237,37,174,69,4,179,81,123,44,54,176,108,48,106,36,36,68,149,211,109,175,170,28,5,28,241,37,24,114,225,94,85,215,166,7,117,150,255,186,98,73,140,65,214,126,188,97,148,138,165,133,181,102,101,62,119,28,179,105,190,52,193,13,176,133,112,97,247,97,106,173,178,30,201,202,94,80,57,58,244,130,40,161,6,91,150,156,10,175,49,14,111,42,162,205,66,147,97,208,101,222,82,141,178,45,89,46,0,249,214,189,178,19,151,186,138,69,222,103,222,205,252,60,146,54,161,66,35,67,44,134,6,187,98,106,169,15,246,251,171,154,198,97,107,160,26,146,107,182,172,154,72,139,250,222,209,33,50,114,147,78,132,92,49,238,46,78,171,70,249,172,163,205,56,219,180,229,148,195,251,31,77,243,253,63,250,247,63,255,117,88,71,224,105,160,112,39,21,126,223,242,57,108,202,43,228,8,145,251,16,98,183,65,159,195,167,84,63,71,199,191,15,29,118,83,245,57,116,74,245,115,116,198,119,209,169,139,16,202,246,173,142,35,135,219,75,19,120,102,81,55,245,200,188,178,129,183,184,57,171,238,78,92,86,216,74,125,82,160,62,252,158,49,87,147,203,97,29,6,69,126,58,97,227,151,197,231,23,174,119,250,16,248,210,245,174,12,67,95,239,56,69,255,254,20,121,28,222,155,100,14,101,141,38,185,63,77,49,124,239,205,51,15,179,70,116,124,127,162,124,32,221,155,103,14,101,141,230,228,254,52,229,176,185,55,209,2,206,150,29,25,127,49,249,159,89,108,217,199,129,56,237,254,54,11,175,182,143,34,236,214,73,130,203,250,98,87,85,186,115,19,91,45,94,167,98,204,61,152,44,27,240,63,186,113,42,205,6,252,18,187,166,74,113,19,95,41,107,236,242,82,192,104,39,186,82,45,14,178,203,1,78,159,51,57,196,57,71,176,246,214,238,19,10,209,32,94,197,190,174,173,151,240,113,76,111,125,182,117,102,213,91,99,31,77,62,181,197,42,21,36,243,82,207,7,252,181,100,139,163,225,84,196,239,49,149,170,248,202,111,182,97,37,172,111,59,121,103,239,142,255,247,137,230,19,118,101,89,160,181,131,27,88,173,47,114,66,231,85,43,127,133,151,90,73,108,245,102,4,79,157,138,102,116,128,229,125,213,168,152,217,166,172,236,230,111,205,238,69,176,148,27,171,199,60,87,203,3,192,215,24,187,93,241,109,45,253,48,140,90,108,245,88,217,252,126,211,50,81,228,137,150,17,104,79,61,31,124,101,165,171,9,187,206,231,79,49,223,88,116,230,197,192,135,82,56,23,178,142,153,182,3,135,191,135,124,89,57,160,225,129,56,210,153,237,21,95,227,76,46,55,171,250,186,196,34,21,232,56,17,9,231,17,181,204,17,26,27,52,52,228,25,23,126,89,98,64,24,33,129,174,218,47,197,125,231,30,235,79,77,40,65,13,11,62,109,123,137,124,76,24,196,12,81,134,105,184,33,142,141,32,164,6,190,241,98,106,180,126,250,137,71,111,201,82,184,190,210,241,247,242,175,236,118,16,251,120,178,101,32,137,242,181,11,2,53,98,68,241,151,140,25,145,113,208,218,96,225,204,7,220,118,138,69,153,161,139,246,12,35,254,206,196,200,139,159,187,206,3,74,34,26,195,76,187,196,122,2,243,136,187,50,223,163,119,253,93,187,195,92,189,174,182,111,26,124,86,19,209,46,187,21,123,191,96,203,86,33,39,71,29,108,38,204,220,69,201,106,141,75,231,199,63,138,11,186,185,131,11,75,244,248,131,200,196,20,71,119,176,225,57,54,95,215,24,159,122,87,9,193,156,0,127,142,47,28,170,149,206,4,201,208,94,67,203,23,10,1,32,230,135,119,250,210,255,65,130,127,116,214,82,8,212,225,167,13,230,129,109,38,47,56,191,190,178,214,235,86,77,73,53,173,162,185,177,144,70,197,189,73,99,231,29,162,179,26,9,19,48,57,123,91,5,60,136,16,150,151,48,242,230,240,171,14,219,238,70,25,207,90,148,196,51,107,121,211,162,113,245,182,149,69,15,44,16,133,236,138,123,242,214,178,188,90,37,230,250,240,241,55,110,79,229,84,125,85,225,254,190,63,24,24,175,122,70,199,224,125,55,238,218,42,94,161,228,10,255,247,54,240,60,105,141,77,143,42,87,203,22,229,191,179,203,210,204,48,254,114,255,167,114,32,183,136,81,103,249,45,112,178,158,71,226,141,25,187,198,52,241,253,219,61,253,64,142,88,23,48,236,34,89,118,152,112,39,23,21,88,31,233,117,198,200,47,94,208,10,215,28,194,14,177,20,56,76,40,91,125,138,117,162,226,12,184,61,105,43,231,119,45,199,149,159,28,244,251,192,236,9,187,228,25,36,159,241,218,218,242,124,2,123,242,134,249,178,209,106,150,2,136,75,29,121,111,192,178,148,172,220,250,11,60,52,168,170,9,83,167,169,238,136,214,209,228,165,0,67,224,135,225,180,43,80,251,142,158,104,231,87,30,200,252,220,246,25,245,229,158,125,109,147,184,91,109,249,110,173,139,178,71,166,44,177,185,181,94,83,204,171,172,89,57,127,124,70,171,98,200,11,185,74,22,84,185,40,60,211,197,50,47,130,79,65,184,8,248,68,196,78,65,252,232,146,206,29,171,7,106,74,96,229,71,78,46,193,251,209,163,180,228,48,159,121,34,182,91,42,193,163,232,239,182,142,233,56,185,64,177,151,178,86,62,165,77,132,207,65,154,222,41,147,75,15,100,58,167,250,204,114,55,149,36,75,212,84,146,52,45,83,9,68,30,166,250,74,211,46,237,182,98,146,101,47,73,29,115,191,124,234,81,179,96,105,23,50,238,138,180,198,86,167,153,242,147,196,182,49,218,200,99,165,39,53,205,113,28,35,88,60,180,204,25,61,255,78,244,74,46,41,98,125,17,14,132,184,18,135,34,152,52,164,156,7,93,150,202,24,243,89,60,246,121,4,182,204,125,248,159,231,166,73,109,189,149,220,194,85,214,10,31,208,219,154,9,118,104,69,187,88,41,107,67,140,250,109,141,208,29,26,201,157,69,75,13,38,39,136,173,70,219,161,33,61,163,65,20,87,189,152,61,135,241,220,72,249,51,15,59,102,50,13,141,5,164,81,80,73,162,242,230,210,43,123,54,48,171,133,6,101,202,153,150,109,38,46,246,91,155,152,68,147,187,169,68,147,28,151,220,12,182,25,120,186,3,240,116,19,48,123,55,217,12,189,131,249,162,241,38,104,246,196,181,25,218,223,1,218,223,4,205,95,165,54,99,147,29,176,73,9,118,46,95,86,173,240,210,241,49,236,44,48,15,52,254,203,145,137,212,240,205,67,60,139,110,15,102,183,150,74,199,176,211,156,86,21,231,8,224,175,177,214,116,113,74,209,178,67,21,72,62,129,85,33,121,193,157,88,249,68,232,12,142,227,25,89,95,154,44,121,251,110,26,69,157,136,132,81,86,174,222,196,10,216,141,117,236,13,180,238,128,87,79,101,34,171,95,123,61,201,210,75,189,248,216,139,39,4,83,121,217,41,230,114,126,41,171,63,216,152,118,149,127,174,21,200,247,30,40,23,91,185,245,114,46,103,249,51,252,187,134,89,234,166,118,89,203,139,75,164,196,80,128,226,22,151,93,138,90,234,50,87,97,170,66,190,17,85,247,40,226,10,86,239,24,39,196,159,49,249,9,171,165,53,148,91,187,242,55,198,176,229,96,134,143,224,4,134,173,250,63,126,58,222,175,95,85,77,211,150,111,143,34,194,182,64,43,180,128,37,37,75,142,76,146,94,248,84,69,179,191,17,29,246,175,253,31,236,123,73,87,116,55,0,0 };
