const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPTDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element)};handleEvent(fauxEvent);});break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
"</button></div>");$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);}});break;case UI_SWITCHER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
"</div>");$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);}});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);}});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);}});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);}});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);}});break;case UI_SLIDER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();console.log("slvalue:"+val+":"+number);websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3246] PROGMEM = { 31,139,8,0,193,91,7,94,2,255,237,90,239,114,219,54,18,255,158,167,96,216,78,68,158,21,253,73,155,52,149,76,103,20,89,73,116,85,108,159,45,55,157,75,115,26,72,132,44,78,40,146,37,65,203,174,71,175,113,15,114,143,116,79,114,139,5,64,130,20,101,43,113,219,235,77,238,67,28,113,177,248,97,177,187,88,44,128,157,133,65,194,140,243,225,100,120,52,28,15,123,163,201,235,243,161,243,164,213,234,206,68,195,201,97,111,60,152,28,191,122,117,54,24,59,237,156,62,156,140,135,227,193,200,209,8,39,189,67,167,93,236,135,164,86,59,231,233,115,202,147,34,83,95,112,61,201,185,94,158,143,199,199,71,206,55,69,62,73,109,183,190,201,57,71,189,151,32,196,183,69,70,65,108,183,190,205,249,206,222,13,199,253,55,131,83,231,105,145,53,163,183,91,79,53,238,209,240,16,104,207,74,188,130,218,110,61,203,57,143,206,223,190,4,218,119,69,78,73,109,183,190,211,212,53,248,105,12,74,62,57,31,59,207,139,220,90,75,187,245,60,239,241,250,180,119,242,198,249,94,18,122,135,135,130,50,57,57,30,30,113,86,217,208,31,13,122,167,146,185,221,250,94,27,177,247,210,105,151,12,34,104,154,65,206,6,163,65,31,208,74,54,81,228,182,102,149,227,147,241,144,235,191,100,22,69,110,107,102,121,59,4,66,201,42,72,107,107,54,121,219,251,201,105,151,236,129,180,182,110,138,241,224,196,105,151,45,129,196,182,102,135,215,189,243,215,3,167,157,219,97,140,140,146,220,214,236,208,235,247,185,115,60,47,113,74,178,134,144,185,246,225,241,187,163,204,177,71,131,87,227,204,129,79,135,175,223,140,51,55,237,15,142,198,96,117,53,195,254,100,124,126,250,183,243,227,225,217,32,131,234,79,6,111,7,167,189,81,190,80,250,147,147,1,244,58,29,254,8,93,159,100,196,119,131,113,239,236,228,77,111,164,193,79,206,206,143,94,141,142,223,21,198,232,247,78,79,143,199,153,91,247,39,189,209,240,239,189,83,208,245,179,140,116,216,59,253,33,115,209,254,228,232,248,104,224,60,121,250,180,123,73,98,227,34,38,209,226,144,48,226,4,116,101,244,226,152,92,91,54,182,44,72,210,155,205,168,239,204,137,159,80,36,37,190,231,210,184,31,6,204,11,210,48,77,100,211,60,13,102,204,11,3,99,22,250,97,220,247,73,146,88,248,115,232,218,55,242,135,115,148,46,167,52,206,232,221,100,229,177,217,66,227,35,9,213,117,214,137,41,75,227,192,132,63,191,164,161,151,80,179,43,89,164,10,21,3,93,210,152,248,110,214,156,235,83,113,68,148,209,56,246,46,105,156,49,229,250,85,76,43,202,72,18,45,136,207,50,166,76,223,138,39,73,131,185,31,174,52,28,161,126,213,62,35,113,28,230,253,149,41,84,51,241,189,95,73,236,5,25,3,183,132,106,116,73,252,209,236,186,116,78,82,159,41,162,217,93,175,31,112,205,175,232,52,9,103,31,187,218,111,48,67,64,103,140,186,101,43,196,244,151,148,38,236,56,246,104,192,8,39,157,208,120,233,37,9,252,178,236,155,245,131,140,51,33,151,244,181,114,0,104,242,195,25,241,207,88,24,147,11,218,72,40,27,50,186,180,76,154,68,169,135,126,146,152,245,191,158,29,31,53,18,6,179,184,240,230,215,86,230,62,182,221,213,128,99,24,63,140,53,108,15,44,140,30,4,35,186,232,110,133,177,46,42,199,202,193,187,222,220,202,186,62,116,130,212,247,237,155,28,11,133,138,72,156,208,156,203,238,10,29,230,67,190,247,220,15,32,164,32,191,255,80,150,151,196,12,84,240,181,229,134,179,116,9,154,179,27,196,117,45,243,47,166,221,8,231,115,88,20,95,91,230,87,113,184,130,239,5,91,250,150,105,218,93,105,138,198,204,15,97,108,112,106,129,162,67,195,170,59,3,43,164,201,0,124,35,134,17,182,152,143,163,103,188,48,70,76,151,225,37,21,139,201,196,85,242,248,34,166,52,48,237,13,86,144,179,192,23,83,183,130,75,8,141,82,24,77,227,40,52,164,4,92,200,71,95,61,127,246,205,211,110,69,175,124,234,5,106,96,221,204,124,111,246,177,35,53,183,46,204,121,65,2,215,167,63,122,137,55,245,124,143,93,247,129,112,1,250,185,1,51,62,44,207,255,209,163,135,74,229,141,133,231,186,52,176,111,50,123,112,255,207,221,85,218,40,99,135,137,15,46,225,199,200,75,24,13,32,186,152,151,217,152,51,28,211,172,87,203,82,71,165,103,6,196,224,247,142,78,207,224,55,101,150,185,74,58,205,166,185,183,242,2,55,92,53,184,171,114,1,26,139,48,97,1,89,210,61,179,185,74,52,243,135,65,24,209,192,81,130,90,244,146,241,192,23,36,161,79,161,247,5,0,10,78,131,243,237,96,193,109,150,102,244,10,164,203,84,151,139,144,123,19,139,83,218,93,107,162,161,111,238,34,27,50,2,100,217,99,11,104,148,211,110,65,227,132,42,4,177,163,112,83,160,197,74,8,188,213,45,173,100,104,104,184,184,144,121,43,117,50,171,79,67,247,26,105,51,248,162,177,3,81,82,110,38,156,189,193,174,35,42,183,147,98,90,219,1,239,67,142,242,54,6,161,164,188,177,85,242,129,139,35,29,166,199,226,208,79,26,243,48,30,16,24,151,250,148,75,230,28,224,68,32,130,95,137,73,222,112,246,78,41,100,74,102,123,221,213,244,97,101,157,64,89,118,119,26,83,242,177,171,230,128,153,118,39,155,63,243,152,79,133,132,62,153,82,31,189,100,73,188,224,13,37,32,175,90,234,57,67,25,15,243,227,14,23,21,52,13,136,93,165,24,241,217,23,211,179,111,196,167,195,225,25,153,154,123,21,60,221,53,133,117,164,115,98,132,4,77,9,82,131,68,224,241,16,70,247,93,239,210,240,92,167,230,185,230,158,80,163,231,238,61,48,107,224,118,224,248,78,141,173,66,158,63,164,203,32,49,96,31,117,13,38,204,107,0,187,150,87,72,3,192,183,205,123,31,64,171,185,191,120,122,160,64,113,194,156,214,4,226,254,34,110,10,142,36,34,1,14,239,87,143,142,221,12,252,251,120,5,219,78,45,3,188,36,126,74,17,144,99,8,180,38,76,230,192,44,171,85,156,79,254,108,122,133,37,251,59,234,117,154,50,22,10,205,78,89,80,146,128,115,132,193,18,86,14,133,48,26,0,11,114,227,198,97,21,120,235,6,15,91,118,161,75,26,221,218,65,4,240,42,59,137,78,7,153,149,184,246,80,56,217,93,108,95,44,76,103,11,220,81,58,121,44,178,111,104,35,138,41,95,135,135,34,19,131,224,165,11,33,33,234,40,110,119,93,71,20,48,196,231,96,200,45,104,189,177,222,213,161,244,203,114,37,177,4,249,248,201,150,69,42,162,60,31,205,202,77,238,56,102,219,124,97,130,21,96,235,118,205,14,164,101,249,168,124,70,178,179,23,212,14,246,189,32,74,153,193,119,8,167,134,61,166,225,85,77,140,89,26,50,12,250,220,88,106,80,158,91,20,252,15,83,208,218,78,178,52,149,43,226,156,113,154,197,48,146,13,161,60,35,71,44,187,6,191,43,233,168,15,254,251,139,138,226,1,201,204,41,247,96,217,53,149,26,245,189,131,125,98,20,98,78,68,92,177,236,206,79,234,70,117,208,209,2,206,109,220,50,226,224,252,163,121,113,254,7,255,254,231,191,246,155,4,44,13,34,220,41,10,222,27,124,138,52,213,29,10,2,197,247,17,136,223,106,124,138,60,149,252,5,113,252,251,136,195,111,92,62,69,156,74,254,130,56,211,187,196,105,10,23,202,83,72,199,145,203,237,133,9,114,230,94,55,247,226,101,109,139,220,226,6,168,190,187,224,178,195,173,162,207,74,162,31,255,192,37,87,193,101,191,9,139,162,24,78,248,250,229,254,249,153,219,157,190,4,62,119,187,171,194,208,182,59,148,208,191,191,132,232,134,247,150,177,128,82,150,50,190,191,148,98,241,222,91,204,34,76,89,206,233,253,229,196,85,116,111,49,11,40,101,41,103,247,151,82,46,153,123,203,89,194,217,158,140,225,173,255,255,204,62,203,63,30,139,131,235,111,179,231,106,41,84,204,111,78,36,184,236,47,18,170,202,164,77,100,89,216,167,102,44,61,136,147,45,248,159,92,57,181,118,11,126,137,132,169,86,78,223,107,85,131,77,38,2,70,59,203,85,114,33,200,46,71,55,61,92,34,196,25,34,88,15,55,174,6,74,222,32,94,118,190,172,172,75,216,56,97,215,62,207,154,121,247,206,212,39,179,143,93,177,65,5,233,178,210,242,1,94,248,223,98,104,56,15,225,93,156,98,21,95,197,60,27,54,193,230,109,103,238,252,237,236,255,54,209,108,194,239,9,75,98,237,96,6,222,235,179,140,208,123,217,145,74,77,32,27,225,247,153,74,75,34,203,91,196,116,238,212,52,165,3,44,206,85,19,197,204,243,49,177,89,112,52,158,229,131,214,116,68,165,247,50,88,38,27,239,199,45,215,40,2,192,215,148,186,125,241,109,221,248,97,24,117,248,230,177,182,241,150,209,50,73,228,137,145,9,112,207,61,31,108,101,101,155,9,191,146,198,215,132,175,45,182,240,18,144,135,49,56,18,242,137,153,182,3,231,190,175,112,87,121,204,194,199,226,52,103,118,215,184,197,153,72,55,235,250,182,196,61,21,196,113,162,56,92,70,204,50,199,100,106,176,208,144,199,91,248,101,137,5,97,132,49,76,213,126,97,226,251,199,67,62,159,134,96,130,30,22,124,218,246,13,241,105,204,33,22,132,113,76,195,13,105,98,4,33,51,232,149,151,48,163,243,243,207,232,189,21,59,225,230,78,135,111,190,95,216,189,32,245,233,236,150,133,36,218,55,238,6,212,138,17,205,159,179,102,196,171,121,103,139,134,115,27,160,238,148,20,85,138,46,235,51,140,240,173,132,11,47,126,238,26,7,20,69,12,70,57,119,133,246,4,230,1,154,178,56,163,183,195,93,167,195,77,189,201,182,103,26,24,213,132,183,203,105,37,222,175,212,178,149,203,201,85,7,201,132,89,184,35,89,111,200,210,251,233,143,146,133,92,221,33,11,47,86,248,131,132,73,24,141,238,144,6,235,68,190,172,53,62,247,46,210,152,162,0,248,162,92,58,79,43,158,25,145,174,189,129,86,108,20,4,64,44,46,239,236,177,250,189,4,255,224,108,188,130,171,179,79,23,212,3,105,38,54,156,93,94,88,155,125,235,166,20,53,235,162,153,177,84,10,132,214,100,137,243,150,176,69,35,14,83,80,57,127,206,4,60,240,16,254,180,62,246,150,240,171,9,105,119,171,74,206,70,148,38,11,235,230,170,195,146,250,117,39,247,30,216,32,74,5,2,247,148,91,171,84,234,84,168,235,253,135,223,120,60,85,23,244,69,185,251,187,225,104,100,188,28,24,61,3,231,110,220,149,42,94,144,244,130,254,247,18,120,172,187,226,225,81,149,27,217,162,253,119,54,89,86,220,132,207,229,127,42,3,162,70,140,38,47,209,128,147,245,50,18,207,197,212,53,230,169,239,95,63,212,15,228,132,79,129,66,22,201,11,156,132,57,145,84,146,250,64,239,51,37,126,249,110,86,152,102,31,50,196,74,224,48,101,124,247,41,247,137,202,17,240,246,186,163,130,221,181,58,77,60,57,232,119,129,249,227,117,197,11,72,177,106,179,115,203,203,9,228,228,45,243,69,171,211,174,4,16,151,58,242,222,128,23,218,88,152,103,105,114,104,80,117,19,66,167,169,174,136,54,209,228,165,0,71,192,195,112,54,21,232,125,199,76,180,243,43,58,50,158,219,62,161,191,204,217,55,146,196,221,122,203,23,107,157,148,191,47,229,197,185,157,205,158,34,174,242,97,101,252,248,132,81,197,146,23,116,85,239,166,74,72,176,188,196,50,207,131,143,65,184,10,48,16,241,83,16,30,93,178,216,177,126,160,66,2,111,63,112,10,69,202,143,30,101,45,251,197,250,15,145,110,169,90,141,66,190,197,55,14,217,80,44,188,202,170,15,13,89,102,104,228,197,132,70,94,50,104,100,149,129,134,168,1,52,178,90,191,28,56,43,242,169,14,17,60,89,211,10,109,150,52,73,8,196,86,173,72,68,47,177,18,174,43,35,174,8,191,98,126,160,118,71,249,178,164,163,77,176,42,39,47,250,73,124,180,79,199,220,131,255,247,204,78,198,156,137,144,96,44,220,206,167,9,83,8,255,85,194,224,178,40,74,83,28,38,216,97,20,237,122,162,106,12,177,118,110,27,132,237,48,72,225,68,87,169,87,185,204,110,27,40,217,97,32,189,34,64,52,215,189,132,191,39,97,149,156,252,89,132,157,114,154,134,198,55,55,163,196,146,70,213,195,101,247,222,124,97,212,75,3,202,242,41,173,114,74,220,142,119,182,73,18,205,238,22,37,154,21,100,41,196,129,237,192,243,29,128,231,219,128,249,219,195,118,232,29,212,23,77,183,65,243,71,162,237,208,254,14,208,254,54,104,124,216,217,142,29,239,128,29,87,96,23,42,39,213,62,41,13,159,192,254,76,209,209,240,151,35,43,106,225,27,93,60,247,110,15,98,96,71,213,51,216,89,241,170,242,115,2,240,151,84,27,186,28,121,180,194,70,5,34,147,178,18,146,23,220,137,85,172,136,205,225,16,207,200,231,210,230,85,188,119,139,81,230,137,226,48,202,219,213,195,82,9,187,181,137,189,69,172,59,224,179,247,38,44,239,214,222,32,242,202,72,47,57,244,146,89,76,153,188,50,20,33,31,175,54,245,103,15,211,174,227,231,70,131,124,53,129,118,145,16,109,182,35,157,23,160,224,119,131,242,90,70,237,202,19,155,43,168,177,161,0,197,93,40,191,90,180,212,149,168,194,84,141,152,206,169,219,8,113,145,169,79,12,5,194,183,64,60,167,116,180,129,10,91,92,241,222,21,54,110,174,248,8,206,49,212,106,254,227,231,195,189,230,69,221,52,109,241,128,39,28,236,22,100,5,22,240,114,90,41,34,167,100,183,38,117,49,234,111,35,13,255,215,253,15,242,7,65,125,104,53,0,0 };
