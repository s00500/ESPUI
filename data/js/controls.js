/*
var keys = {};
connection.onopen = function () {
wsString += " $(\"#connection_status\").toggleClass(\"color-green\");";
wsString += " $(\"#connection_status\").text(\"Connected\");";
wsString += String(" connection.send(")+"\"{'mode': 'check_tb_status'}\");";
for(int i=0; i<tb_index; i++){
  wsString += String(" $(document).on('click',")+"'#tb"+i+"', function() {";
  wsString += String(" if($('#tb")+i+"').hasClass(\"button\")){ console.log(\"Button Clicked!\"); connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'on'}\""+"); }";
  wsString += String(" else if($('#tb")+i+"').hasClass(\"button-primary\")){ connection.send("+"\"{'mode': 'tb_click', 'index': '"+i+"', 'status': 'off'}\""+"); } });";
}
wsString += " };";
wsString += " connection.onerror = function (error) { $('#connection_status').toggleClass(\"color-red\"); $(\"#connection_status\").text(\"Error / No Connection\"); };";
wsString += " connection.onmessage = function (e) {";
wsString += " console.log(e.data); ";
wsString += " var obj = jQuery.parseJSON(e.data);";
wsString += " if(obj.mode === 'create_label'){ $('#row').append(\"<div class='two columns card'><h5>\"+obj.l_title+\"</h5><hr /><h3><span class='label'>\"+obj.l_value+\"</span></h3></div>\");}";
wsString += " else if(obj.mode === 't_button_startup'){ var tb_index = obj.index; for(i=0; i<tb_index; i++){ var tb_index2 = \"tb\"+i; var tb_status = obj[tb_index2]; var tb_index3 = \"#\"+tb_index2;  console.log(tb_status); if(tb_status === \"1\"){$(tb_index3).toggleClass('button-primary'); $(tb_index3).text('Turn Off');} else if(tb_status === \"0\"){$(tb_index3).toggleClass('button'); $(tb_index3).text('Turn On');} } }";
wsString += " else if(obj.mode === 't_button_click'){ var tb_index = \"tb\"+obj.index; var tb_status = obj[tb_index]; var tb_index3 = \"#\"+tb_index;  console.log(tb_status); if(tb_status == \"1\"){ console.log(\"Status Tuned ON\");  $(tb_index3).text('Turn Off'); $(tb_index3).removeClass( \"button\" ).addClass( \"button-primary\" );} else if(tb_status == \"0\"){console.log(\"Status Tuned OFF\"); $(tb_index3).text('Turn On'); $(tb_index3).removeClass( \"button-primary\" ).addClass( \"button\" ); } } };";

*/



// TASKS:
// Control statuses and color TAG
// Request and add JSONFile for dom assembly
// Send WS Commands

// Roadmap
// Build changeable Labels
// Build a serial Console?
var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + '/ws');
  websock.onopen = function(evt) {
    console.log('websock open');
    $("#conStatus").toggleClass("color-green");
    $("#conStatus").text("Connected");
   };
  websock.onclose = function(evt) {
    console.log('websock close');
    $("#conStatus").toggleClass("color-red");
    $("#conStatus").text("Error / No Connection");
  };
  websock.onerror = function(evt) {
    console.log(evt);
    $("#conStatus").toggleClass("color-red");
    $("#conStatus").text("Error / No Connection");
  };
  websock.onmessage = function(evt) {
    console.log(evt);
    var data = JSON.parse(evt.data);
    console.log(data);
    var e = document.body;
    if (data.type === 'domButton') {
      //initial rendering of the Buttons
      $('#row').append("<div class='two columns card'><h5>"+data.label+"</h5><hr/><button id="+"tb"+data.index+"></button></div>");
    }
    if (data.type === 'domLabel') {
      //initial rendering of the Labels
      $('#row').append("<div class='two columns card'><h5>"+data.l_title+"</h5><hr /><h3><span class='label'>"+data.label+"</span></h3></div>");
    }
    else if (data.type === 'value') {
      // Display some kind of value
    }
    else {
      console.log('unknown event');
    }
  };
}

function buttonclick(e) {
  // TODO: Make this more generic
  websock.send(e.id);
}
