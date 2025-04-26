[Temporary readme] [WIP]

C++ Raylib Project for OOP University Class

## Idea
  You're a tiny submarine that wants to explore the depths of an underwater cavern system equipped with a tiny mining laser and some rudimentary defense systems. Your goal is to mine for resources, explore the caverns, upgrade your tools and ward off enemies that get more and more scary as you go deeper.

## Inspiration
- Sebastian Lague Cube Marched Submarine Terrain

## Implementations
  The project uses Raylib framework for rendering and makes extensive use of the gpu for rendering the terrain with a custom shader.

## Milestones
- [X] Terrain
  - [X] Rendering
  - [X] Chunking
  - [X] Mining
  - [X] Collisions
- [ ] Player
  - [X] Movement
  - [ ] Game feel
  - [ ] Basic animations
  - [ ] Tool system
  - [ ] Oxygen and respawn system (after you die you can go to the location where you died and see your remains (maybe loot))
- [ ] Enemies
  - [ ] Rudimentary enemy movement
  - [ ] Enemy behaviours (Predators/Prey)
  - [ ] Chunk and area based optimizations
  - [ ] Different enemy types
- [ ] Exploration features
  - [ ] Points of interest within the caverns
- [ ] Other Gameplay features
  - [ ] Story?
  - [ ] Goal?

## Administrative details
#### Cerinte generale
- [X] Folosirea coerenta și motivată a specificatorilor de acces (`private`, `protected`, `public`)
- [X] Folosirea coerentă și motivată a `const` și referință `&`
- [X] Teste
- [X] Fara memory leakuri

#### Cerinte Tema 1
- [X] Campuri + Metode Publice/Private
- [X] Constructori
- [X] Destructori
- [X] Supraincarcare de operatori: in `Component` si `CollisionInfo`

#### Cerinte Tema 2
- [X] Mosteniri: de la `Component`
- [X] Functii virtuale: in `Component`
- [X] Exceptii (si handling)
- [ ] Utilizarea `dynamic_cast` sau `static_cast`
- [X] Bonus: O clasa abstracta: `Component`
