import five from "johnny-five"

const board = new five.Board({
  port: "/dev/ttyACM1"
});

board.on("ready", () => {
  const led = new five.Led(13);
  led.blink(500);
});