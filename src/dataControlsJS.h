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
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
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
"</div>");requestOrientationPermission();break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);break;case UPDATE_SLIDER:slider_move($("#sl"+data.id),data.value,"100",false);break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
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

const uint8_t JS_CONTROLS_GZIP[3340] PROGMEM = { 31,139,8,0,70,56,242,94,2,255,237,90,235,118,219,198,17,254,239,167,128,224,28,19,168,40,94,124,139,67,10,242,161,41,218,102,67,139,170,68,197,57,117,92,157,37,177,20,113,12,2,200,98,33,74,97,248,26,125,144,62,82,159,164,179,55,96,1,130,20,109,37,105,122,220,31,150,137,217,217,111,191,157,153,189,207,36,12,98,106,92,244,47,251,39,253,81,191,51,184,124,115,209,119,30,55,26,237,137,42,56,235,13,134,157,99,144,53,149,236,244,184,51,234,93,14,95,191,62,239,141,156,166,174,59,234,143,122,3,71,19,156,66,205,66,61,46,202,192,250,151,93,38,121,156,87,234,10,173,199,153,214,171,139,209,104,120,226,60,201,235,73,105,179,241,36,211,28,116,94,1,137,167,121,69,33,108,54,158,102,122,231,239,251,163,238,219,222,153,243,44,175,154,202,155,141,103,154,246,160,127,12,178,231,5,93,33,109,54,158,103,154,39,23,239,94,129,236,219,188,166,148,54,27,223,106,230,234,253,56,2,195,159,94,140,156,23,121,109,173,164,217,120,145,213,120,115,214,57,125,235,124,39,5,157,227,99,33,185,60,29,246,79,152,170,44,232,14,122,157,51,169,220,108,124,167,181,216,121,229,52,11,14,17,50,205,33,231,189,65,175,11,104,5,159,40,113,83,243,202,240,116,212,103,246,47,184,69,137,155,154,91,222,245,65,80,240,10,151,53,53,159,188,235,252,232,52,11,254,224,178,166,238,138,81,239,212,105,22,61,193,133,77,205,15,111,58,23,111,122,78,179,224,7,41,109,106,110,232,116,187,44,54,50,23,140,184,166,20,107,0,105,100,31,15,223,159,164,113,61,232,189,30,165,241,123,214,127,243,118,148,70,105,183,119,50,2,167,171,14,118,47,71,23,103,127,187,24,246,207,123,41,84,247,178,247,174,119,214,25,100,227,164,123,121,218,131,90,103,253,31,160,234,227,84,248,190,55,234,156,159,190,237,12,52,248,203,243,139,147,215,131,225,251,92,27,221,206,217,217,112,148,70,117,247,178,51,232,255,189,115,6,166,126,158,138,142,59,103,223,167,17,218,189,60,25,158,244,156,199,207,158,181,175,17,49,174,8,138,102,199,136,34,39,192,11,163,67,8,186,181,108,94,50,67,113,103,50,193,190,51,69,126,140,185,40,246,61,23,147,110,24,80,47,72,194,36,150,69,211,36,152,80,47,12,140,73,232,135,164,235,163,56,182,248,207,190,107,47,229,15,231,36,153,143,49,73,229,237,120,225,209,201,76,211,67,49,214,109,214,34,152,38,36,48,225,207,207,73,232,197,216,108,75,21,105,66,165,128,231,152,32,223,77,139,51,123,42,141,8,83,76,136,119,141,73,170,148,217,87,41,45,48,69,113,52,67,62,77,149,82,123,43,157,56,9,166,126,184,208,112,132,249,85,249,4,17,18,102,245,149,43,84,49,242,189,95,16,241,130,84,129,121,66,21,186,136,124,50,219,46,158,162,196,167,74,104,182,87,171,7,204,242,11,60,142,195,201,167,182,246,27,220,16,224,9,197,110,209,11,4,255,156,224,152,14,137,135,3,138,152,232,20,147,185,23,199,240,203,178,151,171,7,169,102,140,174,241,27,21,0,80,228,135,19,228,159,211,144,160,43,92,139,49,237,83,60,183,76,28,71,137,199,227,36,54,171,127,61,31,158,212,98,10,189,184,242,166,183,86,26,62,182,221,214,128,9,180,31,18,13,219,3,15,243,8,130,22,93,30,110,185,182,174,74,219,202,192,219,222,212,74,171,238,57,65,226,251,246,50,195,226,164,34,68,98,156,105,217,109,97,195,172,201,15,158,251,17,72,10,241,135,143,69,190,136,80,48,193,55,150,27,78,146,57,88,206,174,33,215,181,204,191,152,118,45,156,78,97,80,124,99,153,15,73,184,128,239,25,157,251,150,105,218,109,233,138,218,196,15,161,109,8,106,129,162,67,195,168,59,7,47,36,113,15,98,131,64,11,27,220,199,208,83,93,104,131,224,121,120,141,197,96,50,249,40,57,184,34,24,7,166,189,166,10,60,115,122,4,187,37,90,130,52,103,97,212,141,147,208,144,12,24,201,71,15,95,60,127,242,172,93,82,43,235,122,78,26,88,203,137,239,77,62,181,164,229,170,171,92,167,103,40,112,125,252,131,23,123,99,207,247,232,109,23,4,87,96,160,37,248,113,175,104,128,71,143,246,148,205,107,51,207,117,113,96,47,83,135,176,1,144,197,171,116,82,170,14,61,239,93,195,143,129,23,83,28,192,244,98,94,167,109,78,120,155,102,181,156,75,149,91,157,7,214,194,11,220,112,81,99,1,201,90,169,69,33,161,123,142,105,254,250,107,121,201,139,198,166,146,167,79,159,164,254,229,243,233,123,60,62,135,223,152,90,230,34,110,213,235,230,126,177,226,44,140,105,128,230,120,223,108,173,23,50,212,125,179,190,0,147,183,87,24,248,126,49,184,196,120,160,2,54,12,194,8,7,142,178,172,133,175,41,155,170,131,56,244,49,212,191,2,72,161,105,48,189,29,98,110,83,108,82,124,3,252,82,95,103,99,38,139,127,74,18,220,94,181,51,106,124,52,237,194,141,43,2,100,113,140,229,208,48,147,109,65,99,130,50,4,177,6,178,216,225,33,86,64,96,165,110,97,238,129,130,154,203,167,30,86,138,157,52,76,199,161,123,203,101,19,248,194,4,98,75,45,127,76,189,70,111,35,44,23,192,252,222,188,85,50,229,48,17,69,227,56,64,215,101,98,232,8,140,4,170,23,65,136,243,102,138,171,55,204,160,197,245,188,84,15,194,134,203,25,52,9,253,184,54,13,73,15,1,121,236,99,214,61,231,136,91,3,22,174,27,97,169,37,83,111,21,86,10,169,108,87,87,109,205,170,86,90,11,76,110,183,199,4,163,79,109,101,9,113,24,105,21,163,154,96,63,68,174,85,212,230,199,145,86,106,115,234,81,31,139,14,249,104,140,125,110,161,57,242,130,183,24,65,247,148,129,50,133,34,30,63,68,180,88,207,192,187,128,152,218,81,124,118,133,53,236,165,248,116,164,3,204,253,18,29,57,120,53,77,238,84,48,172,16,213,80,4,163,12,22,155,67,215,187,54,60,215,169,120,174,185,47,172,238,185,251,15,204,10,132,58,12,54,167,66,23,33,219,101,37,243,32,54,96,183,225,26,84,132,148,1,234,218,238,75,250,11,190,109,86,251,8,74,205,195,217,179,35,5,202,59,204,100,117,16,30,206,72,93,104,196,17,10,120,243,126,121,235,188,154,193,255,30,44,96,113,174,164,128,215,200,79,48,7,100,24,2,173,14,157,57,50,139,102,21,135,184,63,155,93,97,154,248,29,237,58,78,40,13,133,101,199,52,40,48,96,26,97,48,135,129,134,33,204,3,80,225,218,124,121,181,114,186,85,131,77,149,118,174,74,18,109,173,32,86,185,50,63,137,74,71,169,151,152,245,56,57,89,93,44,242,52,76,38,51,190,236,182,178,249,207,94,226,90,68,48,27,181,199,98,191,202,6,163,70,66,66,84,57,221,246,170,202,81,192,17,95,130,33,215,233,85,117,109,122,80,71,247,175,43,150,196,24,100,237,199,27,70,169,88,90,88,107,86,230,115,199,49,155,230,75,19,220,0,59,6,23,54,27,166,214,42,235,145,172,236,5,149,163,67,47,136,18,106,176,101,201,169,240,26,227,240,166,34,218,44,52,25,6,93,230,45,213,40,219,129,229,2,144,239,212,43,59,113,169,171,88,228,125,230,221,204,207,35,105,19,42,52,50,196,98,104,176,27,165,150,250,96,191,191,170,105,28,182,6,170,33,185,102,203,170,137,180,168,239,29,29,34,35,55,233,68,200,21,227,238,226,180,106,148,207,58,218,140,179,77,91,78,57,188,255,209,52,223,255,163,127,255,243,95,135,117,4,158,6,10,119,82,225,215,43,159,195,166,188,66,142,16,185,15,33,118,249,243,57,124,74,245,115,116,252,251,208,97,23,83,159,67,167,84,63,71,103,124,23,157,186,8,161,108,223,234,56,114,184,189,52,129,103,22,117,83,143,204,43,27,120,139,139,178,234,238,196,101,133,173,212,39,5,234,195,239,25,115,53,185,28,214,97,80,228,167,19,54,126,89,124,126,225,122,167,15,129,47,93,239,202,48,244,245,142,83,244,239,79,145,199,225,189,73,230,80,214,104,146,251,211,20,195,247,222,60,243,48,107,68,199,247,39,202,7,210,189,121,230,80,214,104,78,238,79,83,14,155,123,19,45,224,108,217,145,241,7,146,255,153,197,150,125,28,136,211,238,111,179,240,106,251,40,194,46,153,36,184,172,47,118,85,165,59,55,177,213,226,117,42,198,220,131,201,178,1,255,163,27,167,210,108,192,47,177,107,170,20,55,241,149,178,198,46,47,5,140,118,162,43,213,226,32,187,28,224,244,57,147,67,156,115,4,107,111,237,62,161,16,13,226,17,236,235,218,122,9,31,199,244,214,103,91,103,86,189,53,246,209,228,83,91,172,82,65,50,47,245,124,192,31,71,182,56,26,78,69,252,218,82,169,138,175,252,102,27,86,194,250,182,147,119,246,204,248,127,159,104,62,97,55,148,5,90,59,184,129,213,250,34,39,116,94,181,242,87,120,169,149,196,86,111,70,240,212,169,104,70,7,88,222,87,141,138,153,109,202,202,110,254,214,236,94,4,75,185,177,122,204,115,181,60,0,124,141,177,219,21,223,214,210,15,195,168,197,86,143,149,205,239,55,45,19,69,158,104,25,129,246,212,243,193,87,86,186,154,176,219,123,254,242,242,141,69,103,94,12,124,40,133,115,33,235,152,105,59,112,248,123,200,151,149,3,26,30,136,35,157,217,94,241,53,206,228,114,179,170,175,75,44,82,129,142,19,145,112,30,81,203,28,161,177,65,67,67,158,113,225,151,37,6,132,17,18,232,170,253,82,220,119,238,177,254,212,132,18,212,176,224,211,182,151,200,199,132,65,204,16,101,152,134,27,226,216,8,66,106,224,27,47,166,70,235,167,159,120,244,150,44,133,235,43,29,127,30,255,202,110,7,177,143,39,91,6,146,40,95,187,32,80,35,70,20,127,201,152,17,9,6,173,13,22,206,124,192,109,167,88,148,25,186,104,207,48,226,207,74,140,188,248,185,235,60,160,36,162,49,204,180,75,172,39,48,143,184,43,243,61,122,215,223,181,59,204,213,235,106,251,166,193,103,53,17,237,178,91,177,247,11,182,108,21,114,114,212,193,102,194,204,93,148,172,214,184,116,126,252,163,184,160,155,59,184,176,188,142,63,136,76,76,113,116,7,27,158,82,243,117,141,241,169,119,149,16,204,9,240,215,247,194,161,90,233,76,144,12,237,53,180,124,161,16,0,98,126,120,167,15,251,31,36,248,71,103,45,99,64,29,126,218,96,30,216,102,242,130,243,235,43,107,189,110,213,148,84,211,42,154,27,11,89,83,220,155,52,118,222,33,58,171,145,48,1,147,179,167,84,192,131,8,97,105,8,35,111,14,191,234,176,237,110,148,241,172,69,73,60,179,150,55,45,26,87,111,91,89,244,192,2,81,72,166,184,39,111,45,169,171,85,98,174,15,31,127,227,246,84,10,213,87,21,238,239,251,131,129,241,170,103,116,12,222,119,227,174,173,226,21,74,174,240,127,111,3,207,115,212,216,244,168,82,179,108,81,254,59,187,44,77,4,227,15,245,127,42,7,114,139,24,117,150,206,2,39,235,121,36,222,152,177,107,76,19,223,191,221,211,15,228,136,117,1,195,46,146,37,131,9,119,114,81,129,245,145,94,103,140,252,226,5,173,112,205,33,236,16,75,129,195,132,178,213,167,88,39,42,206,128,219,115,180,114,126,215,82,90,249,201,65,191,15,204,158,176,75,158,65,242,9,174,173,45,207,39,176,39,111,152,47,27,173,102,41,128,184,212,145,247,6,44,41,201,226,251,44,141,135,6,85,53,97,234,52,213,29,209,58,154,188,20,96,8,252,48,156,118,5,106,223,209,19,237,252,202,3,153,159,219,62,163,190,220,179,175,109,18,119,171,45,223,173,117,81,246,200,148,229,49,183,214,107,138,121,149,53,43,231,143,207,104,85,12,121,33,87,185,129,42,121,133,39,182,88,230,69,240,41,8,23,1,159,136,216,41,136,31,93,210,185,99,245,64,77,9,172,252,200,201,229,115,63,122,148,150,28,230,51,79,196,118,75,37,120,228,246,91,108,225,208,49,29,39,23,40,246,82,214,202,103,176,137,240,57,72,179,57,101,46,233,129,204,222,84,159,89,170,166,146,100,121,153,74,146,102,97,42,129,72,187,84,95,105,150,165,221,86,76,178,100,37,169,99,238,151,79,61,106,22,44,237,66,198,93,145,214,216,234,52,83,126,146,216,54,70,27,121,172,244,28,166,57,142,99,4,139,135,150,57,163,167,219,137,94,201,37,69,172,47,194,129,16,87,142,26,172,82,206,131,46,203,92,140,249,44,30,251,60,2,91,230,62,252,207,83,209,164,182,222,74,110,225,42,107,133,15,232,109,205,4,59,180,162,93,172,148,181,33,70,253,182,70,232,14,141,228,206,162,165,6,147,19,196,86,163,237,208,144,158,209,32,138,171,94,204,158,195,120,42,164,252,153,135,29,51,153,134,198,2,210,40,168,36,81,121,115,233,149,61,27,152,213,66,131,50,229,76,203,54,19,23,251,173,77,76,162,201,221,84,162,73,142,75,110,6,219,12,60,221,1,120,186,9,152,189,155,108,134,222,193,124,209,120,19,52,123,226,218,12,237,239,0,237,111,130,230,175,82,155,177,201,14,216,164,4,59,151,30,171,86,120,233,248,24,118,22,152,7,26,255,229,200,188,105,248,230,33,158,69,183,7,179,91,75,165,99,216,105,10,171,138,115,4,240,215,88,107,186,56,165,104,201,160,10,36,159,175,170,144,188,224,78,172,124,222,115,6,199,241,140,172,47,77,150,171,125,55,141,162,78,68,194,40,43,87,111,98,5,236,198,58,246,6,90,119,192,171,167,50,145,196,175,189,158,100,217,164,94,124,236,197,19,130,169,188,236,20,115,57,191,148,213,31,108,76,187,202,63,215,10,228,123,15,148,139,173,220,122,57,151,179,252,25,254,93,195,44,117,83,187,172,229,197,37,82,98,40,64,113,139,203,46,69,45,117,153,171,48,85,33,223,136,170,123,20,113,5,171,119,140,19,226,207,152,252,132,213,210,26,202,173,93,249,27,99,216,114,48,195,71,112,2,195,86,253,31,63,29,239,215,175,170,166,105,203,183,71,17,97,91,160,21,90,192,114,144,37,71,38,73,47,124,170,162,217,223,136,14,251,215,254,15,211,56,193,141,99,55,0,0 };
