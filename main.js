import express from 'express'
import { Server } from "socket.io"
import { createServer } from 'http'
import { readFile } from 'node:fs/promises'
import MQTT from './mqtt.js'

const app = express(); 
const server = createServer(app); 
const sockets = new Server(server);
const mqtt = MQTT();

app.use(express.static('public'));
app.use(express.urlencoded({extended: true}));
app.get('/', async (req, res) => {
    res.send(await readFile('./main.html', 'utf-8'));
});

mqtt.client.on('connect', function () {
    console.log('> MQTT Server Connected')
}); 
mqtt.client.on('message', function (topic, payload) {
    var message = payload.toString();
    console.log('> Message From Arduino:', message);
}); 

mqtt.client.subscribe('arduino');
mqtt.client.publish('nodejs','test');

server.listen(3000, () => {
    console.log("http://localhost:3000");
});