const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UI_RELOAD=201;const UPDATE_OFFSET=100;const UI_EXTEND_GUI=210;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPDATE_ACCEL=118;const UI_SEPARATOR=19;const UPDATE_SEPARATOR=119;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart,});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){console.log(evt);var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:$("#row").html("");$("#tabsnav").html("");$("#tabscontent").html("");if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>(data.controls.length-1)){websock.send("uiok:"+(data.controls.length-1));}
break;case UI_EXTEND_GUI:data.controls.forEach(element=>{var fauxEvent={data:JSON.stringify(element),};handleEvent(fauxEvent);});if(data.totalcontrols>data.startindex+(data.controls.length-1)){websock.send("uiok:"+(data.startindex+(data.controls.length-1)));}
break;case UI_RELOAD:window.location.reload();break;case UI_TITEL:document.title=data.label;$("#mainHeader").html(data.label);break;case UI_LABEL:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GAUGE:case UI_SEPARATOR:if(data.visible)addToHTML(data);break;case UI_BUTTON:if(data.visible){addToHTML(data);$("#btn"+data.id).on({touchstart:function(e){e.preventDefault();buttonclick(data.id,true);},touchend:function(e){e.preventDefault();buttonclick(data.id,false);},});}
break;case UI_SWITCHER:if(data.visible){addToHTML(data);switcher(data.id,data.value);}
break;case UI_CPAD:case UI_PAD:if(data.visible){addToHTML(data);$("#pf"+data.id).on({touchstart:function(e){e.preventDefault();padclick(UP,data.id,true);},touchend:function(e){e.preventDefault();padclick(UP,data.id,false);},});$("#pl"+data.id).on({touchstart:function(e){e.preventDefault();padclick(LEFT,data.id,true);},touchend:function(e){e.preventDefault();padclick(LEFT,data.id,false);},});$("#pr"+data.id).on({touchstart:function(e){e.preventDefault();padclick(RIGHT,data.id,true);},touchend:function(e){e.preventDefault();padclick(RIGHT,data.id,false);},});$("#pb"+data.id).on({touchstart:function(e){e.preventDefault();padclick(DOWN,data.id,true);},touchend:function(e){e.preventDefault();padclick(DOWN,data.id,false);},});$("#pc"+data.id).on({touchstart:function(e){e.preventDefault();padclick(CENTER,data.id,true);},touchend:function(e){e.preventDefault();padclick(CENTER,data.id,false);},});}
break;case UI_SLIDER:if(data.visible){addToHTML(data);rangeSlider(!sliderContinuous);}
break;case UI_TAB:if(data.visible){$("#tabsnav").append("<li><a onmouseup='tabclick("+data.id+")' href='#tab"+data.id+"'>"+data.value+"</a></li>");$("#tabscontent").append("<div id='tab"+data.id+"'></div>");tabs=$(".tabscontent").tabbedContent({loop:true}).data("api");$("a").filter(function(){return $(this).attr("href")==="#click-to-switch";}).on("click",function(e){var tab=prompt("Tab to switch to (number or id)?");if(!tabs.switchTab(tab)){alert("That tab does not exist :\\");}
e.preventDefault();});}
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
break;case UI_GRAPH:if(data.visible){addToHTML(data);graphData[data.id]=restoreGraphData(data.id);renderGraphSvg(graphData[data.id],"graph"+data.id);}
break;case ADD_GRAPH_POINT:var ts=Math.round(new Date().getTime()/1000);graphData[data.id].push({x:ts,y:data.value});saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case CLEAR_GRAPH:graphData[data.id]=[];saveGraphData();renderGraphSvg(graphData[data.id],"graph"+data.id);break;case UI_ACCEL:if(hasAccel)break;hasAccel=true;if(data.visible){addToHTML(data);requestOrientationPermission();}
break;case UPDATE_LABEL:$("#l"+data.id).html(data.value);if(data.hasOwnProperty('elementStyle')){$("#l"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SWITCHER:switcher(data.id,data.value=="0"?0:1);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SLIDER:slider_move($("#id"+data.id),data.value,"100",false);if(data.hasOwnProperty('elementStyle')){$("#sl"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_NUMBER:$("#num"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#num"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_TEXT_INPUT:$("#text"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#text"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_SELECT:$("#select"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#select"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_BUTTON:case UPDATE_PAD:case UPDATE_CPAD:break;case UPDATE_GAUGE:$("#gauge"+data.id).val(data.value);if(data.hasOwnProperty('elementStyle')){$("#gauge"+data.id).attr("style",data.elementStyle);}
break;case UPDATE_ACCEL:break;default:console.error("Unknown type or event");break;}
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.hasOwnProperty('panelStyle')){$("#id"+data.id).attr("style",data.panelStyle);}
if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}
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
$(this).attr("callbackSet","true");}});};var addToHTML=function(data){panelStyle=data.hasOwnProperty('panelStyle')?" style='"+data.panelStyle+"' ":"";elementStyle=data.hasOwnProperty('elementStyle')?" style='"+data.elementStyle+"' ":"";panelwide=data.hasOwnProperty('wide')?"wide":"";if(!data.hasOwnProperty('parentControl')||$("#tab"+data.parentControl).length>0){var parent=data.hasOwnProperty('parentControl')?$("#tab"+data.parentControl):$("#row");var html="";switch(data.type){case UI_LABEL:case UI_BUTTON:case UI_SWITCHER:case UI_CPAD:case UI_PAD:case UI_SLIDER:case UI_NUMBER:case UI_TEXT_INPUT:case UI_SELECT:case UI_GRAPH:case UI_GAUGE:case UI_ACCEL:html="<div id='id"+data.id+"' "+panelStyle+" class='two columns "+panelwide+" card tcenter "+
colorClass(data.color)+"'><h5>"+data.label+"</h5><hr/>"+
elementHTML(data.type,data.id,data.value,elementStyle)+
"</div>";break;case UI_SEPARATOR:html="<div id='id"+data.id+"' "+panelStyle+" class='sectionbreak columns'>"+
"<h5>"+data.label+"</h5><hr/></div>";break;}
parent.append(html);}else{var parent=$("#id"+data.parentControl);parent.append(elementHTML(data.type,data.id,data.value,elementStyle));}}
var elementHTML=function(type,id,value,elementStyle){switch(type){case UI_LABEL:return"<span id='l"+id+"' "+elementStyle+
" class='label label-wrap'>"+value+"</span>";case UI_BUTTON:return"<button id='btn"+id+"' "+elementStyle+
" onmousedown='buttonclick("+id+", true)'"+
" onmouseup='buttonclick("+id+", false)'>"+
value+"</button>";case UI_SWITCHER:return"<label id='sl"+id+"' "+elementStyle+
" class='switch "+(value=="1"?"checked":"")+"'>"+
"<div class='in'>"+
"<input type='checkbox' id='s"+id+"' onClick='switcher("+id+",null)' "+
(value=="1"?"checked":"")+"/></div></label>";case UI_CPAD:case UI_PAD:return"<nav class='control'><ul>"+
"<li><a onmousedown='padclick(UP, "+id+", true)' "+
"onmouseup='padclick(UP, "+id+", false)' id='pf"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(RIGHT, "+id+", true)' "+
"onmouseup='padclick(RIGHT, "+id+", false)' id='pr"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(LEFT, "+id+", true)' "+
"onmouseup='padclick(LEFT, "+id+", false)' id='pl"+id+"'>&#9650;</a></li>"+
"<li><a onmousedown='padclick(DOWN, "+id+", true)' "+
"onmouseup='padclick(DOWN, "+id+", false)' id='pb"+id+"'>&#9650;</a></li>"+
"</ul>"+
(type==UI_CPAD?"<a class='confirm' onmousedown='padclick(CENTER,"+id+", true)' "+
"onmouseup='padclick(CENTER, "+id+", false)' id='pc"+id+"'>OK</a>":"")+
"</nav>";case UI_SLIDER:return"<div class='range-slider'>"+
"<input id='sl"+id+"' type='range' min='0' max='100' value='"+value+"' "+
elementStyle+" class='range-slider__range'><span class='range-slider__value'>"+
value+"</span></div>";case UI_NUMBER:return"<input style='color:black;' "+elementStyle+" id='num"+id+
"' type='number' value='"+value+"' onchange='numberchange("+id+")' />";case UI_TEXT_INPUT:return"<input style='color:black;' "+elementStyle+" id='text"+id+
"' value='"+value+"' onchange='textchange("+id+")' />";case UI_SELECT:return"<select style='color:black;' "+elementStyle+" id='select"+id+
"' onchange='selectchange("+id+")' />";case UI_GRAPH:return"<figure id='graph"+id+"'><figcaption>"+label+"</figcaption></figure>";case UI_GAUGE:return"WILL BE A GAUGE <input style='color:black;' id='gauge"+id+
"' type='number' value='"+value+"' onchange='numberchange("+id+")' />";case UI_ACCEL:return"ACCEL // Not implemented fully!<div class='accelerometer' id='accel"+id+
"' ><div class='ball"+id+"'></div><pre class='accelerometeroutput"+id+"'></pre>";default:return"";}}
)=====";

