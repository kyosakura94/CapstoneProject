![NoRo Logo](/Media/logo.png)

The repository is meant for Game Programming capstone project at Humber College, developed by [Nghia Tran](http://kyosakura94.com/). 

For our this capstone project, I built a OPENGL based game engine entirely in C/C++, which we started writing in late August, 2020. In order to show off these features, we built a Mario-party style game entirely using our engine, which we started working on in January, 2021.

Feel free to check out my [engine code](NoRoEngine/NoRoEngine/Engine/) or the [game protype](NoRoEngine/NoRoEngine/Game/) that I made. You can also find more media, such as our technical design document or the game engine architecture in [media folder](media).
<br></br>
<br></br>
<p align="center">Check out a trailer of my game below!</p>
<p align="center"><a href="https://youtu.be/c22WljzI1EA"><img src="https://img.youtube.com/vi/PAHP0r7vL6A/0.jpg"></a></p>

## NoRoGameEngine Features

NoRoGameEngine features an advanced renderer, fully supported object and material loading using simple Objloader and Assimp library, custom shaders, compute rendering, real-time physics, animations, scene creation and management, an entity-component system, multithreading, basic AI features as us path findings, steering or state machine, audio, input,  event listener, and custom data structures and design patterns. 

## Last Accient Tower - Game prototype

Players move around a small board game. Each space has a different function (play a minigame, gain gold, lose gold etc.). Players compete in minigames, with the winner gaining gold. The player with the most gold at the end of the last round will win the game. A mix of both luck and skill is required!

The game features an opening menu that allows the player to play the game normally, or pick from a list of hand-crafted minigames. 

## Using our engine

In order to use our engine to make a game, create an instance of OdeumEngine, and initialize it with an instance of a GameInterface in your entry point.

```C++
  OdeumEngine* app;
  app = new OdeumEngine();
  app->SetGameInterface(new Game1);
  ASSERT(app != nullptr, "App is null!");

  if (app->Initialize())
	  app->Run();

  SAFE_DELETE(app);
```

From here, simply use the concrete implementation of the GameInterface to list and determine which concretely implemented scenes to load, given a new scene index. Scenes (read: levels) represent the sections of gameplay that the player will experience.

## Expanded Credits

Aidan Zizys - Engine, Graphics, Systems programmer
  * [Website](https://aidanzizysgamedev.com) | [Github](https://github.com/azgame) | [LinkedIn](https://www.linkedin.com/in/aidan-zizys/)

Benjamin Sayliss - Engine, Physics programmer
  * [Website](https://benjaminsaylissgamedev.com) | [Github](https://github.com/BenSayliss) | [LinkedIn](https://www.linkedin.com/in/benjamin-sayliss-407311177/)

Robert Ward - Gameplay programmer
  * [Website](https://robertwardgamedev.com) | [Github](https://github.com/docdodo) | [LinkedIn](https://www.linkedin.com/in/robert-ward-710234175/)

Ryan Thorne - Generalist programmer
  * [Website](https://www.ryanthornegamedev.com) | [Github](https://github.com/mongoose9304) | [LinkedIn](https://www.linkedin.com/in/ryan-thorne-250366175/)

Diego Gregori - Generalist programmer
  * [Website](https://www.diegogregorigamedev.com) | [Github](https://github.com/dgmode21) | [LinkedIn](https://www.linkedin.com/in/diego-gregori/)

## Copyrights

We've included copyright notices for all of the assets (models, textures, sound, etc.) we used in our game. They can be found in the copyright pdf in this folder.

We used a few C++ libraries to help build our engine, including FMOD, ImGui, Nlohmann's JSON library, AssImp, and we used a number of files from Microsoft's Mini Engine sample set for developing our renderer. These files include their original licenses.
