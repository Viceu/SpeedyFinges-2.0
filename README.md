# Speedy Fingies 2.0

**This is a music rhythm game! Grab a friend, pick a side with its unique play pin (button or touch pad), and play along and vibe with the music! See who wins at the end... (with a surprised ending music to vibe with as well~) Or, play solo and try to beat your last score!**


### Inspiration
We made a sequel to a game we made from last year, as we wished to revamp it with our new skillset. We improved the game play functionality and added key features such as multi player, multi sensors, bigger screen, and music alongside its transcribed rhythm map! The original inspiration is having a ddr game without the physical activity required.

### How we built it
We used an Arduino Due to control the LED matrix driver and get input from the sensors. Additionally, we used an Adafruit library to program the LED matrix with the driver. We used a software called Processing to read cues from serial communication and play music at the right time from a laptop.

### Challenges we faced
Debugging the LED matrix was a major challenge since the base shield we were using was interfering with our I2C device. After recoordinating our I2C devices, we also encountered frequent problems with the driver and power connection with our computer.

### Accomplishments that we're proud of
The Speedy Fingies 2.0 is a complete upgrade from the previous version, totally exceeding our expection. With our new hardware knowledge, specifically regarding the driver and I2C, and our new foundings on useful tools and libraries, we were able to implement all the functions we had imagined for this game. We're proud of being able to make the LED driver working smoothly after all the connection problems, and we were able to use the LED matrix to its fullest basic functions for explicit UI, including animation, image display, and text scrolls.

### What we learned
We learned how to systematically debug I2C communication and the LED driver. We also learned how to make use of custom built libraries for specific devices, namingly the Adafruit driver and its LED and GFX library. On top of that, we found out about Processing, its usages and how to connect to our external devices so music can be played alongside the game display.

## What's next?
We would add more music and incorporate a more engaging input device, like a gesture sensor or a sound sensor where the person has to scream on beat. As well, having another screen to display additional information such as the music or real time score would be useful.
