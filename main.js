import express from 'express'
import {Server} from "socket.io"
import {createServer} from 'http'
import { readFile } from 'node:fs/promises'
import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline'

const port = new SerialPort({ path: '/dev/ttyACM1', baudRate: 115200, lock: false });
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }))
port.on("open", () => {console.log('> Serial Opened');});
let arduinoBegin = 0; let arduinoWriting = 0;
parser.on('data', data =>{
    if (data == "Started"){
        console.log('> Arduino Started');
        arduinoBegin = 1;
    } else if (data == "Writing") {
        console.log('> Started Writing')
        sockets.emit("on");
        arduinoWriting = 1;
    } else if (data == "Finished"){
        console.log('> Finished Writing')
        sockets.emit("off");
        arduinoWriting = 0;
    } else {
        console.log('> Recieved: ', data);
    }
}); 

function writeToPort(morseCode){
    if (!arduinoBegin) {
        setTimeout(writeToPort(morseCode), 100);
        return 0;
    } else if (arduinoWriting) {
        sockets.emit('wait');
        return 0;
    } port.write(morseCode);
    return 1;
}

const app = express(); 
const server = createServer(app); 
const sockets = new Server(server);

app.use(express.static('public'));
app.use(express.urlencoded({extended: true}));
app.use(function (req, res, next) {
    res.setHeader('Content-Type', 'text/html');
    next();
});

app.get('/', async (req, res) => {
    res.send(await readFile('./main.html', 'utf8'));
});

sockets.on('connection', (socket) => {
    console.log(`User Connected: ${socket.id}`);
    socket.on('morse', (text) => {writeToPort(text);});
    socket.on('disconnect', () => {
        console.log(`User Disconnected: ${socket.id}`);
    });
});

server.listen(3000, () => {
    console.log("http://localhost:3000");
});
