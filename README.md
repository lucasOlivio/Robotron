# Robotron

## About

Project 1 for "Game frameworks and patterns" class. A replica of robotron to demonstrate the design patterns seen in class.

## Requirements

- Visual studio 2022

## How to run

1. Unzip all libs from the .zip file in the "extern/lib/" folder
2. Open the solution in VS
3. Build first the "Engine" project, then run the startup project
4. If using the .exe generated remember that it should be at the same folder as the "assets" and "configs" folder
5. Press P to start playing.

## How to play

1. A/W/S/D to move the character.
2. SPACE to shoot.
3. Use your mouse to aim.
4. Avoid everything, except the humans! save the humans!

## Questions

- All characters and moving objects are implemented their base functionality using the "iGameObject" interface for the manager not having to know about all the different types and the "GameCharacter" abstract class to implement the basic load and update of all the game objects.
- The details for each different gameplay for enemies and player and projectiles are in more specific classes like "Player" or "BouncingProjectile" for the tanks orbs.
- It uses the "GameObjectFactory" as the abstract factory.
- The "iGameMediator" as the mediator for all the gameobjects to find other or spawn other gameobjects etc.
- The "ComponentBuilder" as the Builder for all the components for the Scene, so it doesn't need to known and implement the creating and initial setting for each component.
- The "Input" as the PIMPL for the GLFW window and key management, so the other gameplay can get mouse input and position and key inputs without the need of the GLFW lib.
- Interface pattern is used for most of the generic implementations, like the "iGameObject" and "iComponent", so the game manager and the scene can handle them in a standard flow and without changes for each game object or component added.

## How to use

- To start a new game press P
- To change any model, light or camera location you can just change the parameters on the "scene.json" file
- To change in the scene you can also use these controls:
  - Close Window: ESC - Close the window when the ESC key is pressed.
  - Save Scene: F1 - Save the scene when the F1 key is pressed.
  - Reload Scene: F5 - Reload the scene when the F5 key is pressed.

  - PAGE DOWN/PAGE UP: Change selected entity
  - ARROW LEFT/ARROW RIGHT: Change selected component
  - ARROW UP/ARROW DOWN: Change selected property

  - A/D - Change selected parameter in the X axis.
  - Q/E - Change selected parameter in the Y axis.
  - W/S - Change selected parameter in the Z axis.
