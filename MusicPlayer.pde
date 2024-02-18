import processing.serial.*;
import ddf.minim.*;

Serial myPort;
Minim minim;
AudioPlayer player;
AudioPlayer player2;
byte arbitraryCode = 97;
byte arbitraryCode2 = 67;

void setup()
{
  // In the next line, you'll need to change this based on your USB port name
  String myNewPort = Serial.list() [3];
  
  myPort = new Serial(this, myNewPort, 9600);
  // /dev/tty.usbmodem141301
  minim = new Minim(this);

  // Put in the name of your sound file below, and make sure it is in the same directory
  player = minim.loadFile("lastkwismas.mp3");
  player2 = minim.loadFile("apple.mp3");
}

void draw() {
  while (myPort.available() > 0) {
    int inByte = myPort.read();
    if (inByte == arbitraryCode) {
      player.rewind();
      player.play();
      player2.pause();
    }
    else if (inByte ==arbitraryCode2){
      player.pause();
      player2.rewind();
      player2.play();
  }
}
}
