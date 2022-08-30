import express from 'express'
import { Server } from "socket.io"
import { createServer } from 'http'
import { readFile } from 'node:fs/promises'
import SerialStart from './resources/test_serialport.js'

const app = express(); 
const server = createServer(app); 
const sockets = new Server(server);
const serial = new SerialStart();

app.use(express.static('public'));
app.use(express.urlencoded({extended: true}));

app.get('/', async (req, res) => {
    res.send(await readFile('./main.html', 'utf-8'));
});

sockets.on('connection', (socket) => {
    // User Login //
    console.log(`User Connected: ${socket.id}`);
    socket.on('disconnect', () => {console.log(`User Disconnected: ${socket.id}`);});
    // Morse Led //
    socket.on('morse', (text) => {
        if (!serial.writeToPort(text)){
            sockets.emit('wait');
        }
    });
})

server.listen(3000, () => {
    console.log("http://localhost:3000");
});