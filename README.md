# Screenshot
![screenshot_2024-02-25-024106](https://github.com/SAANN3/QtOpengl-try/assets/95036865/8d647e09-67bb-461a-82a9-5755ccc80548)
# What is it?
This was my try to learn some basics of opengl. I started working with "qt" version of it because i already knew this framework.
Maybe this code can help you to solve some problems. If you wish to change objects,position or sizes, you should look at function ```initializeGL()``` in glwindow.cpp.

There are many appends of basic rectangle that take as parameters : width,height,lenght,transparency(which stopped working after i added lighting),x position in world,y and z)

For example this big rectangle working as a ground
```
rectangles.append(new Rectangle(1000.0,0.5,1000.0,1.0,0.0,-10.0,0));
```
Also at if you dislike red background you can change it in same function
```
glClearColor(1.0f, 0.2f, 0.2f, 1.0f);
```
change values inside to your taste


Also not having files in src folder and instead in root directory feels very weird to me

# Movement
Simply looking at one picture is quite boring, this means we should find a way to move around, right?

Function ```keyPressEvent``` in the same, glwindow.cpp, helps us with that. Here are movements(also remember that movement done according to camera view)
| Keys | Job   
|:-:|:-: 
| w | moving forward(where are you currently looking)
| s | moving backwards
| a | moving left
| d | moving right
| E | move light position forward on x axis
| Q | move light position backwards on x axis
| Z | move light up
| X | move down

Also if you hold a left mouse button, you can change camera view(rotate camera)
# How to use?
Haven't tested on windows(i think it'll work) but on linux it straightforward - Clone repo, open QtCreator, open repo in it and run
