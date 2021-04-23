![NoRo Logo](/Media/logo.png)

The repository is meant for Game Programming capstone project at Humber College, developed by [Nghia Tran](http://kyosakura94.com/). 

For this capstone project, I built a OPENGL-based game engine using entirely in C/C++. In order to show off these features, I built a 3D Puzzle style prototype game using my engine.

Feel free to check out my [engine code](NoRoEngine/NoRoEngine/Engine/) or the [game protype](NoRoEngine/NoRoEngine/Game/) that I made. You can also find more media, such as our technical design document or the game engine architecture in [media folder](media).
<br></br>
<br></br>
<p align="center">Check out a trailer of my game below!</p>
<p align="center"><a href="https://youtu.be/c22WljzI1EA"><img src="http://i3.ytimg.com/vi/c22WljzI1EA/maxresdefault.jpg"></a></p>

## NoRoGameEngine Features

NoRoGameEngine features an advanced renderer, fully supported object and material loading using simple Objloader and Assimp library, custom shaders, compute rendering, real-time physics, animations, scene creation and management, an entity-component system, multithreading, basic AI features as us path findings, steering or state machine, audio, input,  event listener, and custom data structures and design patterns. 

## Last Accient Tower - Game prototype

Last Accident Tower is a 3D puzzle game. During the game, the main mission is to collect the item and bring it to the designed destination in order to complete the game. The player needs to controller the character and make it move back and forth by using the WSAD key while carefully avoiding obstacles in the level. 

The game features an opening menu that allows the player to start the game, and move between scenes.

## Using my engine

In order to use my engine to make a game, first make sure that all the libraries reference have been linked properly. Second try an create an instance of NoRoGameEngine, and initialize it with an instance of a GameInterface in the entry point.

```C++
int main(int argc, char* argv[]) 
{
	CoreEngine::GetInstance()->SetGameInterface(new GameTest);

	if (!CoreEngine::GetInstance()->OnCreate("NoRoEngine", 1080, 720))
	{
		return 0;
	}

	//Core game loop
	CoreEngine::GetInstance()->Run();

	printf("\n");

	return 0;
}
```

From here, simply use the concrete implementation of the GameInterface to list and determine which concretely implemented scenes to load, given a new scene index. Scenes (read: levels) represent the sections of gameplay that the player will experience.

## Expanded Credits

Nghia Tran - Engine, Graphics, Animator, Systems programmer
  * [Website](http://kyosakura94.com) | [Github](https://github.com/kyosakura94) | [LinkedIn](https://www.linkedin.com/in/nghiatran-0494/)

## Special Thanks

Thuy Minh - 2D Artist for cube textures  [ArtStation](http://kyosakura94.com)

Duc Huy - 3D Animator for character's animation [ArtStation](http://kyosakura94.com)

Do Quang - 2D Artist for background image [ArtStation](https://www.artstation.com/artwork/B1Wv8m)


## Copyrights

I've also included copyright notices for all of the assets include background images, textures, sound, etc. that I used in the game. They can be found in the copyright pdf in this folder.

I used a few C++ libraries to help build our engine, including FMOD, Nlohmann's JSON library, AssImp, Network Protocols and I used a number of files from OpenGL Tutorial samples for developing shadows. These files include their original licenses.
