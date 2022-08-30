import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline'

export default function SerialStart(){

    const port = new SerialPort({ path: '/dev/ttyACM1', baudRate: 115200, lock: false });
    const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }));

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
        if (!arduinoBegin || arduinoWriting) {return 0;} 
        port.write(morseCode);
        return 1;
    }

    return {
        writeToPort
    }

}