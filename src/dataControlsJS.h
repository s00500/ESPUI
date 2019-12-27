const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPTDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
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
var rangeSlider=function(){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3215] PROGMEM = { 31,139,8,0,173,39,6,94,2,255,237,90,253,114,211,72,18,255,159,167,16,130,194,210,197,248,3,22,150,149,163,80,198,49,224,91,147,228,18,103,161,142,229,92,99,107,28,171,144,37,173,52,138,147,77,249,53,238,65,238,145,238,73,174,167,103,36,141,100,217,49,100,119,111,175,184,42,42,88,61,61,191,249,77,247,76,207,87,79,3,63,102,218,249,96,60,56,26,140,6,221,225,248,205,249,192,126,210,106,117,166,162,224,228,176,59,234,143,143,95,191,62,235,143,236,118,46,31,140,71,131,81,127,104,43,130,147,238,161,221,46,214,67,81,171,157,235,244,184,228,73,81,169,39,180,158,228,90,175,206,71,163,227,35,251,105,81,79,74,219,173,167,185,230,176,251,10,72,124,87,84,20,194,118,235,187,92,239,236,253,96,212,123,219,63,181,159,21,85,51,121,187,245,76,209,30,14,14,65,246,188,164,43,164,237,214,243,92,243,232,252,221,43,144,125,95,212,148,210,118,235,123,197,92,253,15,35,48,242,201,249,200,126,81,212,86,74,218,173,23,121,141,55,167,221,147,183,246,15,82,208,61,60,20,146,241,201,241,224,136,171,202,130,222,176,223,61,149,202,237,214,15,74,139,221,87,118,187,228,16,33,83,28,114,214,31,246,123,128,86,242,73,42,110,43,94,57,62,25,13,184,253,75,110,73,197,109,197,45,239,6,32,40,121,5,101,109,197,39,239,186,31,236,118,201,31,40,107,171,174,24,245,79,236,118,217,19,40,108,43,126,120,211,61,127,211,183,219,185,31,70,168,40,197,109,197,15,221,94,143,15,142,23,37,77,41,86,16,178,161,125,120,252,254,40,27,216,195,254,235,81,54,128,79,7,111,222,142,178,97,218,235,31,141,192,235,105,15,123,227,209,249,233,223,206,143,7,103,253,12,170,55,238,191,235,159,118,135,249,68,233,141,79,250,80,235,116,240,19,84,125,146,9,223,247,71,221,179,147,183,221,161,2,63,62,59,63,122,61,60,126,95,104,163,215,61,61,61,30,101,195,186,55,238,14,7,127,239,158,130,173,159,103,162,195,238,233,143,217,16,237,141,143,142,143,250,246,147,103,207,58,151,36,210,46,34,18,206,15,9,35,182,79,151,90,55,138,200,181,97,98,201,156,196,221,233,148,122,246,140,120,49,237,204,18,127,202,220,192,215,166,129,23,68,61,143,196,177,129,63,7,142,121,35,127,216,71,201,98,66,163,76,222,137,151,46,155,206,21,61,18,83,213,48,86,68,89,18,249,58,252,249,37,9,220,152,234,29,169,34,237,148,42,208,5,141,136,231,100,197,185,209,82,141,144,50,26,69,238,37,141,50,165,220,136,169,210,146,50,18,135,115,226,177,76,41,51,106,170,19,39,254,204,11,150,10,142,176,113,90,62,37,81,20,228,245,83,123,167,197,196,115,127,37,145,235,103,10,220,220,105,161,67,162,207,122,199,161,51,146,120,44,21,234,157,213,234,30,183,248,146,78,226,96,250,185,163,252,238,5,190,79,167,140,58,101,47,68,244,151,132,198,236,56,114,169,207,8,23,157,208,104,225,198,49,252,50,204,155,213,189,76,51,38,151,244,77,234,101,40,242,130,41,241,206,88,16,145,11,218,136,41,27,48,186,48,116,26,135,137,139,131,33,214,235,127,61,59,62,106,196,12,122,113,225,206,174,141,108,140,152,102,71,1,142,160,253,32,82,176,93,240,48,231,206,91,116,112,76,21,218,186,168,108,43,7,239,184,51,35,171,122,223,246,19,207,51,111,114,44,36,21,146,40,166,185,150,217,17,54,204,155,252,232,58,159,128,164,16,127,252,84,230,75,34,6,38,120,104,56,193,52,89,128,229,204,6,113,28,67,255,139,110,54,130,217,12,70,254,67,67,127,16,5,75,248,158,179,133,103,232,186,217,145,174,104,76,189,0,218,134,65,45,80,84,104,152,90,103,224,133,36,238,195,216,136,160,133,13,238,227,232,153,46,180,17,209,69,112,73,197,100,210,113,150,60,190,136,40,245,117,115,77,21,120,22,244,34,234,84,104,9,210,200,66,107,106,71,129,38,25,112,146,143,30,188,120,254,244,89,167,162,86,222,245,130,212,55,110,166,158,59,253,108,73,203,173,10,125,158,19,223,241,232,79,110,236,78,92,207,101,215,61,16,92,128,125,110,192,141,247,203,253,127,244,232,126,106,242,198,220,117,28,234,155,55,153,63,248,248,207,135,171,244,81,166,14,29,239,95,194,143,161,27,51,234,67,116,209,47,179,54,167,216,166,94,175,230,82,71,163,103,14,196,8,247,158,78,206,224,55,101,134,190,140,173,102,83,223,91,186,190,19,44,27,124,168,114,2,141,121,16,51,159,44,232,158,222,92,198,138,251,3,63,8,169,111,167,68,13,122,201,120,224,243,227,192,163,80,251,2,0,133,166,198,245,118,240,224,38,79,51,122,5,236,50,211,229,20,242,209,196,162,132,118,86,10,53,28,155,187,112,67,69,128,44,143,216,2,26,229,178,45,104,92,80,133,32,150,13,238,10,244,88,9,129,151,58,165,153,12,5,13,7,39,50,47,165,118,230,245,73,224,92,163,108,10,95,52,178,33,74,202,197,132,171,55,216,117,72,229,114,82,220,187,90,88,12,220,88,20,120,113,99,22,68,125,2,149,168,71,57,172,125,128,44,32,252,94,9,134,55,92,221,42,197,59,169,108,174,58,74,103,140,172,18,244,212,236,76,34,74,62,119,82,2,184,23,182,50,242,204,101,30,244,133,51,241,200,132,122,232,226,5,113,253,183,148,56,176,176,200,121,154,43,148,241,112,7,107,113,170,96,38,64,228,161,17,181,197,103,79,116,207,188,17,159,54,135,103,100,162,239,85,232,116,86,20,38,129,170,137,225,13,102,178,16,53,72,8,195,21,98,224,190,227,94,106,174,99,215,92,71,223,187,135,72,174,179,119,79,175,193,152,129,81,107,215,216,50,224,139,127,178,240,99,13,22,65,71,99,194,55,26,168,43,155,2,233,0,248,54,121,237,3,40,213,247,231,207,14,82,80,236,48,151,53,65,184,63,143,154,66,35,14,137,143,205,123,213,173,99,53,13,255,62,94,194,154,81,203,0,47,137,151,80,4,228,24,2,173,9,157,57,208,203,102,21,39,136,63,155,93,97,190,253,142,118,157,36,140,5,194,178,19,230,151,24,112,141,192,95,4,73,76,33,6,250,160,130,218,24,245,141,130,110,93,227,49,199,44,84,73,194,173,21,68,244,173,242,147,168,116,144,121,137,91,15,201,201,234,98,237,97,65,50,157,227,114,96,229,129,196,188,161,141,48,162,124,30,30,138,109,20,68,30,149,132,132,168,35,221,206,170,142,40,224,136,175,193,144,235,199,106,109,190,167,199,198,111,107,40,137,41,200,219,143,55,76,82,17,162,121,107,70,238,114,219,214,219,250,75,29,188,0,235,174,163,91,176,167,202,91,229,61,146,149,93,191,118,176,239,250,97,194,52,30,222,237,26,214,152,4,87,53,209,102,169,201,192,239,113,103,165,141,242,141,65,97,252,225,254,177,182,19,151,102,58,20,177,207,216,205,98,24,201,154,72,71,70,142,88,30,26,252,54,195,74,63,248,239,111,42,138,251,36,115,167,92,131,101,213,68,90,212,115,15,246,137,86,136,57,33,113,196,180,59,63,169,107,213,65,71,9,56,219,180,101,196,193,254,135,179,98,255,15,254,253,207,127,237,55,9,120,26,40,220,74,5,79,246,95,194,166,186,66,129,80,116,23,66,252,222,225,75,248,84,234,23,232,120,119,161,195,239,68,190,132,78,165,126,129,206,228,54,58,77,49,132,242,253,159,109,203,233,246,82,7,158,249,168,155,185,209,162,182,129,183,184,163,169,239,78,92,86,216,74,125,90,162,126,252,35,103,158,6,151,253,38,76,138,98,56,225,243,151,143,207,175,92,238,212,41,240,181,203,93,21,134,178,220,33,67,239,238,12,113,24,222,153,99,1,165,204,50,186,59,75,49,121,239,76,179,8,83,230,57,185,59,79,156,69,119,166,89,64,41,179,156,222,157,165,156,50,119,230,89,194,217,188,25,195,123,249,255,153,117,150,127,60,142,61,215,249,173,214,92,101,11,21,241,107,15,9,46,235,139,13,85,229,166,77,236,178,176,78,77,91,184,16,39,91,240,63,185,178,107,237,22,252,18,27,166,90,121,251,94,171,106,108,60,22,48,202,89,174,82,11,65,118,57,186,169,225,18,33,206,16,193,40,59,95,60,181,124,91,155,44,225,210,152,93,123,124,147,204,171,91,19,143,76,63,119,196,122,228,39,139,74,71,251,120,57,191,197,175,112,252,193,123,179,84,85,124,21,183,213,176,230,53,183,29,177,243,199,172,255,251,68,241,9,191,211,43,209,218,193,13,188,214,87,57,161,251,202,146,70,141,97,243,193,239,30,83,43,137,77,221,60,162,51,187,166,24,29,96,177,175,10,21,61,223,126,137,181,129,163,241,77,61,88,77,69,76,237,94,6,203,184,241,122,220,115,141,34,0,124,77,168,211,19,223,198,141,23,4,161,197,215,138,149,137,55,130,134,78,66,87,180,76,64,123,230,122,224,43,35,91,59,248,245,49,222,252,63,52,216,220,141,129,15,99,112,2,228,29,211,77,27,142,121,15,112,17,121,204,130,199,226,240,166,119,86,184,162,233,40,215,235,234,42,196,71,42,208,177,195,40,88,132,204,208,71,100,162,177,64,147,167,89,248,101,136,9,161,5,17,116,213,124,169,227,91,197,125,222,159,134,80,130,26,6,124,154,230,13,241,104,196,33,230,132,113,76,205,9,104,172,249,1,211,232,149,27,51,205,250,249,103,28,189,21,11,223,250,194,134,143,176,223,216,53,32,245,232,116,203,68,18,229,107,87,1,233,140,17,197,95,51,103,196,51,182,181,193,194,185,15,208,118,41,139,42,67,151,237,25,132,248,174,193,201,139,159,187,198,129,84,34,26,163,92,187,194,122,2,243,0,93,89,236,209,187,193,174,221,225,174,94,87,219,211,53,140,106,98,180,203,110,197,238,175,212,48,75,143,11,208,146,26,58,114,27,224,156,132,157,133,94,184,48,89,173,49,237,126,248,163,152,118,63,108,99,74,174,110,97,202,51,15,254,32,170,188,169,45,92,99,70,195,91,200,98,78,200,183,21,62,102,238,69,18,81,36,128,15,203,165,147,121,170,51,37,114,214,172,161,21,11,133,0,16,139,145,35,123,179,254,40,193,63,217,107,143,225,233,41,170,3,230,129,141,43,22,156,93,94,24,235,117,235,186,164,154,85,81,220,88,74,251,65,111,178,216,126,71,216,188,17,5,9,152,156,191,106,2,30,12,32,254,194,62,114,23,240,171,9,27,248,86,21,207,70,152,196,115,227,230,202,98,113,253,218,202,71,15,172,61,165,60,129,59,242,86,178,146,172,10,115,125,252,244,27,183,151,230,0,125,83,195,253,253,96,56,212,94,245,181,174,134,125,215,110,219,133,94,144,228,130,254,247,206,6,152,99,197,163,103,154,90,100,138,242,223,217,101,89,34,19,190,154,255,169,28,136,22,209,154,60,83,3,206,232,139,80,60,60,83,71,155,37,158,119,125,95,61,218,19,222,5,10,27,84,158,231,36,220,137,162,18,235,3,181,206,132,120,229,91,94,225,154,125,216,124,86,2,7,9,227,139,83,185,78,88,142,128,219,211,143,10,126,87,114,50,241,80,162,222,42,230,207,224,21,111,41,197,12,77,107,203,27,12,108,247,91,250,203,150,213,174,4,16,215,67,242,6,130,231,219,24,184,133,83,120,40,80,117,29,66,167,158,94,54,173,163,201,251,6,142,128,231,236,172,43,80,251,150,158,40,71,99,28,200,120,36,252,130,250,242,56,176,182,255,220,173,182,124,251,86,69,249,75,85,158,136,107,173,215,20,113,149,55,43,227,199,23,180,42,166,188,144,167,105,111,233,46,7,179,76,12,253,220,255,236,7,75,31,3,17,63,96,225,169,40,139,29,171,123,105,72,224,229,7,118,33,33,249,209,163,172,100,191,152,6,34,118,99,105,214,71,97,59,198,23,14,89,80,204,191,202,146,16,53,153,109,168,229,57,133,90,158,57,168,101,9,130,154,72,5,212,178,148,191,28,56,203,245,169,14,17,124,179,166,228,219,44,104,28,19,136,173,74,186,137,154,105,37,134,174,140,184,34,252,138,254,129,217,237,116,44,75,57,250,4,147,115,242,157,100,236,161,127,44,125,15,254,223,211,173,76,57,163,16,99,44,220,172,167,144,41,132,255,42,50,56,45,138,108,138,205,248,59,180,162,220,124,84,181,33,230,206,182,70,216,14,141,20,14,139,149,118,149,211,108,91,67,241,14,13,169,185,5,162,184,238,198,252,101,10,147,229,228,207,34,236,132,203,20,52,190,184,105,37,149,36,172,110,46,187,65,231,19,163,94,106,80,102,81,41,9,84,226,158,221,218,196,36,156,222,78,37,156,22,184,20,226,192,102,224,217,14,192,179,77,192,252,21,99,51,244,14,230,11,39,155,160,249,115,211,102,104,111,7,104,111,19,52,62,17,109,198,142,118,192,142,42,176,11,9,148,233,58,41,29,31,195,250,76,113,160,225,47,91,38,214,194,55,14,241,124,116,187,16,3,173,52,51,194,204,114,88,211,113,78,0,254,146,42,77,151,35,143,146,223,152,130,200,77,89,9,201,245,111,197,42,38,198,230,112,136,167,229,125,105,243,100,222,219,105,148,117,194,40,8,243,242,244,137,170,132,221,90,199,222,64,235,22,248,236,229,10,179,188,149,215,12,91,185,206,196,196,105,33,229,119,165,234,179,137,110,214,241,115,173,64,190,186,64,185,216,6,173,151,163,156,39,176,224,119,131,242,92,200,66,163,80,92,33,141,180,20,80,92,174,242,187,74,35,189,99,77,49,211,66,220,196,165,119,16,242,181,6,223,14,241,52,98,41,192,105,13,159,103,184,202,138,92,146,221,96,212,69,48,86,235,20,22,191,226,101,47,44,233,220,37,33,156,112,168,209,252,199,207,135,123,205,139,186,174,155,226,145,144,255,235,252,7,70,122,210,16,223,52,0,0 };
