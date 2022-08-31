import { io } from "https://cdn.socket.io/4.4.1/socket.io.esm.min.js";
const socket = io();
document.getElementById("ledon").addEventListener("click", () => {socket.emit('ledon');}); 
document.getElementById("ledoff").addEventListener("click", () => {socket.emit('ledoff');}); 
document.getElementById("but").addEventListener("click", function() {
    socket.emit('morseMqtt', document.getElementById("morse").value);
}); var doc = document.getElementById("led").getSVGDocument();
socket.on('connect', () => {
    console.log(`Client Connected: ${socket.id}`);
    socket.on('on', (state) => {
        doc.getElementById("lamp1").setAttribute("style", "fill: red; filter: drop-shadow(0 0 2rem red);");
        doc.getElementById("lamp2").setAttribute("style", "fill: red; filter: drop-shadow(0 0 2rem red);");
        document.getElementById("but").style.backgroundColor = "crimson";
    });
    socket.on('off', (state) => {
        doc.getElementById("lamp1").setAttribute("style", "fill: red; filter: brightness(50%);");
        doc.getElementById("lamp2").setAttribute("style", "fill: red; filter: brightness(50%);");
        document.getElementById("but").style.backgroundColor = "green";
        document.getElementById("myPopup").classList.remove("show");
    });
    socket.on('wait', (state) => {
        document.getElementById("myPopup").classList.add("show");
    });
});