const uint8_t JS_CONTROLS_GZIP[3810] PROGMEM = { 31,139,8,0,242,53,223,97,2,255,197,91,91,119,218,72,18,126,247,175,144,149,57,65,44,152,75,110,147,1,203,57,4,147,132,29,98,123,109,60,153,179,153,172,143,16,141,209,177,144,52,146,48,246,56,252,247,173,174,190,168,91,8,204,224,217,217,151,24,149,170,171,191,174,170,238,174,139,226,134,65,146,26,151,253,171,254,73,127,216,239,12,174,62,94,246,237,23,141,70,219,21,47,206,123,131,211,206,49,208,154,130,118,118,220,25,246,174,78,63,124,184,232,13,237,166,202,219,251,117,216,59,57,102,50,154,10,125,216,31,246,6,182,66,56,3,137,57,121,72,202,38,233,95,117,41,229,133,206,212,101,92,47,50,174,247,151,195,225,233,137,253,82,231,227,212,102,227,101,198,57,232,188,7,16,175,116,70,70,108,54,94,101,124,23,95,250,195,238,167,222,185,253,90,103,149,244,102,227,181,194,61,232,31,3,237,77,142,151,81,155,141,55,25,231,201,229,231,247,64,251,81,231,228,212,102,227,71,69,93,160,71,48,200,217,229,208,126,171,115,43,111,154,141,183,217,136,143,231,157,179,79,246,79,156,208,57,62,102,148,171,179,211,254,9,101,229,47,186,131,94,231,156,51,55,27,63,41,51,118,222,219,205,156,65,24,77,49,200,69,111,208,235,130,180,156,77,4,185,169,88,229,244,108,216,167,250,207,153,69,144,155,138,89,62,247,129,144,179,10,210,154,138,77,62,119,126,181,155,57,123,32,173,169,154,98,216,59,179,155,121,75,32,177,169,216,225,99,231,242,99,207,110,230,236,192,169,77,197,12,157,110,151,250,70,206,4,156,218,124,171,42,230,172,115,222,25,158,130,25,127,202,235,70,190,81,94,201,157,112,124,250,229,68,238,131,65,239,195,80,250,251,121,255,227,167,161,244,234,110,239,100,8,78,34,20,210,189,26,94,158,255,235,242,180,127,209,147,162,186,87,189,207,189,243,206,32,219,87,221,171,179,30,140,58,239,255,2,67,95,72,226,151,222,176,115,113,246,169,51,80,196,95,93,92,158,124,24,156,126,209,230,232,118,206,207,79,135,114,23,116,175,58,131,254,191,59,231,96,154,55,146,116,220,57,255,89,122,116,247,234,228,244,164,103,191,120,253,186,125,235,196,198,117,236,68,211,99,39,117,236,128,44,140,78,28,59,247,86,25,223,76,157,164,227,186,196,183,39,142,159,16,36,37,190,55,38,113,55,12,82,47,152,135,243,132,191,154,204,3,55,245,194,192,112,67,63,140,187,190,147,36,22,254,236,143,203,15,252,135,125,50,159,141,72,44,233,237,100,225,165,238,84,225,115,18,162,234,172,21,147,116,30,7,38,252,243,251,60,244,18,98,182,57,11,87,161,96,32,51,18,59,254,88,190,206,244,41,56,34,146,146,56,246,110,73,44,153,50,253,10,166,5,73,157,36,154,58,126,42,153,164,190,5,79,50,15,38,126,184,80,228,48,245,139,247,174,19,199,97,54,94,152,66,188,118,124,239,15,39,246,2,201,64,45,33,94,142,157,248,198,108,143,201,196,153,251,169,32,154,237,229,114,143,106,126,65,70,73,232,222,180,149,223,96,134,128,184,41,25,231,173,16,147,223,231,36,73,79,99,143,4,169,67,73,103,36,158,121,73,2,191,172,242,195,114,79,114,38,206,45,249,40,28,0,94,249,161,235,248,23,105,24,59,215,164,150,144,180,159,146,153,101,146,36,154,123,232,39,137,89,253,231,197,233,73,45,73,97,21,215,222,228,222,146,238,83,46,183,21,193,49,204,31,198,138,108,15,44,140,30,4,51,142,209,221,180,185,174,11,231,202,132,183,189,137,37,135,238,219,193,220,247,203,15,153,44,4,21,57,113,66,50,174,114,155,233,48,155,242,171,55,254,6,32,25,249,235,183,60,94,39,78,65,5,63,88,227,208,157,207,64,115,229,154,51,30,91,230,63,204,114,45,156,76,96,83,252,96,153,207,226,112,1,207,211,116,230,91,166,89,110,115,83,212,92,63,132,185,193,169,153,20,85,52,236,186,11,176,194,60,233,129,111,196,48,195,26,243,81,233,146,23,230,136,201,44,188,37,108,51,153,184,75,14,174,99,66,2,179,188,194,10,56,53,190,152,140,11,184,24,104,68,97,212,141,147,208,224,8,40,200,231,207,222,190,121,249,186,93,48,42,91,186,70,13,172,7,215,247,220,155,22,215,92,117,169,45,122,234,4,99,159,252,226,37,222,200,243,189,244,190,11,132,107,80,208,3,216,113,63,175,128,231,207,247,133,206,107,83,111,60,38,65,249,65,26,132,110,128,204,95,185,145,36,59,172,188,119,11,63,6,94,146,146,0,142,23,243,86,206,233,226,156,102,181,24,75,21,181,142,142,181,240,130,113,184,168,81,135,164,179,212,162,48,78,247,109,211,252,254,189,248,205,219,198,186,55,175,94,189,148,246,197,243,244,11,25,93,192,111,146,90,230,34,105,213,235,102,37,63,112,26,38,105,224,204,72,197,108,173,190,164,82,43,102,125,1,42,111,47,9,224,221,89,56,151,177,39,28,54,12,194,136,4,182,208,172,69,110,83,122,84,7,73,232,19,24,127,13,34,25,167,65,249,182,240,185,117,190,153,146,59,192,39,109,157,237,153,204,255,211,120,78,218,203,118,6,13,119,211,54,216,144,17,68,230,247,152,38,141,80,218,6,105,148,80,36,129,221,129,212,119,208,197,30,147,64,217,199,185,195,8,94,212,198,120,22,209,183,196,150,126,59,10,199,247,72,115,225,137,196,224,108,226,62,164,236,181,244,62,34,252,70,212,131,254,86,193,25,68,73,169,51,74,2,231,182,136,12,64,97,107,164,234,43,240,121,156,38,127,157,195,145,154,191,224,11,249,192,143,144,78,69,199,161,159,212,38,97,220,115,0,60,241,9,93,158,125,132,103,61,220,100,119,76,117,15,148,189,149,187,58,56,115,185,186,108,43,106,182,228,40,176,65,6,53,13,83,199,23,243,29,89,250,244,62,9,174,211,233,65,179,44,55,31,92,94,1,156,221,115,47,188,129,109,181,150,29,22,50,138,137,115,211,22,170,206,82,163,214,255,119,133,76,239,244,184,131,45,77,238,42,59,173,120,155,241,171,58,96,169,100,43,127,150,196,196,15,157,49,236,12,157,27,147,198,150,116,236,212,75,125,194,188,198,119,70,196,71,55,156,57,94,240,137,56,224,67,194,11,51,134,188,60,76,245,90,226,137,229,93,242,49,75,172,36,137,229,53,242,17,19,4,229,37,15,236,91,66,203,120,61,248,164,12,199,215,48,252,52,252,60,176,216,254,212,65,176,196,116,101,208,67,126,20,93,220,40,13,204,10,178,65,140,131,23,99,26,206,221,41,234,190,149,157,25,229,7,82,139,98,66,205,126,204,98,60,170,202,121,154,210,227,14,46,82,139,139,168,210,211,16,140,82,69,41,96,211,93,100,240,187,109,201,110,101,125,109,34,65,126,124,117,236,68,130,75,85,136,101,236,142,143,0,115,98,105,206,47,245,78,127,111,165,188,104,178,179,238,34,103,204,22,125,121,86,221,85,119,69,50,84,221,33,68,255,233,16,105,210,248,116,144,154,148,21,152,241,211,97,98,58,251,116,156,186,152,21,160,163,167,3,165,25,249,211,113,106,82,86,96,186,79,135,201,10,2,79,7,154,147,179,113,119,99,73,235,241,205,23,211,24,248,2,239,117,107,127,37,14,88,17,59,236,188,95,149,169,7,30,78,20,225,13,116,232,123,71,135,142,17,6,51,144,68,230,145,93,2,30,182,14,169,209,138,89,46,25,211,152,76,236,18,149,160,208,75,71,252,1,143,153,138,121,88,119,142,14,235,32,178,48,162,145,115,142,189,91,195,27,227,84,154,176,195,58,188,161,99,233,56,27,4,212,116,1,240,52,34,227,46,123,182,32,255,13,163,22,53,211,178,140,113,155,101,58,145,199,102,118,128,123,226,249,16,172,89,210,108,52,77,193,20,243,7,43,157,122,9,224,73,83,72,65,232,194,204,178,109,219,16,9,211,117,31,164,225,1,59,75,33,157,71,103,50,145,110,86,85,7,160,241,4,192,177,163,56,156,69,16,50,15,157,145,145,134,6,27,72,127,89,1,86,80,12,72,223,192,39,223,177,56,110,159,174,167,198,152,96,132,5,143,16,19,56,62,137,169,136,169,147,82,153,198,56,36,137,17,132,169,65,238,32,81,50,90,191,253,134,201,64,129,207,21,184,20,43,7,74,243,67,100,11,35,186,44,146,96,176,25,137,170,247,89,2,49,143,155,114,27,232,172,109,246,152,25,45,140,48,169,163,118,99,63,205,202,158,176,221,158,89,50,208,5,236,146,160,50,143,160,47,4,133,77,70,40,247,145,160,97,80,1,132,195,58,147,121,100,174,174,232,115,127,219,229,120,227,162,165,84,76,195,11,162,121,202,76,192,151,149,120,127,64,130,91,126,16,171,68,87,152,121,129,169,95,155,43,88,58,191,254,93,88,156,187,71,176,208,42,236,223,4,6,210,245,232,17,52,88,0,127,252,36,147,69,162,175,220,119,190,217,43,213,39,113,144,183,1,1,156,116,248,226,226,246,218,90,29,91,53,145,150,157,253,186,243,228,106,246,45,220,180,137,253,217,73,167,181,56,156,131,91,211,196,28,36,194,138,105,81,107,232,205,224,87,189,217,104,52,138,144,214,162,121,50,181,30,238,90,105,82,189,111,101,218,128,93,152,43,205,237,130,92,193,173,180,20,90,5,10,251,250,237,47,158,79,212,229,169,249,68,29,185,204,222,203,178,50,166,253,143,95,84,27,139,153,185,173,173,116,139,48,83,86,67,183,44,239,224,14,39,166,6,64,167,139,224,44,14,35,56,52,239,173,18,207,218,46,210,123,159,148,202,236,154,83,5,9,7,134,215,220,131,213,17,197,144,100,208,189,33,166,134,251,162,97,190,107,180,154,127,14,90,242,100,108,44,100,96,65,192,21,45,58,90,218,14,7,185,10,202,170,9,206,108,42,213,179,191,15,40,79,6,169,44,184,10,21,97,0,108,103,203,234,146,118,129,165,36,165,24,161,144,187,244,47,194,150,19,181,147,113,89,122,188,114,53,63,21,218,138,176,93,192,241,52,91,37,101,89,100,214,74,110,173,142,100,105,62,5,114,237,204,175,201,95,180,168,188,172,93,214,196,142,61,70,23,205,28,81,43,196,74,164,101,94,6,55,65,184,8,12,90,229,163,209,28,134,96,166,56,63,151,123,178,34,4,239,143,108,173,145,255,252,185,124,115,168,87,6,217,13,45,202,83,249,13,188,86,13,145,19,16,95,83,130,58,172,64,3,217,128,114,14,170,109,107,7,10,164,52,12,140,222,201,96,199,204,129,236,234,241,158,226,1,239,226,137,199,172,101,39,40,89,127,78,80,100,55,78,16,88,251,77,60,201,110,91,185,45,144,100,69,107,206,99,86,148,150,37,47,145,193,115,89,148,219,11,151,144,97,23,160,21,180,42,76,137,143,3,219,132,104,45,142,229,30,77,91,48,95,59,224,103,244,21,62,65,66,66,104,73,82,73,71,120,30,242,181,241,141,95,41,122,98,130,52,76,103,24,53,160,181,121,126,51,22,114,178,124,64,169,165,207,72,146,56,215,196,86,202,153,106,219,135,225,99,141,23,158,174,48,191,4,121,182,184,3,56,29,119,106,214,65,99,229,203,196,199,153,91,102,5,254,98,75,132,115,171,179,48,210,134,89,240,76,223,52,77,176,197,44,244,240,221,48,7,59,155,55,77,146,110,51,137,72,141,11,167,160,201,236,198,25,156,209,22,115,176,163,122,147,81,248,89,190,209,48,91,76,164,86,34,217,235,170,151,140,225,164,195,182,31,255,169,139,29,81,154,34,141,110,58,35,199,50,143,138,167,147,229,17,122,248,84,115,19,242,110,138,210,72,97,69,148,214,58,36,145,251,56,148,200,213,176,104,135,255,122,193,147,45,4,79,214,9,166,53,170,245,162,183,80,95,52,90,39,154,150,19,215,139,246,183,16,237,175,19,141,21,192,245,178,227,45,100,199,5,178,181,86,176,8,164,185,225,19,72,14,8,58,26,254,178,249,55,2,240,140,46,158,121,183,7,39,120,11,70,186,55,180,27,153,239,159,36,14,136,191,37,202,212,249,99,75,105,124,10,33,122,111,86,72,242,130,71,101,233,61,254,76,28,202,51,178,181,52,179,8,122,19,140,60,79,4,215,124,246,94,212,31,115,178,27,171,178,215,192,122,68,188,40,75,178,15,86,148,2,99,214,57,245,146,99,47,113,99,146,242,122,23,50,217,107,239,55,229,155,35,119,100,43,247,156,118,207,232,55,23,132,47,116,1,145,239,184,196,170,255,231,183,227,74,253,186,106,226,85,198,138,158,88,204,197,42,69,107,245,230,212,174,68,74,225,97,228,114,201,75,166,235,174,220,117,3,219,232,128,42,64,215,241,253,145,227,222,92,16,136,249,246,109,147,90,197,100,31,70,168,234,17,131,50,180,66,17,75,225,111,42,11,211,133,198,179,183,126,222,42,159,181,189,92,138,78,183,204,187,51,61,99,2,254,144,5,124,246,163,17,228,59,211,192,88,145,22,237,114,209,98,133,214,237,90,166,217,86,67,104,123,139,208,124,69,166,250,90,74,197,137,22,176,248,98,145,244,13,21,69,255,34,63,85,247,154,229,40,245,172,82,249,251,119,121,19,23,84,197,120,31,245,168,161,213,199,182,145,251,110,147,216,172,203,207,62,66,0,159,218,252,133,128,222,52,213,210,42,165,225,183,182,85,151,107,30,236,222,124,197,250,82,113,43,150,229,69,108,41,178,92,175,228,26,104,201,138,234,47,134,75,15,32,187,148,46,66,250,97,225,124,22,36,130,131,154,145,50,56,241,216,72,217,39,20,180,40,92,28,67,99,31,96,250,90,52,22,88,129,216,60,172,3,233,112,26,215,105,229,152,187,148,44,59,161,122,171,171,37,154,170,150,255,97,149,25,251,11,185,210,87,214,118,222,101,189,9,251,38,11,69,138,133,211,182,8,204,182,97,21,58,146,229,158,94,108,167,56,196,177,177,85,37,55,95,173,223,77,67,242,54,80,134,103,231,11,138,128,209,5,3,11,226,55,233,230,252,211,200,195,4,148,135,90,133,43,73,232,83,59,27,246,164,74,81,91,6,254,123,176,136,157,136,170,83,54,152,168,156,35,51,223,251,23,179,176,160,22,231,193,78,255,186,153,120,191,139,70,54,192,169,68,194,108,72,213,192,219,151,54,51,76,181,55,86,196,201,46,82,52,185,4,201,248,20,152,114,87,11,160,108,141,20,103,242,184,66,120,87,201,172,88,162,244,216,52,223,201,171,28,14,200,50,107,197,237,49,223,229,163,188,128,211,240,66,194,250,133,93,194,65,163,240,174,196,230,22,83,135,65,151,46,74,76,69,63,205,99,203,195,184,12,123,56,27,230,22,254,124,88,199,101,41,235,94,57,188,196,250,3,71,226,228,159,185,192,182,159,251,12,176,214,147,100,54,82,123,255,134,110,36,10,206,84,140,84,200,202,173,132,171,166,159,49,176,134,227,243,103,63,189,121,221,104,103,109,203,199,102,103,205,242,109,1,228,184,53,12,241,206,24,240,195,130,109,33,232,204,26,2,127,103,4,216,138,223,22,129,206,172,33,24,109,68,80,103,238,96,241,106,21,115,166,119,38,160,202,28,103,226,197,179,210,26,148,188,15,191,37,76,206,93,12,212,21,64,79,127,166,24,153,219,83,136,224,198,234,46,103,55,178,240,113,101,47,170,1,179,182,43,245,19,128,237,81,100,46,25,51,15,86,211,128,191,206,157,93,106,54,26,74,139,149,31,52,184,26,61,192,42,154,144,71,232,176,193,240,16,46,228,64,137,250,33,134,39,173,184,168,114,81,134,88,34,91,4,143,248,240,2,111,141,32,150,191,105,175,156,102,38,46,21,107,61,188,97,204,22,203,210,147,162,181,193,65,139,97,178,224,225,9,4,211,21,88,166,174,192,82,162,157,93,161,177,18,81,190,153,93,128,70,41,53,21,98,225,97,150,188,248,176,94,243,39,128,136,2,15,135,146,77,172,149,134,10,167,102,33,157,152,121,226,93,207,99,130,50,121,243,143,127,102,1,47,92,135,247,219,43,50,54,81,168,248,0,99,85,209,24,31,114,209,95,250,131,129,241,190,103,116,12,36,27,155,148,141,211,179,82,253,255,192,240,44,80,229,176,240,193,168,211,207,229,97,103,205,34,166,91,50,54,38,112,139,221,239,171,27,210,161,77,77,18,135,51,90,11,102,32,145,36,65,30,169,220,35,200,198,164,246,216,85,23,129,102,139,100,133,243,148,182,212,37,115,132,90,44,248,143,34,255,5,35,73,65,184,38,56,0,0 };
