import express from 'express'
import { Server } from "socket.io"
import { createServer } from 'http'
import { readFile } from 'node:fs/promises'
import SerialStart from './resources/test_serialport.js'
import MqttTest from './resources/test_mqtt.js'
import Morse from './resources/test_morseMqtt.js'

const app = express(); 
const server = createServer(app); 
const sockets = new Server(server);

const serial = new SerialStart();
const mqtt = MqttTest();
const morse = Morse();

app.use(express.static('public'));
app.use(express.urlencoded({extended: true}));

app.get('/', async (req, res) => {
    res.send(await readFile('./main.html', 'utf-8'));
});

sockets.on('connection', (socket) => {

    // User Login //
    console.log(`User Connected: ${socket.id}`);
    socket.on('disconnect', () => {console.log(`User Disconnected: ${socket.id}`);});

    // Morse Led  //
    socket.on('morse', (text) => {
        if (!serial.writeToPort(text)){
            sockets.emit('wait');
        }
    });

    // Mqtt //
    socket.on('ledon', (text) => {
        mqtt.client.publish('nodejs', 'on');
        console.log('Published On Message');
    }); 
    socket.on('ledoff', (text) => {
        mqtt.client.publish('nodejs', 'off');
        console.log('Published Off Message');
    });

    // Morse Mqtt //
    socket.on('morseMqtt', (text) => {
        var fText = text.toUpperCase();
        for (var i = 0; i < fText.length; i++) {
            if (!(fText.charAt(i).search(" 1234567890ABCDEFGHIJKLMNOPQRSTUVWYZ"))) break;
            if (fText.charAt(i) == ' ') {
                setTimeout(() => {mqtt.client.publish('nodejs', 'off')} , 7000);
                continue;
            } var code = morse.morsecodes[fText.charAt(i)];
            for (var j = 0; j < code.length; j++) {
                var timeout = 0;
                mqtt.client.publish('nodejs', 'on');
                if (code.charAt(j) == '.') timeout += 1000;
                else if (code.charAt(j) == '-') timeout += 3000;
                setTimeout(() => {mqtt.client.publish('nodejs','off');}, timeout);
                setTimeout(() => {}, 1000);
            } setTimeout(() => {}, 3000); 
        } 
    }); //Fix setTimeout running independently

})

server.listen(3000, () => {
    console.log("http://localhost:3000");
});