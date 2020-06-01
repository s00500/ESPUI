const char JS_CONTROLS[] PROGMEM = R"=====(
const UI_INITIAL_GUI=200;const UPDATE_OFFSET=100;const UI_TITEL=0;const UI_PAD=1;const UPDATE_PAD=101;const UI_CPAD=2;const UPDATE_CPAD=102;const UI_BUTTON=3;const UPDATE_BUTTON=103;const UI_LABEL=4;const UPDATE_LABEL=104;const UI_SWITCHER=5;const UPDATE_SWITCHER=105;const UI_SLIDER=6;const UPDATE_SLIDER=106;const UI_NUMBER=7;const UPDATE_NUMBER=107;const UI_TEXT_INPUT=8;const UPDATE_TEXT_INPUT=108;const UI_GRAPH=9;const ADD_GRAPH_POINT=10;const CLEAR_GRAPH=109;const UI_TAB=11;const UPDATE_TAB=111;const UI_SELECT=12;const UPDATE_SELECT=112;const UI_OPTION=13;const UPDATE_OPTION=113;const UI_MIN=14;const UPDATE_MIN=114;const UI_MAX=15;const UPDATE_MAX=115;const UI_STEP=16;const UPDATE_STEP=116;const UI_GAUGE=17;const UPDATE_GAUGE=117;const UI_ACCEL=18;const UPTDATE_ACCEL=117;const UP=0;const DOWN=1;const LEFT=2;const RIGHT=3;const CENTER=4;const C_TURQUOISE=0;const C_EMERALD=1;const C_PETERRIVER=2;const C_WETASPHALT=3;const C_SUNFLOWER=4;const C_CARROT=5;const C_ALIZARIN=6;const C_DARK=7;const C_NONE=255;var graphData=new Array();var hasAccel=false;var sliderContinuous=false;function colorClass(colorId){colorId=Number(colorId);switch(colorId){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:return"dark";default:return"";}}
var websock;var websockConnected=false;function requestOrientationPermission(){}
function saveGraphData(){localStorage.setItem("espuigraphs",JSON.stringify(graphData));}
function restoreGraphData(id){var savedData=localStorage.getItem("espuigraphs",graphData);if(savedData!=null){savedData=JSON.parse(savedData);return savedData[id];}
return[];}
function restart(){$(document).add("*").off();$("#row").html("");websock.close();start();}
function conStatusError(){websockConnected=false;$("#conStatus").removeClass("color-green");$("#conStatus").addClass("color-red");$("#conStatus").html("Error / No Connection &#8635;");$("#conStatus").off();$("#conStatus").on({click:restart});}
function handleVisibilityChange(){if(!websockConnected&&!document.hidden){restart();}}
function start(){document.addEventListener("visibilitychange",handleVisibilityChange,false);if(window.location.port!=""||window.location.port!=80||window.location.port!=443){websock=new WebSocket("ws://"+window.location.hostname+":"+window.location.port+"/ws");}else{websock=new WebSocket("ws://"+window.location.hostname+"/ws");}
websock.onopen=function(evt){console.log("websock open");$("#conStatus").addClass("color-green");$("#conStatus").text("Connected");websockConnected=true;};websock.onclose=function(evt){console.log("websock close");conStatusError();};websock.onerror=function(evt){console.log(evt);conStatusError();};var handleEvent=function(evt){var data=JSON.parse(evt.data);var e=document.body;var center="";switch(data.type){case UI_INITIAL_GUI:if(data.sliderContinuous){sliderContinuous=data.sliderContinuous;}
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
if(data.type>=UPDATE_OFFSET&&data.type<UI_INITIAL_GUI){var element=$("#id"+data.id);if(data.type==UPDATE_SLIDER){element.removeClass("slider-turquoise slider-emerald slider-peterriver slider-wetasphalt slider-sunflower slider-carrot slider-alizarin");element.addClass("slider-"+colorClass(data.color));}else{element.removeClass("turquoise emerald peterriver wetasphalt sunflower carrot alizarin");element.addClass(colorClass(data.color));}}};websock.onmessage=handleEvent;}
function sliderchange(number){var val=$("#sl"+number).val();websock.send("slvalue:"+val+":"+number);}
function numberchange(number){var val=$("#num"+number).val();websock.send("nvalue:"+val+":"+number);}
function textchange(number){var val=$("#text"+number).val();websock.send("tvalue:"+val+":"+number);}
function selectchange(number){var val=$("#select"+number).val();websock.send("svalue:"+val+":"+number);}
function buttonclick(number,isdown){if(isdown)websock.send("bdown:"+number);else websock.send("bup:"+number);}
function padclick(type,number,isdown){switch(type){case CENTER:if(isdown)websock.send("pcdown:"+number);else websock.send("pcup:"+number);break;case UP:if(isdown)websock.send("pfdown:"+number);else websock.send("pfup:"+number);break;case DOWN:if(isdown)websock.send("pbdown:"+number);else websock.send("pbup:"+number);break;case LEFT:if(isdown)websock.send("pldown:"+number);else websock.send("plup:"+number);break;case RIGHT:if(isdown)websock.send("prdown:"+number);else websock.send("prup:"+number);break;}}
function switcher(number,state){if(state==null){if($("#s"+number).is(":checked")){websock.send("sactive:"+number);$("#sl"+number).addClass("checked");}else{websock.send("sinactive:"+number);$("#sl"+number).removeClass("checked");}}else if(state==1){$("#sl"+number).addClass("checked");$("#sl"+number).prop("checked",true);}else if(state==0){$("#sl"+number).removeClass("checked");$("#sl"+number).prop("checked",false);}}
var rangeSlider=function(isDiscrete){var slider=$(".range-slider"),range=$(".range-slider__range"),value=$(".range-slider__value");slider.each(function(){value.each(function(){var value=$(this).prev().attr("value");$(this).html(value);});if(!isDiscrete){range.on({input:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});}else{range.on({input:function(){$(this).next().html(this.value);},change:function(){sliderchange($(this).attr("id").replace(/^\D+/g,""));}});}});};
)=====";

