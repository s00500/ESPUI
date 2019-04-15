const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPTDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:data.controls.forEach((element)=>{var fauxEvent={data:JSON.stringify(element)};handleEvent(fauxEvent);});break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
"</div>");rangeSlider();break;case UI_NUMBER:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
break;case UI_MIN:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){console.log("MIN"+data.value);parent.attr("min",data.value);}}
break;case UI_MAX:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){console.log("MAX"+data.value);parent.attr("max",data.value);}}
break;case UI_STEP:if(data.parentControl){var parent=$("#id"+data.parentControl+" input");if(parent.size()){console.log("STEP"+data.value);parent.attr("step",data.value);}}
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
"</div>");graphData["graph"+data.id]=[];renderGraphSvg(graphData["graph"+data.id],"graph"+data.id);break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData["graph"+data.id].push({x:ts,y:data.value});renderGraphSvg(graphData["graph"+data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData["graph"+data.id]=[];renderGraphSvg(graphData["graph"+data.id],"graph"+data.id);break;case UI_GAUGE:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
"</div>");break;case UI_ACCEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
parent.append("<div id='id"+
data.id+
"' class='two columns card tcenter "+
colorClass(data.color)+
"'>"+
"<h5>"+
data.label+
"</h5><hr/>"+
"WILL BE A ACCEL<div class='accelerometer' id='accel"+
data.id+
"' ><div class='ball"+
data.id+
"'></div><pre class='accelerometeroutput"+
data.id+
"'></pre>"+
"</div>");break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);break;case UPDATE_SLIDER:slider_move($("#sl"+data.id),data.value,"100",false);break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();console.log("slvalue:"+val+":"+number);websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3048] PROGMEM = { 31,139,8,0,82,134,180,92,2,255,237,90,253,114,219,184,17,255,63,79,193,99,50,17,85,43,250,72,46,185,28,101,58,163,200,74,162,158,98,187,182,124,201,244,114,213,64,36,100,113,66,145,60,18,180,236,243,232,53,250,32,125,164,62,73,23,11,144,4,41,74,86,226,107,218,142,59,147,113,196,197,98,241,195,46,118,177,0,214,14,252,152,105,231,195,201,240,104,56,30,246,70,147,183,231,67,235,105,187,221,181,69,195,201,97,111,60,152,28,191,121,115,54,24,91,157,156,62,156,140,135,227,193,200,82,8,39,189,67,171,83,236,135,164,118,39,231,233,115,202,211,34,83,95,112,61,205,185,94,159,143,199,199,71,214,179,34,159,164,118,218,207,114,206,81,239,53,128,248,190,200,40,136,157,246,247,57,223,217,135,225,184,255,110,112,106,61,47,178,102,244,78,251,185,194,61,26,30,2,237,69,137,87,80,59,237,23,57,231,209,249,251,215,64,251,161,200,41,169,157,246,15,138,186,6,31,199,160,228,147,243,177,245,178,200,173,180,116,218,47,243,30,111,79,123,39,239,172,31,37,161,119,120,40,40,147,147,227,225,17,103,149,13,253,209,160,119,42,153,59,237,31,149,17,123,175,173,78,201,32,130,166,24,228,108,48,26,244,65,90,201,38,41,185,163,88,229,248,100,60,228,250,47,153,37,37,119,20,179,188,31,2,161,100,21,164,117,20,155,188,239,125,180,58,37,123,32,173,163,154,98,60,56,177,58,101,75,32,177,163,216,225,109,239,252,237,192,234,228,118,24,35,163,36,119,20,59,244,250,125,190,56,94,150,56,37,89,145,144,45,237,195,227,15,71,217,194,30,13,222,140,179,5,124,58,124,251,110,156,45,211,254,224,104,12,86,79,103,216,159,140,207,79,255,114,126,60,60,27,100,162,250,147,193,251,193,105,111,148,59,74,127,114,50,128,94,167,195,159,161,235,211,140,248,97,48,238,157,157,188,235,141,20,241,147,179,243,163,55,163,227,15,133,49,250,189,211,211,227,113,182,172,251,147,222,104,248,215,222,41,232,250,69,70,58,236,157,254,148,45,209,254,228,232,248,104,96,61,125,254,188,123,73,34,237,34,34,225,252,144,48,98,249,116,169,245,162,136,92,27,245,238,44,241,109,230,6,190,102,7,94,16,245,61,18,199,6,254,28,58,245,27,249,195,58,74,22,83,26,101,244,110,188,116,153,61,87,248,72,76,85,45,152,17,101,73,228,235,240,231,183,36,112,99,170,119,37,139,84,74,202,64,23,52,34,158,147,53,231,26,74,57,66,202,104,20,185,151,52,202,152,114,141,165,76,75,202,72,28,206,137,199,50,166,76,131,41,79,156,248,51,47,88,42,114,132,66,211,118,155,68,81,144,247,79,149,155,54,19,207,253,157,68,174,159,49,112,221,166,141,14,137,62,235,93,135,206,72,226,177,148,168,119,87,171,7,92,241,75,58,141,3,251,115,87,249,221,15,124,159,218,140,58,214,140,120,49,205,173,16,209,152,145,136,25,245,155,71,134,19,216,201,130,250,172,222,36,142,99,232,127,210,235,205,96,54,3,155,61,50,244,135,81,176,132,239,57,91,120,134,174,215,187,82,110,211,246,130,152,2,139,148,210,93,61,80,12,236,159,49,194,146,120,0,19,141,96,132,13,88,184,244,140,23,198,136,232,34,184,164,98,101,232,104,242,39,23,17,165,190,94,95,99,5,156,5,190,136,58,21,92,2,52,162,208,90,218,81,160,73,4,28,228,227,135,47,95,60,123,222,173,232,149,79,189,64,245,141,27,219,115,237,207,166,212,220,170,48,231,57,241,29,143,254,236,198,238,212,245,92,118,221,7,194,5,232,231,198,157,25,223,149,231,255,248,241,119,169,202,155,115,215,113,168,95,191,201,236,193,141,153,137,77,109,148,177,195,196,7,151,240,99,228,198,140,250,224,42,250,101,54,166,141,99,234,141,106,44,13,84,122,102,64,244,205,15,116,122,6,191,41,51,244,101,108,182,90,250,222,210,245,157,96,217,244,2,155,112,0,205,121,16,51,159,44,232,158,222,90,198,138,249,3,63,8,169,111,165,64,13,122,201,184,23,251,113,224,81,232,125,1,2,5,167,198,249,118,176,224,38,75,51,122,5,232,50,213,229,16,242,213,196,162,132,118,87,10,52,92,155,187,96,67,70,16,89,94,177,5,105,148,211,182,72,227,132,42,9,220,11,133,41,208,98,37,9,188,213,225,49,242,207,103,199,71,205,144,68,224,77,208,208,228,180,58,246,165,86,102,245,105,224,92,35,205,134,47,26,89,224,242,50,50,114,246,38,187,14,169,140,141,197,172,203,196,102,192,198,162,192,139,155,179,32,26,16,232,100,80,143,162,191,91,7,136,3,162,201,149,192,120,195,59,152,8,41,102,16,132,46,220,217,117,198,189,234,42,211,49,178,78,48,215,122,119,26,81,242,185,155,66,192,60,206,204,224,51,151,121,48,27,142,197,35,83,234,161,145,23,196,245,223,81,226,64,156,148,158,154,51,148,229,97,246,101,114,168,160,40,144,216,5,175,66,110,241,217,23,19,172,223,136,79,139,139,103,100,170,239,85,240,116,87,20,220,64,229,196,0,7,190,44,72,77,18,194,130,133,40,184,239,184,151,154,235,88,53,215,209,247,30,160,36,215,217,123,160,215,96,213,192,186,181,106,108,25,240,189,44,89,248,177,6,49,221,209,152,176,142,6,236,202,30,39,77,0,223,117,222,251,0,90,245,253,249,243,131,84,40,78,152,211,90,64,220,159,71,45,193,17,135,196,199,225,189,234,209,177,155,134,127,159,44,97,195,173,101,2,47,137,151,80,20,200,101,8,105,45,152,204,129,94,86,171,200,126,255,219,244,10,30,247,111,212,235,52,97,44,16,154,157,50,191,132,128,115,4,254,34,72,98,10,81,208,7,22,228,198,184,111,20,120,27,26,143,58,245,66,151,36,220,218,65,196,223,42,59,137,78,7,153,149,184,246,16,156,236,46,118,31,22,36,246,28,55,4,51,15,37,245,27,218,12,35,202,253,240,80,100,5,16,123,84,16,82,68,3,225,118,87,13,148,2,134,248,26,25,114,7,89,173,249,123,122,228,185,95,75,73,184,32,31,63,222,224,164,34,72,243,209,140,220,228,150,165,119,244,87,58,88,1,118,94,71,55,33,171,202,71,229,51,146,157,93,191,118,176,239,250,97,194,52,30,224,173,26,246,152,6,87,53,49,102,105,200,192,239,115,99,165,131,242,212,160,176,254,252,196,243,196,122,189,21,75,43,93,138,56,103,156,102,49,140,100,67,164,43,35,151,88,94,26,252,36,110,166,31,252,247,189,138,226,62,201,204,41,119,97,217,53,145,26,245,220,131,125,162,21,98,78,72,28,225,118,231,39,13,173,58,232,40,1,103,27,183,140,56,56,255,112,86,156,255,193,63,255,254,143,253,22,1,75,3,132,91,161,224,169,244,75,208,84,119,40,0,138,238,2,136,159,153,191,4,79,37,127,1,142,119,23,56,252,60,255,37,112,42,249,11,112,166,183,193,105,137,37,148,103,128,150,37,221,237,149,14,56,243,85,55,115,163,69,109,3,110,113,191,208,216,29,184,236,176,21,186,93,130,126,252,19,71,158,6,151,253,22,56,69,49,156,112,255,229,235,243,43,183,59,213,5,190,118,187,171,146,161,108,119,136,208,187,59,66,92,134,119,198,88,144,82,70,25,221,29,165,112,222,59,195,44,138,41,227,156,222,29,39,122,209,157,97,22,164,148,81,218,119,71,41,93,230,206,56,75,114,54,39,99,120,167,252,63,179,207,242,143,39,177,231,58,127,212,158,171,164,80,17,191,248,144,194,101,127,145,80,85,38,109,34,203,194,62,53,109,225,66,156,108,195,255,228,202,170,117,218,240,75,36,76,181,114,250,94,171,26,108,50,17,98,148,179,92,37,23,10,217,229,232,166,134,75,20,113,134,18,140,178,241,197,51,193,253,74,178,132,73,99,118,237,241,36,153,119,55,167,30,177,63,119,197,126,228,39,139,74,67,251,120,215,188,197,174,112,252,193,155,179,148,85,124,21,211,106,216,243,90,219,142,216,249,67,204,255,109,162,216,132,223,234,149,96,237,96,6,222,235,171,140,208,123,109,74,165,198,144,124,240,219,199,84,75,34,169,155,71,116,102,213,20,165,131,88,156,171,2,69,207,211,47,177,55,112,105,60,169,7,173,169,18,83,189,151,133,101,216,120,63,110,185,102,81,0,124,77,169,211,23,223,198,141,23,4,161,201,247,138,85,29,239,4,13,157,132,174,24,153,0,247,204,245,192,86,70,182,119,240,11,100,254,24,160,61,50,216,220,141,1,15,99,112,2,228,19,211,235,22,28,243,30,226,38,242,132,5,79,196,225,77,239,174,112,71,211,145,174,55,212,93,136,175,84,128,99,133,81,176,8,153,161,143,201,84,99,129,38,79,179,240,203,16,14,161,5,17,76,181,254,10,96,241,107,110,62,159,166,96,130,30,6,124,214,235,55,196,163,17,23,49,39,140,203,212,156,128,198,154,31,48,141,94,185,49,211,204,79,159,112,245,86,108,124,235,27,27,62,32,222,179,107,64,234,81,123,139,35,137,246,181,171,128,212,99,68,243,215,248,140,120,130,53,55,104,56,183,1,234,46,69,81,165,232,178,62,131,16,95,54,56,120,241,115,215,56,144,82,196,96,148,115,87,104,79,200,60,64,83,22,103,244,126,184,235,116,184,169,215,217,246,116,13,163,154,88,237,114,90,177,251,59,53,234,165,231,5,24,73,13,29,185,14,208,39,33,179,208,11,23,38,171,53,164,189,143,223,10,105,239,227,54,164,228,234,22,164,252,213,252,27,65,229,67,109,193,26,51,26,222,2,22,235,25,238,87,248,152,185,23,73,68,17,0,190,202,151,78,230,41,143,77,164,215,172,73,43,54,10,2,72,44,70,142,236,193,255,23,93,142,34,7,249,213,250,229,215,46,232,3,50,213,183,188,225,236,242,194,216,204,220,40,17,10,33,169,84,171,130,102,100,177,245,158,176,121,51,10,18,208,53,127,208,4,185,176,114,154,23,148,141,221,5,252,106,65,230,222,222,6,176,25,38,241,220,184,185,50,89,220,184,54,243,229,3,155,207,31,131,91,41,165,49,191,133,158,210,194,149,123,181,206,63,12,71,35,237,245,64,235,105,56,119,237,182,244,243,130,36,23,244,63,119,40,192,194,160,123,106,32,156,187,122,70,39,182,13,27,58,100,154,188,254,70,152,7,73,37,148,7,106,159,41,241,202,215,181,66,213,251,144,69,86,10,14,18,198,119,153,114,159,176,28,202,84,67,41,149,127,120,124,80,239,255,242,7,235,138,87,143,98,29,160,185,229,181,4,18,243,182,254,170,109,118,42,5,136,139,28,121,87,192,107,99,12,76,182,20,28,138,168,134,14,177,78,79,175,133,214,165,201,155,1,46,1,79,196,217,84,160,247,45,51,81,14,177,184,22,241,240,246,5,253,101,226,190,150,41,238,214,91,190,82,171,164,252,77,41,47,247,52,215,123,138,64,200,135,149,14,255,5,163,10,31,21,244,180,222,42,205,71,176,34,196,208,207,253,207,126,176,244,49,114,240,163,16,158,95,178,53,180,122,144,122,53,111,63,176,10,101,175,143,31,103,45,251,197,146,13,145,55,201,146,139,98,226,196,35,189,108,40,214,74,101,213,111,154,44,115,211,242,98,54,45,47,89,211,178,202,52,77,212,160,105,89,173,89,46,56,171,203,169,246,121,158,86,41,181,49,11,26,199,4,130,161,82,24,162,86,69,137,165,43,67,164,136,151,98,126,160,118,43,93,203,146,142,54,193,66,154,60,231,139,61,180,143,169,239,193,255,123,186,153,49,103,16,98,12,110,155,249,20,48,133,120,93,5,6,221,162,136,166,56,140,191,195,40,202,29,69,213,24,194,119,182,13,194,118,24,164,112,172,171,212,171,116,179,109,3,197,59,12,164,86,1,136,230,134,27,243,55,36,44,108,147,63,139,98,167,156,166,72,227,251,147,86,98,73,194,234,225,178,187,110,238,24,141,210,128,178,226,73,41,118,18,55,226,230,38,36,161,125,59,148,208,46,96,41,196,129,205,130,103,59,8,158,109,18,204,223,27,54,139,222,65,125,225,116,147,104,254,48,180,89,180,183,131,104,111,147,104,124,204,217,44,59,218,65,118,84,33,187,80,236,152,238,147,210,240,49,131,108,30,23,26,254,178,44,44,97,224,223,184,196,243,213,237,66,12,52,211,26,134,122,86,111,154,174,115,2,226,47,169,50,116,57,242,40,181,136,169,16,153,87,149,36,185,254,173,178,138,69,172,185,56,148,167,229,115,233,240,194,219,219,97,148,121,194,40,8,243,246,244,49,169,36,187,189,46,123,3,172,91,196,103,111,76,88,94,172,188,59,88,202,197,35,111,18,129,30,111,53,213,7,14,189,222,192,207,181,6,249,62,2,237,34,13,90,111,71,58,47,53,193,239,38,229,117,139,133,65,161,185,130,26,105,169,64,113,13,202,111,21,141,244,54,52,149,153,54,98,18,151,222,22,200,119,21,124,229,195,227,131,169,8,78,123,248,188,26,85,118,228,148,236,174,161,33,130,177,218,167,176,249,21,175,101,97,75,231,38,9,225,72,66,141,214,223,62,29,238,181,46,26,186,94,23,207,121,252,95,247,95,168,42,45,60,69,51,0,0 };
