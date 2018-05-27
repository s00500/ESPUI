const char JS_CONTROLS[] PROGMEM = R"=====(
function colorClass(e){switch(e=Number(e)){case C_TURQUOISE:return"turquoise";case C_EMERALD:return"emerald";case C_PETERRIVER:return"peterriver";case C_WETASPHALT:return"wetasphalt";case C_SUNFLOWER:return"sunflower";case C_CARROT:return"carrot";case C_ALIZARIN:return"alizarin";case C_NONE:default:return""}}function start(){websock=new WebSocket("ws://"+window.location.hostname+"/ws"),websock.onopen=function(e){console.log("websock open"),$("#conStatus").addClass("color-green"),$("#conStatus").text("Connected")},websock.onclose=function(e){console.log("websock close"),$("#conStatus").removeClass("color-green"),$("#conStatus").addClass("color-red"),$("#conStatus").text("Error / No Connection")},websock.onerror=function(e){console.log(e),$("#conStatus").removeClass("color-green"),$("#conStatus").addClass("color-red"),$("#conStatus").text("Error / No Connection")},websock.onmessage=function(e){console.log(e);var c=JSON.parse(e.data),o=(document.body,"");switch(c.type){case UI_TITEL:document.title=c.label,$("#mainHeader").html(c.label);break;case UI_LABEL:$("#row").append("<div class='two columns card tcenter "+colorClass(c.color)+"'><h5 id='"+c.id+"'>"+c.label+"</h5><hr /><span id='l"+c.id+"' class='label label-wrap'>"+c.value+"</span></div>");break;case UI_BUTTON:$("#row").append("<div class='one columns card tcenter "+colorClass(c.color)+"'><h5>"+c.label+"</h5><hr/><button onmousedown='buttonclick("+c.id+", true)' onmouseup='buttonclick("+c.id+", false)' id='"+c.id+"'>"+c.value+"</button></div>"),$("#"+c.id).on({touchstart:function(e){e.preventDefault(),buttonclick(c.id,!0)}}),$("#"+c.id).on({touchend:function(e){e.preventDefault(),buttonclick(c.id,!1)}});break;case UI_SWITCHER:var s="<label id='sl"+c.id+"' class='switch checked'>",n="<div class='in'><input type='checkbox' id='s"+c.id+"' onClick='switcher("+c.id+",null)' checked></div>";"0"==c.value&&(s="<label id='sl"+c.id+"' class='switch'>",n="<div class='in'><input type='checkbox' id='s"+c.id+"' onClick='switcher("+c.id+",null)' ></div>"),$("#row").append("<div id='"+c.id+"' class='one columns card tcenter "+colorClass(c.color)+"'><h5>"+c.label+"</h5><hr/>"+s+n+"</label></div>");break;case UI_CPAD:o="<a class='confirm' onmousedown='padclick(CENTER, "+c.id+", true)' onmouseup='padclick(CENTER, "+c.id+", false)' href='#' id='pc"+c.id+"'>OK</a>";case UI_PAD:$("#row").append("<div class='two columns card tcenter "+colorClass(c.color)+"'><h5>"+c.label+"</h5><hr/><nav class='control'><ul><li><a onmousedown='padclick(FOR, "+c.id+", true)' onmouseup='padclick(FOR, "+c.id+", false)' href='#' id='pf"+c.id+"'>▲</a></li><li><a onmousedown='padclick(RIGHT, "+c.id+", true)' onmouseup='padclick(RIGHT, "+c.id+", false)' href='#' id='pr"+c.id+"'>▲</a></li><li><a onmousedown='padclick(LEFT, "+c.id+", true)' onmouseup='padclick(LEFT, "+c.id+", false)' href='#' id='pl"+c.id+"'>▲</a></li><li><a onmousedown='padclick(BACK, "+c.id+", true)' onmouseup='padclick(BACK, "+c.id+", false)' href='#' id='pb"+c.id+"'>▲</a></li></ul>"+o+"</nav></div>"),$("#pf"+c.id).on({touchstart:function(e){e.preventDefault(),padclick(FOR,c.id,!0)}}),$("#pf"+c.id).on({touchend:function(e){e.preventDefault(),padclick(FOR,c.id,!1)}}),$("#pl"+c.id).on({touchstart:function(e){e.preventDefault(),padclick(LEFT,c.id,!0)}}),$("#pl"+c.id).on({touchend:function(e){e.preventDefault(),padclick(LEFT,c.id,!1)}}),$("#pr"+c.id).on({touchstart:function(e){e.preventDefault(),padclick(RIGHT,c.id,!0)}}),$("#pr"+c.id).on({touchend:function(e){e.preventDefault(),padclick(RIGHT,c.id,!1)}}),$("#pb"+c.id).on({touchstart:function(e){e.preventDefault(),padclick(BACK,c.id,!0)}}),$("#pb"+c.id).on({touchend:function(e){e.preventDefault(),padclick(BACK,c.id,!1)}}),$("#pc"+c.id).on({touchstart:function(e){e.preventDefault(),padclick(CENTER,c.id,!0)}}),$("#pc"+c.id).on({touchend:function(e){e.preventDefault(),padclick(CENTER,c.id,!1)}});break;case UPDATE_LABEL:$("#l"+c.id).html(c.value);break;case UPDATE_SWITCHER:"0"==c.value?switcher(c.id,0):switcher(c.id,1);break;case UI_SLIDER:$("#row").append("<div class='two columns card tcenter card-slider "+colorClass(c.color)+"'><h5 id='"+c.id+"'>"+c.label+"</h5><hr /><div id='sl"+c.id+"' class='rkmd-slider slider-discrete slider-"+colorClass(c.color)+"'><input type='range' min='0' max='100' value='"+c.value+"'></div></div>"),$("#row").append("<script>rkmd_rangeSlider('#sl"+c.id+"');<\/script>");break;case UPDATE_SLIDER:slider_move($("#sl"+c.id),c.value,"100",!1);break;default:console.error("Unknown type or event")}}}function buttonclick(e,c){c?websock.send("bdown:"+e):websock.send("bup:"+e)}function padclick(e,c,o){switch(e){case CENTER:o?websock.send("pcdown:"+c):websock.send("pcup:"+c);break;case FOR:o?websock.send("pfdown:"+c):websock.send("pfup:"+c);break;case BACK:o?websock.send("pbdown:"+c):websock.send("pbup:"+c);break;case LEFT:o?websock.send("pldown:"+c):websock.send("plup:"+c);break;case RIGHT:o?websock.send("prdown:"+c):websock.send("prup:"+c)}}function switcher(e,c){null==c?$("#s"+e).is(":checked")?(websock.send("sactive:"+e),$("#sl"+e).addClass("checked")):(websock.send("sinactive:"+e),$("#sl"+e).removeClass("checked")):1==c?($("#sl"+e).addClass("checked"),$("#sl"+e).prop("checked",!0)):0==c&&($("#sl"+e).removeClass("checked"),$("#sl"+e).prop("checked",!1))}const UI_TITEL=0,UI_LABEL=1,UI_BUTTON=2,UI_SWITCHER=3,UI_PAD=4,UI_CPAD=5,UPDATE_LABEL=6,UPDATE_SWITCHER=7,UI_SLIDER=8,UPDATE_SLIDER=9,FOR=0,BACK=1,LEFT=2,RIGHT=3,CENTER=4,C_TURQUOISE=0,C_EMERALD=1,C_PETERRIVER=2,C_WETASPHALT=3,C_SUNFLOWER=4,C_CARROT=5,C_ALIZARIN=6,C_NONE=7;var websock;
)=====";