const uint8_t JS_CONTROLS_GZIP[3316] PROGMEM = {31, 139, 8, 0, 226, 211, 46, 94, 2, 255, 237, 90, 235, 118, 219, 54, 18, 254, 159, 167, 160, 153, 158, 136, 92, 203, 186, 228, 214, 84, 50, 157, 163, 200, 74, 162, 173, 99, 123, 109, 185, 233, 217, 52, 235, 3, 137, 144, 197, 19, 138, 100, 73, 208, 151, 186, 122, 141, 125, 144, 125, 164, 125, 146, 29, 12, 0, 18, 164, 40, 89, 137, 219, 110, 247, 100, 127, 196, 17, 7, 131, 15, 31, 102, 6, 247, 153, 132, 65, 194, 140, 179, 225, 249, 240, 112, 56, 26, 246, 14, 206, 223, 156, 13, 157, 199, 173, 86, 119, 34, 10, 142, 247, 123, 163, 193, 249, 209, 235, 215, 167, 131, 145, 211, 206, 229, 195, 243, 209, 112, 52, 56, 112, 52, 193, 113, 111, 223, 105, 23, 235, 161, 168, 213, 206, 117, 250, 92, 242, 184, 168, 212, 23, 90, 143, 115, 173, 87, 103, 163, 209, 209, 161, 243, 164, 168, 39, 165, 237, 214, 147, 92, 243, 160, 247, 10, 72, 60, 45, 42, 10, 97, 187, 245, 52, 215, 59, 125, 63, 28, 245, 223, 14, 78, 156, 103, 69, 213, 76, 222, 110, 61, 211, 180, 15, 134, 251, 32, 123, 94, 210, 21, 210, 118, 235, 121, 174, 121, 120, 246, 238, 21, 200, 190, 45, 106, 74, 105, 187, 245, 173, 102, 174, 193, 143, 35, 48, 242, 241, 217, 200, 121, 81, 212, 214, 74, 218, 173, 23, 121, 141, 55, 39, 189, 227, 183, 206, 119, 82, 208, 219, 223, 23, 146, 243, 227, 163, 225, 33, 87, 149, 5, 253, 131, 65, 239, 68, 42, 183, 91, 223, 105, 45, 246, 94, 57, 237, 146, 67, 132, 76, 115, 200, 233, 224, 96, 208, 7, 180, 146, 79, 148, 184, 173, 121, 229, 232, 120, 52, 228, 246, 47, 185, 69, 137, 219, 154, 91, 222, 13, 65, 80, 242, 10, 202, 218, 154, 79, 222, 245, 126, 116, 218, 37, 127, 160, 172, 173, 187, 98, 52, 56, 118, 218, 101, 79, 160, 176, 173, 249, 225, 77, 239, 236, 205, 192, 105, 231, 126, 24, 161, 162, 20, 183, 53, 63, 244, 250, 125, 30, 28, 47, 74, 154, 82, 172, 33, 100, 161, 189, 127, 244, 254, 48, 11, 236, 131, 193, 235, 81, 22, 192, 39, 195, 55, 111, 71, 89, 152, 246, 7, 135, 35, 240, 186, 234, 97, 255, 124, 116, 118, 242, 183, 179, 163, 225, 233, 32, 131, 234, 159, 15, 222, 13, 78, 122, 7, 249, 64, 233, 159, 31, 15, 160, 214, 201, 240, 7, 168, 250, 56, 19, 190, 31, 140, 122, 167, 199, 111, 123, 7, 26, 252, 249, 233, 217, 225, 235, 131, 163, 247, 133, 54, 250, 189, 147, 147, 163, 81, 22, 214, 253, 243, 222, 193, 240, 239, 189, 19, 176, 245, 243, 76, 180, 223, 59, 249, 62, 11, 209, 254, 249, 225, 209, 225, 192, 121, 252, 236, 89, 247, 146, 196, 198, 69, 76, 162, 217, 62, 97, 196, 9, 232, 149, 209, 139, 99, 114, 99, 217, 88, 50, 35, 73, 111, 50, 161, 190, 51, 37, 126, 66, 81, 148, 248, 158, 75, 227, 126, 24, 48, 47, 72, 195, 52, 145, 69, 211, 52, 152, 48, 47, 12, 140, 73, 232, 135, 113, 223, 39, 73, 98, 225, 207, 161, 107, 223, 202, 31, 206, 97, 58, 31, 211, 56, 147, 119, 147, 43, 143, 77, 102, 154, 30, 73, 168, 110, 179, 78, 76, 89, 26, 7, 38, 252, 249, 57, 13, 189, 132, 154, 93, 169, 34, 77, 168, 20, 232, 156, 198, 196, 119, 179, 226, 220, 158, 74, 35, 162, 140, 198, 177, 119, 73, 227, 76, 41, 183, 175, 82, 186, 162, 140, 36, 209, 140, 248, 44, 83, 202, 236, 173, 116, 146, 52, 152, 250, 225, 149, 134, 35, 204, 175, 202, 39, 36, 142, 195, 188, 190, 114, 133, 42, 38, 190, 247, 11, 137, 189, 32, 83, 224, 158, 80, 133, 46, 137, 63, 153, 93, 151, 78, 73, 234, 51, 37, 52, 187, 139, 197, 3, 110, 249, 43, 58, 78, 194, 201, 167, 174, 246, 27, 220, 16, 208, 9, 163, 110, 217, 11, 49, 253, 57, 165, 9, 59, 138, 61, 26, 48, 194, 69, 199, 52, 158, 123, 73, 2, 191, 44, 251, 118, 241, 32, 211, 76, 200, 37, 125, 163, 2, 0, 138, 252, 112, 66, 252, 83, 22, 198, 228, 130, 54, 18, 202, 134, 140, 206, 45, 147, 38, 81, 234, 97, 156, 36, 102, 253, 175, 167, 71, 135, 141, 132, 65, 47, 46, 188, 233, 141, 149, 133, 143, 109, 119, 53, 224, 24, 218, 15, 99, 13, 219, 3, 15, 99, 4, 65, 139, 46, 134, 91, 161, 173, 139, 202, 182, 114, 240, 174, 55, 181, 178, 170, 91, 78, 144, 250, 190, 125, 155, 99, 33, 169, 136, 196, 9, 205, 181, 236, 174, 176, 97, 222, 228, 7, 207, 253, 8, 36, 133, 248, 195, 199, 50, 95, 18, 51, 48, 193, 55, 150, 27, 78, 210, 57, 88, 206, 110, 16, 215, 181, 204, 191, 152, 118, 35, 156, 78, 97, 80, 124, 99, 153, 15, 227, 240, 10, 190, 103, 108, 238, 91, 166, 105, 119, 165, 43, 26, 19, 63, 132, 182, 33, 168, 5, 138, 14, 13, 163, 238, 20, 188, 144, 38, 3, 136, 141, 24, 90, 88, 225, 62, 142, 158, 233, 66, 27, 49, 157, 135, 151, 84, 12, 38, 19, 71, 201, 206, 69, 76, 105, 96, 218, 75, 170, 192, 179, 160, 23, 83, 183, 66, 75, 144, 70, 22, 70, 211, 56, 12, 13, 201, 128, 147, 124, 244, 240, 197, 243, 39, 207, 186, 21, 181, 242, 174, 23, 164, 129, 117, 59, 241, 189, 201, 167, 142, 180, 220, 162, 208, 231, 25, 9, 92, 159, 254, 224, 37, 222, 216, 243, 61, 118, 211, 7, 193, 5, 216, 231, 22, 220, 184, 85, 238, 255, 163, 71, 91, 202, 228, 141, 153, 231, 186, 52, 176, 111, 51, 127, 240, 248, 207, 195, 85, 250, 40, 83, 135, 142, 15, 46, 225, 199, 129, 151, 48, 26, 192, 236, 98, 94, 102, 109, 78, 176, 77, 179, 94, 205, 165, 142, 70, 199, 184, 186, 242, 2, 55, 188, 106, 240, 120, 228, 173, 52, 162, 48, 102, 91, 142, 105, 254, 250, 107, 117, 201, 139, 214, 170, 146, 167, 79, 159, 100, 238, 197, 233, 244, 61, 29, 159, 194, 111, 202, 44, 243, 42, 233, 52, 155, 230, 118, 185, 226, 44, 76, 88, 64, 230, 116, 219, 236, 44, 23, 114, 212, 109, 179, 121, 5, 22, 239, 46, 40, 240, 253, 98, 112, 137, 241, 64, 197, 107, 24, 132, 17, 13, 28, 101, 89, 139, 94, 50, 62, 83, 7, 73, 232, 83, 168, 127, 1, 144, 66, 211, 224, 122, 27, 132, 220, 170, 208, 100, 244, 26, 248, 101, 190, 206, 135, 76, 30, 254, 44, 78, 105, 119, 209, 205, 169, 225, 96, 218, 132, 27, 42, 2, 100, 121, 136, 21, 208, 40, 151, 173, 65, 227, 130, 42, 4, 177, 4, 242, 216, 193, 16, 43, 33, 240, 82, 183, 52, 245, 64, 65, 195, 197, 153, 135, 151, 82, 39, 11, 211, 113, 232, 222, 160, 108, 2, 95, 52, 134, 216, 82, 171, 31, 87, 111, 176, 155, 136, 202, 245, 175, 184, 15, 239, 64, 116, 162, 70, 121, 221, 133, 185, 175, 188, 18, 87, 234, 129, 199, 81, 14, 221, 99, 113, 232, 39, 141, 105, 24, 15, 8, 180, 75, 125, 202, 153, 57, 123, 216, 17, 88, 114, 174, 69, 39, 111, 185, 122, 167, 52, 199, 75, 101, 123, 209, 213, 236, 97, 101, 149, 192, 88, 118, 119, 28, 83, 242, 169, 171, 250, 128, 71, 131, 78, 214, 127, 230, 49, 159, 10, 134, 62, 25, 83, 31, 163, 100, 78, 188, 224, 45, 37, 192, 87, 205, 77, 185, 66, 25, 15, 55, 244, 29, 78, 21, 44, 13, 136, 93, 101, 24, 241, 217, 23, 221, 179, 111, 197, 167, 195, 225, 25, 25, 155, 219, 21, 58, 114, 32, 105, 154, 56, 165, 131, 165, 132, 168, 65, 34, 136, 120, 152, 247, 119, 93, 239, 210, 240, 92, 167, 230, 185, 230, 182, 48, 163, 231, 110, 63, 48, 107, 16, 118, 16, 248, 78, 141, 93, 133, 124, 195, 147, 206, 131, 196, 128, 133, 223, 53, 152, 112, 175, 1, 234, 218, 70, 72, 58, 0, 190, 109, 94, 123, 15, 74, 205, 221, 217, 179, 61, 5, 138, 29, 230, 178, 38, 8, 119, 103, 113, 83, 104, 36, 17, 9, 176, 121, 191, 186, 117, 172, 102, 224, 223, 157, 43, 88, 39, 107, 25, 224, 37, 241, 83, 138, 128, 28, 67, 160, 53, 161, 51, 123, 102, 217, 172, 226, 64, 245, 103, 179, 43, 12, 217, 223, 209, 174, 227, 148, 177, 80, 88, 118, 204, 130, 18, 3, 174, 17, 6, 115, 24, 57, 20, 38, 210, 0, 84, 80, 27, 87, 58, 171, 160, 91, 55, 248, 180, 101, 23, 170, 164, 209, 218, 10, 98, 197, 169, 242, 147, 168, 180, 151, 121, 137, 91, 15, 201, 201, 234, 98, 189, 101, 97, 58, 153, 225, 18, 216, 201, 231, 34, 251, 150, 54, 162, 152, 242, 113, 184, 47, 182, 142, 48, 121, 233, 36, 36, 68, 29, 233, 118, 23, 117, 68, 1, 71, 124, 9, 134, 92, 51, 23, 75, 227, 93, 157, 162, 191, 174, 80, 18, 67, 144, 183, 159, 172, 24, 164, 98, 150, 231, 173, 89, 185, 203, 29, 199, 108, 155, 47, 77, 240, 2, 44, 222, 46, 172, 251, 166, 214, 42, 239, 145, 172, 236, 5, 181, 189, 93, 47, 136, 82, 102, 240, 21, 194, 169, 97, 141, 113, 120, 93, 19, 109, 150, 154, 12, 131, 62, 119, 150, 106, 148, 111, 134, 10, 241, 135, 123, 230, 218, 70, 92, 154, 42, 20, 177, 207, 216, 205, 226, 52, 146, 53, 161, 34, 35, 71, 44, 135, 6, 191, 220, 233, 168, 15, 254, 251, 171, 154, 197, 3, 146, 185, 83, 174, 193, 178, 106, 42, 45, 234, 123, 123, 187, 196, 40, 204, 57, 17, 113, 197, 176, 59, 59, 174, 27, 213, 147, 142, 54, 225, 172, 211, 150, 51, 14, 246, 63, 154, 22, 251, 191, 247, 239, 127, 254, 107, 183, 73, 192, 211, 64, 225, 78, 42, 120, 209, 241, 57, 108, 170, 43, 20, 8, 197, 247, 33, 196, 175, 97, 62, 135, 79, 165, 126, 129, 142, 127, 31, 58, 252, 138, 232, 115, 232, 84, 234, 23, 232, 140, 239, 162, 211, 20, 33, 148, 111, 33, 29, 71, 14, 183, 151, 38, 240, 204, 163, 110, 234, 197, 243, 218, 10, 222, 226, 202, 170, 190, 57, 113, 89, 97, 45, 245, 73, 137, 250, 209, 247, 156, 185, 154, 92, 118, 155, 48, 40, 138, 211, 9, 31, 191, 60, 62, 191, 112, 185, 211, 135, 192, 151, 46, 119, 85, 24, 218, 114, 135, 12, 253, 251, 51, 196, 48, 188, 55, 199, 2, 74, 153, 101, 124, 127, 150, 98, 240, 222, 155, 102, 17, 166, 204, 115, 124, 127, 158, 56, 138, 238, 77, 179, 128, 82, 102, 57, 185, 63, 75, 57, 100, 238, 205, 179, 132, 179, 122, 51, 134, 207, 20, 255, 51, 235, 44, 255, 216, 17, 7, 215, 223, 102, 205, 213, 182, 80, 49, 191, 234, 145, 224, 178, 190, 216, 80, 85, 110, 218, 196, 46, 11, 235, 212, 140, 185, 7, 243, 100, 11, 254, 39, 215, 78, 173, 221, 130, 95, 98, 195, 84, 43, 111, 223, 107, 85, 141, 157, 159, 11, 24, 237, 44, 87, 169, 133, 32, 155, 28, 221, 244, 233, 18, 33, 78, 17, 193, 218, 90, 186, 26, 40, 69, 131, 120, 138, 250, 186, 118, 93, 194, 199, 9, 187, 241, 249, 174, 153, 87, 239, 140, 125, 50, 249, 212, 21, 11, 84, 144, 206, 43, 61, 31, 224, 11, 197, 26, 71, 195, 121, 8, 47, 15, 149, 170, 248, 42, 238, 179, 97, 17, 108, 174, 59, 115, 231, 143, 125, 255, 247, 137, 230, 19, 126, 79, 88, 162, 181, 129, 27, 120, 173, 47, 114, 66, 239, 85, 71, 26, 53, 129, 221, 8, 191, 207, 84, 86, 18, 187, 188, 89, 76, 167, 78, 77, 51, 58, 192, 98, 95, 53, 42, 102, 190, 31, 19, 139, 5, 71, 227, 187, 124, 176, 154, 142, 168, 236, 94, 6, 203, 184, 241, 122, 220, 115, 141, 34, 0, 124, 141, 169, 219, 23, 223, 214, 173, 31, 134, 81, 135, 47, 30, 11, 27, 111, 25, 45, 147, 68, 158, 104, 153, 128, 246, 212, 243, 193, 87, 86, 182, 152, 240, 59, 116, 124, 254, 248, 198, 98, 51, 47, 1, 62, 140, 193, 145, 144, 119, 204, 180, 29, 56, 247, 61, 196, 85, 101, 135, 133, 59, 226, 52, 103, 118, 23, 184, 196, 153, 40, 55, 235, 250, 178, 196, 35, 21, 232, 56, 81, 28, 206, 35, 102, 153, 35, 50, 54, 88, 104, 200, 227, 45, 252, 178, 196, 128, 48, 194, 24, 186, 106, 191, 52, 241, 98, 125, 139, 247, 167, 33, 148, 160, 134, 5, 159, 182, 125, 75, 124, 26, 115, 136, 25, 97, 28, 211, 112, 67, 154, 24, 65, 200, 12, 122, 237, 37, 204, 232, 252, 244, 19, 70, 111, 197, 74, 184, 188, 210, 225, 35, 245, 87, 118, 47, 72, 125, 58, 89, 51, 144, 68, 249, 210, 221, 128, 26, 49, 162, 248, 75, 198, 140, 120, 230, 239, 172, 176, 112, 238, 3, 180, 157, 98, 81, 101, 232, 178, 61, 195, 8, 31, 119, 56, 121, 241, 115, 211, 121, 64, 73, 68, 99, 148, 107, 87, 88, 79, 96, 238, 161, 43, 139, 61, 122, 55, 220, 180, 59, 220, 213, 203, 106, 219, 166, 129, 179, 154, 136, 118, 217, 173, 196, 251, 133, 90, 182, 10, 57, 57, 234, 96, 51, 97, 22, 238, 72, 22, 75, 92, 122, 63, 254, 81, 92, 200, 245, 29, 92, 120, 118, 197, 31, 68, 38, 97, 52, 186, 131, 13, 38, 182, 124, 93, 99, 124, 234, 93, 164, 49, 69, 2, 248, 4, 94, 58, 79, 43, 157, 9, 145, 161, 189, 132, 86, 44, 20, 2, 64, 44, 14, 239, 236, 117, 253, 131, 4, 255, 232, 44, 61, 219, 171, 179, 79, 23, 204, 3, 219, 76, 44, 56, 189, 188, 176, 150, 235, 214, 77, 73, 53, 171, 162, 185, 177, 148, 187, 132, 222, 100, 137, 243, 142, 176, 89, 35, 14, 83, 48, 57, 127, 208, 4, 60, 136, 16, 158, 11, 48, 242, 230, 240, 171, 9, 219, 238, 86, 21, 207, 70, 148, 38, 51, 235, 246, 186, 195, 146, 250, 77, 39, 143, 30, 88, 32, 74, 25, 13, 247, 228, 173, 165, 86, 117, 42, 204, 245, 225, 227, 111, 220, 158, 74, 100, 250, 170, 194, 253, 253, 240, 224, 192, 120, 53, 48, 122, 6, 246, 221, 184, 107, 171, 120, 65, 210, 11, 250, 223, 219, 192, 99, 162, 24, 159, 30, 85, 126, 148, 45, 202, 127, 103, 151, 101, 217, 88, 248, 92, 254, 167, 114, 32, 90, 196, 104, 242, 156, 18, 56, 89, 207, 35, 241, 92, 76, 93, 99, 154, 250, 254, 205, 150, 126, 32, 39, 188, 11, 20, 118, 145, 60, 35, 75, 184, 19, 69, 37, 214, 123, 122, 157, 49, 241, 203, 119, 179, 194, 53, 187, 176, 67, 172, 4, 14, 83, 198, 87, 159, 114, 157, 168, 60, 3, 174, 79, 148, 42, 248, 93, 75, 44, 197, 147, 131, 126, 23, 152, 63, 94, 87, 188, 128, 20, 211, 76, 59, 107, 94, 78, 96, 79, 222, 50, 95, 182, 58, 237, 74, 0, 113, 169, 35, 239, 13, 120, 102, 144, 133, 251, 44, 141, 135, 6, 85, 55, 97, 234, 52, 213, 21, 209, 50, 154, 188, 20, 224, 8, 120, 24, 206, 186, 2, 181, 239, 232, 137, 118, 126, 197, 64, 198, 115, 219, 103, 212, 151, 123, 246, 165, 77, 226, 102, 181, 229, 139, 181, 46, 202, 223, 151, 242, 108, 226, 206, 114, 77, 49, 175, 242, 102, 229, 252, 241, 25, 173, 138, 33, 47, 228, 42, 65, 79, 165, 144, 96, 122, 137, 101, 158, 5, 159, 130, 240, 42, 192, 137, 136, 159, 130, 240, 232, 146, 205, 29, 139, 7, 106, 74, 224, 229, 123, 78, 33, 171, 250, 209, 163, 172, 100, 183, 152, 255, 33, 182, 91, 42, 87, 163, 176, 223, 226, 11, 135, 142, 233, 56, 133, 64, 177, 111, 101, 173, 98, 26, 153, 8, 159, 157, 44, 165, 82, 38, 116, 238, 200, 20, 74, 245, 153, 231, 75, 42, 73, 158, 28, 169, 36, 89, 42, 164, 18, 136, 220, 71, 245, 149, 165, 58, 218, 93, 197, 36, 79, 25, 146, 58, 230, 118, 245, 212, 163, 102, 193, 202, 46, 228, 220, 21, 105, 141, 173, 78, 51, 227, 39, 137, 173, 99, 180, 146, 199, 66, 207, 36, 154, 211, 36, 33, 176, 120, 104, 89, 48, 122, 210, 155, 232, 149, 92, 82, 196, 250, 34, 28, 8, 113, 229, 168, 193, 42, 229, 24, 116, 121, 250, 96, 130, 179, 120, 226, 99, 4, 118, 204, 109, 248, 31, 19, 194, 164, 182, 222, 74, 97, 225, 170, 106, 5, 7, 244, 186, 102, 130, 13, 90, 209, 46, 86, 170, 218, 16, 163, 126, 93, 35, 108, 131, 70, 10, 103, 209, 74, 131, 201, 9, 98, 173, 209, 54, 104, 72, 207, 101, 16, 197, 117, 47, 225, 47, 97, 152, 144, 40, 127, 22, 97, 199, 92, 166, 161, 241, 128, 52, 74, 42, 105, 84, 221, 92, 118, 99, 207, 7, 102, 189, 212, 160, 76, 252, 210, 114, 190, 196, 189, 126, 103, 21, 147, 104, 114, 55, 149, 104, 82, 224, 82, 152, 193, 86, 3, 79, 55, 0, 158, 174, 2, 230, 175, 38, 171, 161, 55, 48, 95, 52, 94, 5, 205, 159, 183, 86, 67, 251, 27, 64, 251, 171, 160, 241, 73, 106, 53, 118, 188, 1, 118, 92, 129, 93, 72, 82, 85, 43, 188, 116, 124, 2, 59, 11, 138, 129, 134, 191, 28, 153, 188, 12, 223, 24, 226, 121, 116, 123, 48, 187, 117, 84, 38, 134, 157, 37, 146, 170, 56, 39, 0, 127, 73, 181, 166, 203, 83, 138, 150, 146, 169, 64, 138, 89, 163, 10, 201, 11, 238, 196, 42, 38, 31, 231, 112, 136, 103, 228, 125, 105, 243, 132, 233, 187, 105, 148, 117, 162, 56, 140, 242, 114, 245, 36, 86, 194, 110, 45, 99, 175, 160, 117, 7, 124, 246, 82, 134, 153, 244, 218, 235, 73, 158, 211, 233, 37, 251, 94, 50, 137, 41, 147, 151, 157, 98, 46, 199, 75, 89, 253, 193, 198, 180, 235, 248, 185, 84, 32, 223, 123, 160, 92, 108, 229, 150, 203, 81, 206, 83, 103, 240, 187, 65, 121, 22, 166, 118, 89, 139, 197, 21, 210, 216, 80, 128, 226, 22, 151, 95, 138, 90, 234, 50, 87, 97, 170, 66, 220, 136, 170, 123, 20, 113, 5, 171, 119, 12, 9, 225, 43, 38, 158, 176, 58, 90, 67, 133, 181, 171, 120, 99, 12, 91, 14, 110, 248, 8, 78, 96, 212, 106, 254, 227, 167, 253, 237, 230, 69, 221, 52, 109, 241, 244, 40, 2, 108, 13, 178, 2, 11, 120, 34, 176, 164, 200, 37, 217, 125, 79, 93, 180, 250, 219, 176, 225, 255, 186, 255, 1, 102, 130, 5, 32, 211, 54, 0, 0};
