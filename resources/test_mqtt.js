import * as mqtt from "mqtt"

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
    console.log('Connected')
    client.subscribe('nodejs');
    console.log('Subscribed');
    client.publish('nodejs', 'Test Message');
    console.log('Published');
});

client.on('message', function (topic, payload) {
    var message = payload.toString();
    console.log('Recieved:', message);
});