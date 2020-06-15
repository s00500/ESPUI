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
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
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

const uint8_t JS_CONTROLS_GZIP[3350] PROGMEM = { 31,139,8,0,19,56,231,94,2,255,237,90,107,114,219,70,18,254,239,83,192,112,202,4,86,20,31,126,197,33,5,185,104,138,182,185,161,37,173,68,197,169,117,188,170,33,49,20,81,6,1,4,24,232,17,69,85,251,107,47,176,55,217,147,228,12,123,146,237,233,153,1,6,32,72,209,86,146,205,150,247,135,101,162,167,231,235,158,238,158,233,121,244,52,12,18,102,156,12,79,135,251,195,241,176,55,58,125,125,50,116,30,181,90,221,169,106,56,26,140,14,122,123,64,107,43,218,225,94,111,60,56,61,120,245,234,120,48,118,218,58,239,120,56,30,140,28,141,112,8,61,75,253,144,148,131,13,79,251,156,242,168,200,212,23,92,143,114,174,151,39,227,241,193,190,243,184,200,39,169,237,214,227,156,115,212,123,9,74,60,41,50,10,98,187,245,36,231,59,126,55,28,247,223,12,142,156,167,69,214,140,222,110,61,213,184,71,195,61,160,61,43,241,10,106,187,245,44,231,220,63,121,251,18,104,95,23,57,37,181,221,250,90,51,215,224,251,49,24,254,240,100,236,60,47,114,107,45,237,214,243,188,199,235,163,222,225,27,231,27,73,232,237,237,9,202,233,225,193,112,159,179,202,134,254,104,208,59,146,204,237,214,55,154,196,222,75,167,93,114,136,160,105,14,57,30,140,6,125,64,43,249,68,145,219,154,87,14,14,199,67,110,255,146,91,20,185,173,185,229,237,16,8,37,175,32,173,173,249,228,109,239,123,167,93,242,7,210,218,186,43,198,131,67,167,93,246,4,18,219,154,31,94,247,78,94,15,156,118,201,15,146,218,214,220,208,235,247,121,108,228,46,24,35,167,36,107,0,89,100,239,29,188,219,207,226,122,52,120,53,206,226,247,104,248,250,205,56,139,210,254,96,127,12,78,87,3,236,159,142,79,142,254,114,114,48,60,30,100,80,253,211,193,219,193,81,111,148,207,147,254,233,225,0,122,29,13,191,131,174,143,50,226,187,193,184,119,124,248,166,55,210,224,79,143,79,246,95,141,14,222,21,100,244,123,71,71,7,227,44,170,251,167,189,209,240,175,189,35,48,245,179,140,180,215,59,250,54,139,208,254,233,254,193,254,192,121,244,244,105,247,156,196,198,89,76,162,249,30,97,196,9,232,133,209,139,99,114,101,217,216,50,39,73,111,58,165,190,51,35,126,66,145,148,248,158,75,227,126,24,48,47,72,195,52,145,77,179,52,152,50,47,12,140,105,232,135,113,223,39,73,98,225,207,161,107,95,203,31,206,126,186,152,208,56,163,119,147,11,143,77,231,26,31,73,168,110,179,78,76,89,26,7,38,252,249,49,13,189,132,154,93,201,34,77,168,24,232,130,198,196,119,179,230,220,158,138,35,162,140,198,177,119,78,227,140,41,183,175,98,186,160,140,36,209,156,248,44,99,202,236,173,120,146,52,152,249,225,133,134,35,204,175,218,167,36,142,195,188,191,114,133,106,38,190,247,19,137,189,32,99,224,158,80,141,46,137,63,154,93,151,206,72,234,51,69,52,187,55,55,247,184,229,47,232,36,9,167,31,187,218,111,112,67,64,167,140,186,101,47,196,244,199,148,38,236,32,246,104,192,8,39,29,210,120,225,37,9,252,178,236,235,155,123,25,103,66,206,233,107,21,0,208,228,135,83,226,31,179,48,38,103,180,145,80,54,100,116,97,153,52,137,82,15,227,36,49,235,127,62,62,216,111,36,12,70,113,230,205,174,172,44,124,108,187,171,1,199,32,63,140,53,108,15,60,140,17,4,18,93,12,183,130,172,179,74,89,57,120,215,155,89,89,215,251,78,144,250,190,125,157,99,161,82,17,137,19,154,115,217,93,97,195,92,228,123,207,253,0,74,10,242,251,15,101,125,73,204,192,4,95,89,110,56,77,23,96,57,187,65,92,215,50,255,100,218,141,112,54,131,73,241,149,101,62,136,195,11,248,158,179,133,111,153,166,221,149,174,104,76,253,16,100,67,80,11,20,29,26,102,221,49,120,33,77,6,16,27,49,72,88,225,62,142,158,241,130,140,152,46,194,115,42,38,147,137,179,100,251,44,166,52,48,237,37,86,208,179,192,23,83,183,130,75,40,141,90,24,77,99,63,52,164,6,92,201,135,15,158,63,123,252,180,91,209,43,31,122,129,26,88,215,83,223,155,126,236,72,203,213,111,10,131,158,147,192,245,233,119,94,226,77,60,223,99,87,125,32,156,129,129,174,193,143,247,203,6,120,248,240,190,178,121,99,238,185,46,13,236,235,204,33,124,2,228,241,42,157,148,177,195,200,7,231,240,99,228,37,140,6,176,188,152,231,153,204,41,202,52,235,213,186,212,209,234,24,88,23,94,224,134,23,13,30,144,92,74,35,10,99,118,223,49,205,159,127,174,110,121,222,90,213,242,228,201,227,204,191,184,158,190,163,147,99,248,77,153,101,94,36,157,102,211,220,42,119,156,135,9,11,200,130,110,153,157,229,70,142,186,101,54,47,192,228,221,27,10,250,126,54,184,196,184,167,2,54,12,194,136,6,142,178,172,69,207,25,95,170,131,36,244,41,244,63,3,72,193,105,112,190,13,98,110,85,108,50,122,9,250,101,190,206,231,76,30,255,44,78,105,247,166,155,171,134,179,105,19,221,144,17,32,203,115,172,128,70,57,109,13,26,39,84,33,136,28,200,99,7,67,172,132,192,91,221,210,218,3,13,13,23,151,30,222,74,157,44,76,39,161,123,133,180,41,124,209,24,98,75,165,63,206,222,96,87,17,149,9,176,184,55,239,64,116,34,71,57,241,194,226,87,78,197,149,124,224,113,164,195,240,88,28,250,73,99,22,198,3,2,114,169,79,185,102,206,46,14,4,114,206,165,24,228,53,103,239,148,22,121,201,108,215,111,186,154,65,172,172,23,88,203,238,78,98,74,62,118,213,32,196,57,162,83,14,200,152,250,33,113,173,50,55,158,36,58,153,185,152,199,124,42,6,228,147,9,245,49,168,22,196,11,222,80,2,195,83,107,89,206,80,198,195,253,127,135,143,12,28,3,136,93,101,71,241,217,23,214,176,175,197,167,195,225,25,153,152,91,21,60,114,222,105,156,152,2,192,176,130,212,32,17,76,16,200,19,59,174,119,110,120,174,83,243,92,115,75,88,221,115,183,238,153,53,136,82,152,39,78,141,93,132,124,131,148,46,130,196,128,141,130,107,48,17,13,6,176,107,27,39,233,47,248,182,121,239,93,104,53,119,230,79,119,21,40,14,152,211,154,64,220,153,199,77,193,145,68,36,64,241,126,181,116,236,102,224,223,237,11,200,171,181,12,240,156,248,41,69,64,142,33,208,154,48,152,93,179,108,86,113,254,250,163,217,21,102,248,111,104,215,73,202,88,40,44,59,97,65,73,3,206,17,6,11,152,104,20,194,60,0,22,228,198,204,104,21,120,235,6,95,229,236,66,151,52,90,219,65,36,168,42,63,137,78,187,153,151,184,245,80,57,217,93,228,103,22,166,211,57,102,204,78,190,116,217,215,180,17,197,148,207,218,61,177,213,228,147,81,83,66,66,212,81,221,238,77,29,81,192,17,159,131,33,83,236,77,125,105,121,80,167,238,47,43,150,196,28,228,242,147,21,179,84,100,5,46,205,202,125,238,56,102,219,124,97,130,27,32,217,187,176,79,48,53,169,124,68,178,179,23,212,118,119,188,32,74,153,193,51,138,83,195,30,147,240,178,38,100,150,68,134,65,159,123,75,9,229,155,167,66,0,226,38,187,182,145,46,77,21,139,56,102,28,102,113,29,201,68,168,208,200,17,203,161,193,47,131,58,234,131,255,254,162,150,241,128,100,238,148,57,91,118,77,165,69,125,111,119,135,24,133,69,39,34,174,152,119,39,135,117,163,122,213,209,86,156,117,220,114,201,193,241,71,179,226,248,119,127,249,215,191,255,254,207,95,254,177,211,36,224,109,80,227,86,117,240,118,228,83,52,170,238,80,80,42,190,171,82,252,254,230,83,116,170,228,47,168,228,223,85,37,126,191,244,41,42,85,242,23,84,154,108,162,82,83,132,84,190,5,117,28,57,253,94,152,160,107,30,133,51,47,94,212,86,232,46,238,188,234,155,43,47,59,172,85,127,90,82,255,224,91,174,185,90,108,118,154,48,73,138,203,11,159,207,60,94,63,51,255,233,83,226,115,243,95,21,134,158,255,80,69,255,238,42,98,60,222,89,201,2,202,146,154,241,221,213,20,83,249,206,122,22,97,150,20,157,220,93,81,156,76,119,214,179,128,178,164,230,244,238,106,202,105,115,103,69,75,56,107,118,104,248,214,241,63,147,124,249,199,182,56,253,254,58,137,88,219,87,197,252,190,72,130,203,254,98,151,85,185,147,19,91,47,236,83,51,22,30,44,150,45,248,159,92,58,181,118,11,126,137,93,84,173,188,169,175,85,9,59,61,21,48,218,9,175,146,11,65,54,57,208,233,107,38,66,28,35,130,117,127,233,126,161,20,13,226,61,235,203,218,138,9,31,39,236,202,231,91,105,222,189,51,241,201,244,99,87,100,169,32,93,84,122,62,192,119,142,53,142,134,83,18,222,64,42,86,241,85,220,124,67,38,108,174,59,137,231,47,134,255,247,137,230,19,126,217,88,82,107,3,55,240,94,159,229,132,222,203,142,52,106,2,91,18,126,41,170,172,36,182,123,243,152,206,156,154,102,116,128,197,177,106,170,152,249,166,76,100,11,142,198,183,254,96,53,29,81,217,189,12,150,233,198,251,113,207,53,138,0,240,53,161,110,95,124,91,215,126,24,70,29,158,61,110,108,188,170,180,76,18,121,66,50,1,238,153,231,131,175,172,44,155,240,139,120,124,68,249,202,98,115,47,1,125,24,131,115,34,31,152,105,59,112,24,124,128,105,101,155,133,219,226,136,103,118,111,48,199,153,72,55,235,122,94,226,145,10,234,56,81,28,46,34,102,153,99,50,49,88,104,200,51,47,252,178,196,132,48,194,24,134,106,191,48,241,118,254,62,31,79,67,48,65,15,11,62,109,251,154,248,52,230,16,115,194,56,166,225,134,52,49,130,144,25,244,210,75,152,209,249,225,7,140,222,138,84,184,156,233,240,165,251,11,187,45,164,62,157,174,153,72,162,125,233,194,64,205,24,209,252,57,115,70,212,10,116,86,88,56,247,1,218,78,105,81,101,232,178,61,195,8,95,136,184,242,226,231,166,235,128,162,8,97,148,115,87,88,79,96,238,162,43,139,35,122,59,220,116,56,220,213,203,108,91,166,129,171,154,136,118,57,172,196,251,137,90,182,10,57,57,235,96,51,97,22,46,78,110,150,116,233,125,255,123,233,66,46,111,209,133,151,104,252,78,202,36,140,70,183,104,131,213,49,95,214,28,159,121,103,105,76,81,1,124,72,47,29,170,21,207,148,200,208,94,66,43,54,10,2,32,22,167,119,246,70,255,94,130,127,112,150,30,255,213,225,167,11,230,129,109,38,54,28,159,159,89,203,125,235,166,84,53,235,162,185,177,84,0,133,222,100,137,243,150,176,121,35,14,83,48,57,127,21,5,60,136,16,94,81,48,246,22,240,171,9,219,238,86,149,158,141,40,77,230,214,245,101,135,37,245,171,78,30,61,144,32,74,117,17,119,212,91,171,207,234,84,152,235,253,135,95,89,158,170,134,250,162,194,253,221,112,52,50,94,14,140,158,129,99,55,110,219,42,158,145,244,140,254,247,54,240,88,110,198,151,71,85,101,101,139,246,223,216,101,89,77,23,190,185,255,161,28,136,22,49,154,188,50,5,78,214,139,72,188,57,83,215,152,165,190,127,117,95,63,144,19,62,4,10,187,72,94,215,37,220,137,164,146,214,187,122,159,9,241,203,23,181,194,53,59,176,67,172,4,14,83,198,179,79,185,79,84,94,1,215,151,91,21,252,174,85,167,226,201,65,191,15,204,159,180,43,158,69,138,181,170,157,53,207,41,176,39,111,153,47,90,157,118,37,128,184,212,145,247,6,188,190,200,194,125,150,166,135,6,85,55,97,233,52,213,29,209,50,154,188,20,224,8,120,24,206,134,2,189,111,25,137,118,126,197,64,198,115,219,39,244,151,123,246,165,77,226,102,189,229,59,182,78,202,31,157,242,146,228,206,114,79,177,174,114,177,114,253,248,4,169,98,202,11,186,42,243,83,117,40,88,163,98,153,39,193,199,32,188,8,112,33,226,167,32,60,186,100,107,199,205,61,181,36,240,246,93,167,80,154,253,240,97,214,178,83,44,34,17,219,45,85,240,81,216,111,241,196,161,99,58,78,33,80,236,107,217,171,88,140,38,194,103,59,43,204,148,101,161,219,178,16,83,125,230,85,151,138,146,151,88,42,74,86,80,169,8,162,130,82,125,101,5,147,118,87,105,146,215,29,73,30,115,171,122,233,81,171,96,229,16,114,221,149,210,154,182,186,154,153,126,82,177,117,26,173,212,227,70,47,71,90,208,36,33,144,60,180,74,26,189,114,78,140,74,166,20,145,95,132,3,33,174,28,53,89,37,29,131,46,47,66,76,112,21,79,124,140,192,142,185,5,255,99,85,153,228,214,165,20,18,87,149,20,156,208,235,196,4,27,72,209,46,86,170,100,136,89,191,78,8,219,64,72,225,44,90,105,48,185,64,172,53,218,6,130,244,10,7,209,92,247,18,254,28,134,85,141,242,103,17,118,194,105,26,26,15,72,163,196,146,70,213,226,178,43,123,62,49,235,37,129,178,122,76,43,28,19,23,251,157,85,154,68,211,219,85,137,166,5,93,10,43,216,106,224,217,6,192,179,85,192,252,221,100,53,244,6,230,139,38,171,160,249,19,215,106,104,127,3,104,127,21,52,190,74,173,198,142,55,192,142,43,176,11,149,174,42,195,75,199,39,176,179,160,24,104,248,203,145,37,208,240,141,33,158,71,183,7,171,91,71,149,103,216,89,53,170,138,115,2,240,231,84,19,93,94,82,180,186,78,5,82,44,61,85,72,94,112,43,86,177,132,57,135,67,60,35,31,75,155,151,93,223,174,70,153,39,138,195,40,111,87,111,98,37,236,214,50,246,10,181,110,129,87,79,101,162,30,95,123,61,201,11,67,189,100,207,75,166,49,101,242,178,83,172,229,120,41,171,63,216,152,118,29,63,151,26,228,123,15,180,139,173,220,114,59,210,121,61,13,126,55,40,47,229,212,46,107,177,185,130,26,27,10,80,220,226,242,75,81,75,93,230,42,76,213,136,27,81,117,143,34,174,96,245,129,161,66,248,140,137,39,172,142,38,168,144,187,138,55,198,176,229,224,134,143,224,4,70,173,230,223,126,216,219,106,158,213,77,211,150,111,143,34,194,214,64,43,180,128,151,19,75,29,57,37,187,240,169,11,177,191,146,58,252,95,247,63,67,254,8,146,46,55,0,0 };
