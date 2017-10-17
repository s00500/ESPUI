// TASKS:

// Roadmap
// Build changeable Labels
// Build a serial Console?

var websock;
function start() {
  websock = new WebSocket('ws://' + window.location.hostname + '/ws');
  websock.onopen = function(evt) {
    console.log('websock open');
    $("#conStatus").addClass("color-green");
    $("#conStatus").text("Connected");
   };
  websock.onclose = function(evt) {
    console.log('websock close');
    $("#conStatus").removeClass("color-green");
    $("#conStatus").addClass("color-red");
    $("#conStatus").text("Error / No Connection");
  };
  websock.onerror = function(evt) {
    console.log(evt);
    $("#conStatus").removeClass("color-green");
    $("#conStatus").addClass("color-red");
    $("#conStatus").text("Error / No Connection");
  };
  websock.onmessage = function(evt) {
    console.log(evt);
    var data = JSON.parse(evt.data);
    console.log(data);
    var e = document.body;
    if (data.type === 'domButton') {
      //initial rendering of the Buttons
      $('#row').append("<div class='two columns card'><h5>"+data.label+"</h5><hr/><button onClick='buttonclick(this)'' id="+data.id+"></button></div>");
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
  websock.send("bdown:"+e.id);
  /*
  if release
  websock.send("bup:"+e.id);
  */
}
