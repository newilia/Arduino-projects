var socket = new WebSocket('ws://' + window.location.hostname + ':81/');
socket.onmessage = function(event) {
	console.log("Received message: " + event.data);
}

document.getElementById('reset').onclick = function()
{
	console.log("reset");
	socket.send('reset');
}

document.getElementById('test').onclick = function()
{
	console.log("test");
	socket.send('test');
}