import * as mqtt from "mqtt"

export default function MqttTest(){
    
    const host = 'broker.emqx.io'
    const port = '1883'
    const clientId = `mqtt_${Math.random().toString(16).slice(3)}`

    const connectUrl = `mqtt://${host}:${port}`

    const client = mqtt.connect(connectUrl, {
        clientId,
        clean: true,
        connectTimeout: 4000,
        username: 'emqx',
        password: 'public',
        reconnectPeriod: 1000,
    })

    client.on('connect', function () {
        console.log('> Mqtt Connected')
        client.subscribe('arduino');
        console.log('> Subscribed to Arduino');
    });

    client.on('message', function (topic, payload) {
        var message = payload.toString();
        console.log('Recieved:', message);
    });

    return {
        client
    }

}