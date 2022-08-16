import express, { response } from 'express'
import {Server} from "socket.io"
import {createServer} from 'http'
import { readFile } from 'node:fs/promises'

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

app.post('/', (req, res) => {
    console.log(req.body);
    res.redirect('/');
});

sockets.on('connection', (socket) => {
    console.log(`User Connected: ${socket.id}`);
    socket.on('disconnect', () => {
        console.log(`User Disconnected: ${socket.id}`);
    })
});

setInterval(() => {
    sockets.emit("blink");
    console.log("Trying to blink led")
}, 10000);

server.listen(3000, () => {
    console.log("http://localhost:3000");
});