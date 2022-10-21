import * as mqtt from "mqtt"

export default function MQTT(){
    
    const clientId = `mqtt_${Math.random().toString(16).slice(3)}`
    const connectUrl = 'mqtt://broker.emqx.io:1883'
    const client = mqtt.connect(connectUrl, {
        clientId,
        username: 'emqx',
        password: 'public',
    });

    return {
        client,
        clientId
    }

}