const uint8_t JS_CONTROLS_GZIP[1574] PROGMEM = { 31,139,8,0,0,0,0,0,0,3,205,88,235,114,226,54,20,254,223,167,112,181,157,197,30,28,46,109,183,219,2,78,134,37,108,67,151,66,10,164,153,233,116,38,35,108,17,60,49,146,43,201,176,219,76,94,163,15,210,71,234,147,244,72,190,96,176,201,146,77,119,166,127,184,88,71,223,249,116,110,62,58,139,136,186,210,103,212,112,89,192,120,47,192,66,152,196,186,23,27,95,186,75,147,56,163,104,53,39,28,30,89,247,46,22,196,232,221,204,174,38,191,92,141,7,211,126,139,19,25,113,138,224,227,143,136,249,130,160,118,34,210,255,185,63,233,14,207,83,1,178,34,28,7,94,182,124,217,159,245,39,147,193,175,253,73,42,17,18,73,56,247,215,132,103,66,215,253,89,119,122,121,209,29,206,82,161,13,145,88,132,75,28,200,76,104,122,53,122,59,28,95,111,129,68,68,23,1,219,228,112,122,221,201,100,156,97,184,152,115,182,221,223,29,14,126,235,78,6,163,116,25,7,254,159,152,251,52,19,24,141,71,253,150,71,22,56,10,100,42,132,30,30,22,169,213,132,196,92,154,214,253,134,204,5,115,239,28,74,54,198,53,153,79,225,55,145,38,218,136,86,189,142,170,27,159,122,108,83,11,152,139,213,174,218,146,9,73,241,138,84,81,125,35,144,101,39,187,107,140,178,144,80,39,69,87,142,112,25,21,44,32,176,247,22,224,98,57,67,73,193,182,175,76,244,2,214,167,18,203,8,96,106,216,243,98,7,34,237,204,147,91,78,74,229,36,121,15,220,122,140,82,226,74,226,33,235,33,199,192,13,152,32,31,167,160,197,74,176,57,89,177,53,57,138,198,62,93,174,168,28,32,219,7,183,113,163,110,140,152,145,240,6,114,187,196,137,18,57,72,156,252,159,168,174,136,16,248,246,176,149,137,213,94,99,110,184,206,79,211,241,168,22,98,46,136,73,106,30,150,216,178,153,99,122,204,141,86,132,202,218,156,121,31,108,132,172,118,146,174,110,77,126,8,73,146,169,87,131,155,217,96,214,31,182,50,113,233,203,128,56,110,45,192,115,18,104,246,43,236,211,11,130,61,200,23,171,182,148,171,192,76,86,173,246,156,19,124,215,78,145,134,221,55,128,164,182,112,182,81,6,9,33,4,61,19,117,60,127,13,177,0,166,113,42,114,195,84,21,137,86,84,24,144,103,158,33,93,208,74,184,129,170,185,226,226,214,244,31,171,138,42,167,157,229,43,195,247,156,10,8,212,124,79,61,81,191,52,129,42,234,212,151,175,64,2,76,121,218,17,33,166,90,50,200,68,83,173,90,218,208,159,39,27,142,195,24,99,141,131,136,40,12,181,243,180,83,7,154,167,104,255,80,111,174,102,179,241,232,35,167,130,192,122,250,169,202,206,1,199,152,71,82,66,209,128,0,96,145,32,80,18,168,83,137,159,185,129,239,222,153,233,225,108,67,242,136,88,149,84,50,10,15,201,45,112,32,148,96,209,138,153,5,226,141,153,13,180,223,99,81,11,66,209,188,151,44,114,151,186,142,181,242,241,72,106,33,39,107,56,234,121,92,253,76,203,206,83,80,251,237,47,27,214,195,195,1,68,48,228,211,241,154,10,111,207,75,211,235,193,172,119,1,37,94,165,132,112,80,39,246,184,58,177,40,134,67,156,9,6,232,135,10,236,129,41,108,234,236,248,211,167,224,33,159,134,145,52,84,178,56,21,45,58,103,239,99,35,138,45,34,163,61,69,45,197,132,183,96,102,119,26,5,1,88,61,209,146,218,182,141,26,200,113,18,211,191,124,105,30,73,246,51,147,220,245,124,73,160,239,196,206,103,8,123,84,21,85,170,30,232,149,67,201,216,187,236,158,183,24,152,1,167,20,160,34,46,124,190,170,236,230,75,136,189,56,98,122,253,17,180,17,182,241,88,210,60,34,156,102,206,146,147,133,83,121,17,27,54,116,183,73,52,126,215,169,227,83,148,17,84,252,62,67,1,60,80,42,40,94,231,236,32,57,11,64,58,2,235,5,254,41,152,168,220,36,111,199,199,218,99,79,178,220,24,139,173,49,254,249,235,111,101,13,240,161,255,56,133,201,224,199,139,217,145,36,10,178,229,52,248,39,208,24,246,223,30,203,98,95,180,156,68,240,9,36,222,116,123,239,142,36,177,47,90,78,98,126,128,68,29,2,3,85,153,10,32,136,155,221,132,79,189,248,212,98,191,19,42,251,213,190,4,244,136,122,95,2,217,220,66,6,207,228,169,253,88,32,90,68,125,10,209,28,102,142,41,127,38,211,56,238,11,84,139,176,79,161,154,7,205,113,157,63,147,171,14,204,2,213,34,234,83,168,230,48,115,76,221,103,50,77,74,124,129,107,17,247,41,92,119,80,139,237,201,229,121,119,214,207,117,199,89,184,37,205,180,110,5,202,182,100,93,77,190,103,56,203,94,226,90,95,195,106,237,62,104,22,122,163,225,224,28,48,62,241,181,164,254,156,136,192,247,254,147,30,61,109,37,74,250,28,126,183,202,20,197,95,39,158,47,92,184,79,147,244,255,97,253,249,54,136,99,122,75,42,198,202,135,50,219,128,111,252,222,169,52,27,240,75,155,47,38,153,52,190,149,164,12,62,214,253,0,5,63,148,167,138,222,141,134,158,106,50,102,229,69,238,16,86,187,243,123,61,145,68,165,174,140,157,16,31,228,70,221,41,77,165,43,133,176,236,132,147,141,128,42,82,81,148,128,164,115,133,244,234,167,111,176,38,186,162,119,20,94,36,250,200,6,92,37,117,104,194,5,50,55,118,200,119,206,196,118,225,198,119,150,222,46,133,62,218,92,189,138,90,168,74,172,214,222,66,20,234,199,91,172,44,212,1,200,102,219,209,79,58,240,209,9,208,98,123,10,66,55,209,224,238,107,8,93,173,194,221,49,21,84,252,34,196,226,32,196,162,4,66,85,141,34,198,252,32,198,188,4,67,213,243,34,70,112,16,35,40,193,208,133,182,8,194,15,130,240,4,36,63,54,74,211,90,59,79,53,234,80,3,206,116,212,40,231,212,124,97,162,86,114,187,64,214,153,185,139,40,48,160,172,137,246,163,157,134,26,217,153,82,164,91,173,214,254,94,159,30,216,189,59,14,217,2,52,21,53,243,113,53,121,156,144,179,112,187,164,74,177,213,106,0,6,92,136,62,170,237,49,156,166,101,61,168,76,145,217,100,195,105,216,233,104,194,105,218,217,133,222,249,218,206,93,27,157,111,236,184,121,119,190,181,147,107,134,243,202,206,23,110,231,59,123,175,40,59,175,237,172,184,58,223,219,59,121,238,252,96,67,48,131,106,21,143,160,86,133,20,104,212,81,1,186,226,116,1,93,185,65,41,8,103,51,81,216,145,31,128,194,206,252,168,83,1,108,135,154,26,37,158,95,2,229,237,180,18,8,199,147,73,231,181,158,19,37,30,110,127,241,47,76,35,23,3,205,21,0,0 };