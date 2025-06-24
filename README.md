# Snake Remake

<video src="https://github.com/user-attachments/assets/98068975-f146-4a58-98b8-93df47ea64d7" width="500" autoplay loop muted playsinline></video>

## About

This was my pet project and, at the same time, my coursework at the university.  

It's called "Remake" because it's an improved version of my snake game from the 1st year that looked like this:  

<img src="https://github.com/user-attachments/assets/5841ae6c-147f-4871-9f45-017c49af53ee" width="500">

## Key features

- **New nice graphics**
- **Procedural animations**
- **3 types of food:**  
    - regular (red apple)
    - big (golden apple)
    - poison (skull and bones)
- **10 different beautiful snake colors, that automatically change each new game**
- **Wide modding capabilities (textures & snake colors)**

## Modding

Snake Remake is very easy to mod. All game files are located in ```Resources/``` folder.

### Textures

To mod any texture, go to ```Resources/Images/```. Be sure that texture's name remains the same.

### Colors

You can change the colors of the snake's body and the color of the cursor. All of them are defined in ```Resources/Colors.json```.  

Colors are written in ```[r, g, b]``` format, where each value is a number from 0 to 255. Any other format is unacceptable and will cause an error.  

The snake's colors are written in the form of color pairs that have the following format: ```[[head color], [tail color]]```. They are located in the ```body``` array. All intermediate colors between them are calculated at runtime. You can add as many color pairs as you want, but there must be at least one valid pair for game to run.

## Building from source

### 1. Compile:
To build with MinGW (GCC), use this commands:

```
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

If you are using another compiler, be sure to replace ```libraylib.a``` in ```Libs/``` with compatible one.

### 2. Pack

Be sure that compiled ```.exe``` file and the ```Resources/``` folder are located in the same directory.
