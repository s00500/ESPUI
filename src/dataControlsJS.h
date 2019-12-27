const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPTDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);websock=new WebSocket("ws://"+window.location.hostname+"/ws");websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element)};handleEvent(fauxEvent);});break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
"</div>");graphData[data.id]=[];renderGraphSvg(graphData[data.id],"graph"+data.id);break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_GAUGE:var parent;if(data.parentControl){parent=$("#tab"+data.parentControl);}else{parent=$("#row");}
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
var rangeSlider=function(){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3113] PROGMEM = { 31,139,8,0,193,22,6,94,2,255,237,90,253,114,219,54,18,255,63,79,193,178,153,136,58,43,250,72,155,52,165,76,103,20,89,73,212,42,182,207,150,155,204,181,61,15,36,66,22,39,20,201,146,160,101,215,163,215,184,7,185,71,186,39,185,197,2,32,65,138,146,149,248,210,235,77,110,38,227,136,139,197,15,63,236,2,139,175,157,134,65,194,140,243,225,197,240,104,56,30,246,70,23,175,207,135,206,147,118,187,59,21,5,39,135,189,241,224,226,248,213,171,179,193,216,233,228,242,225,197,120,56,30,140,28,77,112,210,59,116,58,197,122,40,106,119,114,157,62,151,60,41,42,245,133,214,147,92,235,229,249,120,124,124,228,124,83,212,147,210,78,251,155,92,115,212,123,9,36,190,45,42,10,97,167,253,109,174,119,246,110,56,238,191,25,156,58,79,139,170,153,188,211,126,170,105,143,134,135,32,123,86,210,21,210,78,251,89,174,121,116,254,246,37,200,190,43,106,74,105,167,253,157,102,174,193,251,49,24,249,228,124,236,60,47,106,107,37,157,246,243,188,198,235,211,222,201,27,231,123,41,232,29,30,10,201,197,201,241,240,136,171,202,130,254,104,208,59,149,202,157,246,247,90,139,189,151,78,167,228,16,33,211,28,114,54,24,13,250,128,86,242,137,18,119,52,175,28,159,140,135,220,254,37,183,40,113,71,115,203,219,33,8,74,94,65,89,71,243,201,219,222,123,167,83,242,7,202,58,186,43,198,131,19,167,83,246,4,10,59,154,31,94,247,206,95,15,156,78,238,135,49,42,74,113,71,243,67,175,223,231,131,227,121,73,83,138,53,132,108,104,31,30,191,59,202,6,246,104,240,106,156,13,224,211,225,235,55,227,108,152,246,7,71,99,240,186,234,97,255,98,124,126,250,215,243,227,225,217,32,131,234,95,12,222,14,78,123,163,124,162,244,47,78,6,80,235,116,248,19,84,125,146,9,223,13,198,189,179,147,55,189,145,6,127,113,118,126,244,106,116,252,174,208,70,191,119,122,122,60,206,134,117,255,162,55,26,254,173,119,10,182,126,150,137,14,123,167,63,102,67,180,127,113,116,124,52,112,158,60,125,218,189,34,177,113,25,147,104,126,72,24,113,2,186,52,122,113,76,110,172,58,150,204,73,210,155,78,169,239,204,136,159,208,238,44,13,166,204,11,3,99,26,250,97,220,247,73,146,88,248,115,232,214,111,229,15,231,40,93,76,104,156,201,187,201,210,99,211,185,166,71,18,170,27,198,142,41,75,227,192,132,63,191,165,161,151,80,179,43,85,164,157,148,2,93,208,152,248,110,86,156,27,77,105,68,148,209,56,246,174,104,156,41,229,70,84,74,75,202,72,18,205,137,207,50,165,204,168,74,39,73,131,153,31,46,53,28,97,99,85,62,37,113,28,230,245,149,189,85,49,241,189,223,73,236,5,153,2,55,183,42,116,73,252,193,236,186,116,70,82,159,41,161,217,93,173,30,112,139,47,233,36,9,167,31,186,218,239,126,24,4,116,202,168,91,246,66,76,127,75,105,194,142,99,143,6,140,112,209,9,141,23,94,146,192,47,171,126,187,122,160,105,38,140,196,12,132,15,45,55,156,166,11,168,80,111,18,215,181,204,191,152,245,102,56,155,129,195,31,90,230,215,113,184,132,239,57,91,248,150,105,214,187,146,65,115,234,135,9,5,21,137,210,213,160,97,68,157,65,227,105,50,0,147,196,208,194,6,214,28,61,211,133,54,98,186,8,175,168,24,67,38,14,142,199,151,49,165,129,89,95,83,5,158,5,189,152,186,21,90,130,52,178,48,90,198,81,104,72,6,156,228,163,175,159,63,251,230,105,183,162,86,222,245,130,52,176,110,167,190,55,253,96,75,203,173,10,125,158,147,192,245,233,79,94,226,77,60,223,99,55,125,16,92,130,125,110,189,153,245,85,185,255,143,30,125,165,76,222,156,123,174,75,131,250,109,230,15,238,246,12,86,249,40,83,135,142,15,174,224,199,200,75,24,13,96,82,153,87,89,155,83,108,211,108,84,115,105,160,209,51,7,226,196,126,71,39,103,240,155,50,203,92,38,118,171,101,238,45,189,192,13,151,77,63,156,226,232,105,206,195,132,5,100,65,247,204,214,50,209,220,31,6,97,68,3,71,17,181,232,21,227,243,61,72,66,159,66,237,75,0,20,154,6,215,219,193,131,155,60,205,232,53,176,203,76,151,83,200,71,19,139,83,218,93,105,212,112,108,238,194,13,21,1,178,60,98,11,104,148,203,182,160,113,65,21,130,136,150,220,21,232,177,18,2,47,117,121,128,253,225,236,248,168,25,145,24,102,19,20,52,185,76,68,90,234,100,94,159,132,238,13,202,166,240,69,99,7,130,131,140,161,92,189,201,110,34,42,163,104,113,203,102,99,49,112,99,113,232,39,205,89,24,15,8,84,162,62,229,176,206,1,178,128,168,115,45,24,222,114,117,27,9,37,12,130,213,165,55,187,81,202,245,85,87,235,140,149,85,130,158,214,187,147,152,146,15,93,69,0,183,128,118,70,158,121,204,135,190,112,38,62,153,80,31,93,188,32,94,240,134,18,23,226,169,156,167,185,66,25,15,55,110,54,167,10,102,2,196,46,204,41,212,22,159,125,209,189,250,173,248,116,56,60,35,19,115,175,66,167,187,162,48,9,116,77,12,111,48,147,133,168,73,34,24,174,16,3,247,93,239,202,240,92,167,230,185,230,222,3,68,242,220,189,7,102,13,198,12,140,90,167,198,150,33,95,243,210,69,144,24,16,251,93,131,9,223,24,160,174,173,133,210,1,240,93,231,181,15,160,212,220,159,63,61,80,160,216,97,46,107,129,112,127,30,183,132,70,18,145,0,155,247,171,91,199,106,6,254,125,188,132,181,186,150,1,94,17,63,165,8,200,49,4,90,11,58,115,96,150,205,42,54,206,127,54,187,194,124,251,140,118,157,164,140,133,194,178,19,22,148,24,112,141,48,88,132,105,66,33,6,6,160,130,218,24,245,173,130,110,195,224,49,167,94,168,146,70,91,43,136,232,91,229,39,81,233,32,243,18,183,30,146,147,213,197,218,195,194,116,58,199,229,192,206,3,73,253,150,54,163,152,242,121,120,40,118,15,16,121,116,18,18,162,129,116,187,171,6,162,128,35,62,5,67,174,31,171,181,249,174,78,75,95,214,80,18,83,144,183,159,108,152,164,34,68,243,214,172,220,229,142,99,118,204,23,38,120,1,214,93,215,180,97,79,149,183,202,123,36,43,123,65,237,96,223,11,162,148,25,60,188,59,53,172,49,9,175,107,162,205,82,147,97,208,231,206,82,141,242,141,65,97,252,5,169,239,139,241,122,39,151,150,26,138,216,103,236,102,49,140,100,77,168,145,145,35,150,135,6,63,196,219,234,131,255,254,162,162,120,64,50,119,202,53,88,86,77,165,69,125,239,96,159,24,133,152,19,17,87,76,187,243,147,134,81,29,116,180,128,179,77,91,70,28,236,127,52,43,246,255,224,95,255,248,231,126,139,128,167,129,194,157,84,240,64,251,49,108,170,43,20,8,197,247,33,196,143,219,31,195,167,82,191,64,199,191,15,29,126,21,240,49,116,42,245,11,116,38,119,209,105,137,33,148,239,255,28,71,78,183,23,38,240,204,71,221,204,139,23,181,13,188,197,213,68,99,119,226,178,194,86,234,211,18,245,227,31,57,115,21,92,246,91,48,41,138,225,132,207,95,62,62,63,113,185,211,167,192,167,46,119,85,24,218,114,135,12,253,251,51,196,97,120,111,142,5,148,50,203,248,254,44,197,228,189,55,205,34,76,153,231,228,254,60,113,22,221,155,102,1,165,204,114,122,127,150,114,202,220,155,103,9,103,243,102,12,175,163,255,103,214,89,254,241,56,241,61,247,63,181,230,106,91,168,152,95,123,72,112,89,95,108,168,42,55,109,98,151,133,117,106,198,194,131,56,217,134,255,201,181,83,235,180,225,151,216,48,213,202,219,247,90,85,99,23,23,2,70,59,203,85,106,33,200,46,71,55,61,92,34,196,25,34,88,101,231,139,23,134,47,107,147,37,92,154,176,27,159,111,146,121,117,123,226,147,233,135,174,88,143,130,116,81,233,232,0,239,164,183,248,21,142,63,120,111,166,84,197,87,113,91,13,107,94,107,219,17,59,127,195,249,191,79,52,159,240,59,189,18,173,29,220,192,107,125,146,19,122,47,109,105,212,4,54,31,252,238,81,89,73,108,234,230,49,157,57,53,205,232,0,139,125,213,168,152,249,246,75,172,13,28,141,111,234,193,106,58,162,178,123,25,44,227,198,235,113,207,53,139,0,240,53,161,110,95,124,91,183,126,24,70,54,95,43,86,117,188,17,180,76,18,121,162,101,2,218,51,207,7,95,89,217,218,193,175,143,249,163,129,241,208,98,115,47,1,62,140,193,9,144,119,204,172,59,112,204,251,26,23,145,199,44,124,44,14,111,102,119,133,43,154,137,114,179,161,175,66,124,164,2,29,39,138,195,69,196,44,115,76,38,6,11,13,121,154,133,95,150,152,16,70,24,67,87,235,47,128,22,191,228,230,253,105,10,37,168,97,193,103,189,126,75,124,26,115,136,57,97,28,211,112,67,154,24,65,200,12,122,237,37,204,176,127,249,5,71,111,197,194,183,190,176,225,219,227,23,118,13,72,125,58,221,50,145,68,249,218,85,128,154,49,162,248,83,230,140,120,189,181,55,88,56,247,1,218,78,177,168,50,116,217,158,97,132,239,26,156,188,248,185,107,28,80,18,209,24,229,218,21,214,19,152,7,232,202,98,143,222,14,119,237,14,119,245,186,218,158,105,96,84,19,163,93,118,43,241,126,167,86,189,244,184,0,45,233,161,35,183,1,206,73,216,89,152,133,11,147,213,26,211,222,251,63,138,105,239,253,54,166,228,250,14,166,252,193,253,15,162,202,155,218,194,53,97,52,186,131,44,166,66,124,89,225,99,230,93,166,49,69,2,248,160,95,58,153,43,157,41,145,179,102,13,173,88,40,4,128,88,140,28,89,174,192,207,18,252,87,231,231,95,187,96,7,216,161,190,230,101,103,87,151,214,186,82,195,148,156,212,9,75,15,65,165,180,22,116,27,75,156,183,132,205,155,113,152,130,109,249,243,37,224,193,72,105,94,82,54,246,22,240,171,5,59,245,118,21,161,102,148,38,115,235,246,218,102,73,227,198,206,135,9,44,50,247,227,169,101,217,216,159,195,14,42,135,229,139,26,183,239,134,163,145,241,114,96,244,12,236,187,113,215,118,242,146,164,151,244,191,183,201,199,28,33,30,6,85,106,76,93,148,127,102,151,101,137,56,248,252,253,167,114,32,90,196,104,241,148,11,56,108,47,34,241,130,76,93,99,150,250,254,205,87,250,25,157,240,46,80,216,105,242,60,29,225,78,20,149,88,31,232,117,38,196,47,95,215,10,215,236,195,46,178,18,56,76,25,95,101,202,117,162,114,40,219,158,62,83,240,187,150,83,136,167,11,253,122,48,127,207,174,120,20,41,102,24,218,91,30,83,96,223,222,54,95,180,237,78,37,128,184,231,145,87,9,60,113,198,194,189,152,198,67,131,106,152,16,26,77,117,107,180,142,38,47,14,56,2,30,152,179,174,64,237,59,122,162,157,113,113,32,227,217,238,35,234,203,125,253,218,70,114,183,218,242,17,91,23,229,79,78,121,34,169,189,94,83,196,85,222,172,140,31,31,209,170,152,242,66,174,210,182,212,118,5,211,69,44,243,60,248,16,132,203,0,3,17,63,41,225,241,38,139,29,171,7,42,36,240,242,3,167,144,80,251,232,81,86,178,95,204,231,16,219,42,149,190,81,216,87,241,133,67,22,20,19,169,178,36,58,67,102,203,25,121,78,156,145,103,190,25,89,130,155,33,82,217,140,44,101,45,7,206,146,118,170,67,4,223,117,105,137,51,11,154,36,4,98,171,150,55,162,167,76,137,161,43,35,174,8,191,162,127,96,118,71,141,101,41,71,159,96,150,77,190,37,76,124,244,143,109,238,193,255,123,166,157,41,103,20,18,140,133,155,245,52,50,133,240,95,69,6,167,69,145,77,177,153,96,135,86,180,43,140,170,54,196,220,217,214,8,219,161,145,194,169,175,210,174,114,154,109,107,40,217,161,33,61,73,64,20,55,188,132,63,49,97,214,155,252,89,132,157,112,153,134,198,23,55,163,164,146,70,213,205,101,87,225,124,98,52,74,13,202,116,40,45,19,74,92,152,219,155,152,68,211,187,169,68,211,2,151,66,28,216,12,60,219,1,120,182,9,152,63,71,108,134,222,193,124,209,100,19,52,127,55,218,12,237,239,0,237,111,130,198,183,158,205,216,241,14,216,113,5,118,33,19,82,173,147,210,241,9,172,207,20,7,26,254,114,28,204,112,224,223,56,196,243,209,237,65,12,180,85,138,67,61,75,70,85,227,156,0,252,21,213,154,46,71,30,45,81,81,129,200,77,89,9,201,11,238,196,42,102,184,230,112,136,103,228,125,233,240,172,220,187,105,148,117,162,56,140,242,114,245,214,84,194,110,175,99,111,160,117,7,124,246,4,133,89,202,218,179,132,163,221,75,242,34,17,232,241,210,83,127,255,48,235,13,252,92,43,144,207,39,80,46,182,65,235,229,40,231,153,40,248,221,164,60,169,177,208,40,20,87,72,99,67,1,138,91,82,126,233,104,169,203,82,133,169,10,113,19,167,46,19,228,179,11,62,2,226,105,196,214,128,85,141,128,167,170,202,138,92,146,93,69,52,68,48,214,235,20,22,191,226,173,45,44,233,220,37,17,156,112,168,213,250,251,47,135,123,173,203,134,105,214,197,107,31,255,215,253,55,16,59,182,192,159,51,0,0 };
